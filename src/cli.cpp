#include "cli.hpp"

// void afficherChemin(const vector<Arret>& chemin, const vector<Ligne>& lignesDistinctes, const vector<int>& indicesChangement) {
//     cout << "Chemin entre les arrêts :\n" << chemin.front().stopName << " -> " << chemin.back().stopName << endl;

//     // Afficher les lignes de transport
//     cout << "Lignes : ";
//     size_t ligneIndex = 0;
//     for (size_t i = 0; i < chemin.size(); ++i) {
//         // Afficher la ligne actuelle
//         cout << lignesDistinctes[ligneIndex].nomLigne;

//         // Vérifier si un changement de ligne est nécessaire
//         if (ligneIndex < indicesChangement.size() && i == static_cast<size_t>(indicesChangement[ligneIndex])) {
//             ++ligneIndex; // Passer à la ligne suivante
//         }

//         if (i < chemin.size() - 1) {
//             cout << "      "; // Espacement entre les lignes
//         }
//     }
//     cout << endl;

//     // Afficher les arrêts
//     cout << "Arrêts : ";
//     for (size_t i = 0; i < chemin.size(); ++i) {
//         cout << chemin[i].stopName; // Afficher le nom de l'arrêt
//         if (i < chemin.size() - 1) {
//             cout << "      "; // Espacement entre les arrêts
//         }
//     }
//     cout << endl;
// }

// void afficherChemin(const vector<Noeud>& chemin, const vector<int>& indicesChangement) {
//     if (chemin.empty()) {
//         cout << "Aucun chemin trouvé entre les arrêts spécifiés." << endl;
//         return;
//     }

//     cout << "Chemin entre les arrêts :\n"
//               << chemin.front().arretId << " -> " << chemin.back().arretId << endl;

//     // Afficher les lignes de transport
//     cout << "Lignes : ";
//     size_t ligneIndex = 0;
//     for (size_t i = 0; i < chemin.size(); ++i) {
//         cout << chemin[i].ligneId; // Afficher l'ID de la ligne

//         // Vérifier si un changement de ligne est nécessaire
//         if (ligneIndex < indicesChangement.size() && i == static_cast<size_t>(indicesChangement[ligneIndex])) {
//             ++ligneIndex; // Passer à la ligne suivante
//         }

//         if (i < chemin.size() - 1) {
//             cout << "      "; // Espacement entre les lignes
//         }
//     }
//     cout << endl;

//     // Afficher les arrêts
//     cout << "Arrêts : ";
//     for (size_t i = 0; i < chemin.size(); ++i) {
//         cout << chemin[i].arretId; // Afficher l'ID de l'arrêt
//         if (i < chemin.size() - 1) {
//             cout << "      "; // Espacement entre les arrêts
//         }
//     }
//     cout << endl;

//     // Afficher les horaires
//     cout << "Horaires : ";
//     for (size_t i = 0; i < chemin.size(); ++i) {
//         cout << chemin[i].heure.heure << ":" << setw(2) << setfill('0') << chemin[i].heure.minute;
//         if (i < chemin.size() - 1) {
//             cout << "      "; // Espacement entre les horaires
//         }
//     }
//     cout << endl;
// }

std::string nettoyerUTF8(const std::string& str) {
    std::string resultat;
    size_t i = 0;

    while (i < str.size()) {
        unsigned char c = str[i];

        // Vérifier si c est un caractère ASCII (1 octet)
        if ((c & 0x80) == 0) {
            resultat += c;
            i++;
        }
        // Vérifier si c est un caractère UTF-8 valide (2, 3 ou 4 octets)
        else if ((c & 0xE0) == 0xC0 && i + 1 < str.size() && (str[i + 1] & 0xC0) == 0x80) {
            resultat += c;
            resultat += str[i + 1];
            i += 2;
        } else if ((c & 0xF0) == 0xE0 && i + 2 < str.size() &&
                   (str[i + 1] & 0xC0) == 0x80 && (str[i + 2] & 0xC0) == 0x80) {
            resultat += c;
            resultat += str[i + 1];
            resultat += str[i + 2];
            i += 3;
        } else if ((c & 0xF8) == 0xF0 && i + 3 < str.size() &&
                   (str[i + 1] & 0xC0) == 0x80 && (str[i + 2] & 0xC0) == 0x80 &&
                   (str[i + 3] & 0xC0) == 0x80) {
            resultat += c;
            resultat += str[i + 1];
            resultat += str[i + 2];
            resultat += str[i + 3];
            i += 4;
        }
        // Si le caractère est invalide, l'ignorer
        else {
            i++;
        }
    }

    return resultat;
}

vector<string> getStopIdByName(const string& stopName, const unordered_map<string, Arret>& stops) {
    vector<string> stopsList;
    for (const auto& stop : stops) {
        if (stop.second.stopName == stopName) {
            stopsList.push_back(stop.first);
            // return stop.first; // Retourne l'ID de l'arrêt
        }
    }
    if(stopsList.empty())
        cerr << "Erreur : Aucun arrêt trouvé pour le nom '" << stopName << "'." << endl;
    return stopsList; // Retourne une chaîne vide si aucun arrêt n'est trouvé
}

void entreeUtilisateur(unordered_map<string, Arret>* stops, vector<Ligne>* lignes) {
    string departName, arriveeName;
    Horaire h1; // Exemple d'heure de départ
    cout << BLEU << BOLD << "Entrez le nom de l'arrêt de départ : " << RESET;
    getline(cin, departName);
    departName = nettoyerUTF8(departName); // Nettoyer le nom de l'arrêt
    cout << BLEU << BOLD << "Entrez le nom de l'arrêt d'arrivée : " << RESET;
    getline(cin, arriveeName);
    arriveeName = nettoyerUTF8(arriveeName); // Nettoyer le nom de l'arrêt
    cout << BLEU << BOLD << "Entrez l'heure de départ (HH MM) : " << RESET;
    cin >> h1.heure >> h1.minute;

    vector<string> depart = getStopIdByName(departName, *stops);
    vector<string> arrivee = getStopIdByName(arriveeName, *stops);

    if (depart.empty() || arrivee.empty()) {
        if (depart.empty()) {
            cerr << "\033[31mErreur : Impossible de trouver l'arrêt de départ spécifié.\033[0m" << endl;
            erreurEntree(departName, stops); // Afficher l'erreur pour le départ
            cout << endl;
        }

        if (arrivee.empty()) {
            cerr << "\033[31mErreur : Impossible de trouver l'arrêt d'arrivée spécifié.\033[0m" << endl;
            erreurEntree(arriveeName, stops); // Afficher l'erreur pour l'arrivée
            cout << endl;
        }
        return; // Sortir avec une erreur
    }


    if (depart == arrivee) {
        cout << "\033[31mLe départ est le même que l'arrivée.\033[0m" << endl;
        return; // Sortir si le départ et l'arrivée sont identiques
    }

    if (h1.heure < 0 || h1.heure > 23 || h1.minute < 0 || h1.minute > 59) {
        cerr << "\033[31mErreur : Heure de départ invalide.\033[0m" << endl;
        return; // Sortir si l'heure de départ est invalide
    }

    // Demande à l'utilisateur quels arrêts il souhaite privilégier
    string reponse;
    cout << BLEU << BOLD << "Voulez vous rentrer des arrêts intermédiaires ? (O/N) " << RESET;
    cin >> reponse;
    vector<string> arretsPrivilegier;
    if (reponse != "N" && reponse != "n") {
        cout << BLEU << BOLD << "Entrez les arrêts intermédiaires (séparés de cette manière 'Campus ; Derain') : " << RESET;
        string arretsPrivilegierInput;
        cin.ignore(); // Ignorer le caractère de nouvelle ligne restant dans le tampon
        getline(cin, arretsPrivilegierInput);
        stringstream ss(arretsPrivilegierInput);
        string arret;
        while (getline(ss, arret, ';')) {
            arret = nettoyerUTF8(arret); // Nettoyer le nom de l'arrêt

            // Supprimer les espaces en début et en fin de chaîne
            arret.erase(arret.find_last_not_of(" \t") + 1); // Supprimer les espaces en fin
            arret.erase(0, arret.find_first_not_of(" \t")); // Supprimer les espaces en début

            arret = getStopIdByName(arret, *stops)[0]; // Obtenir l'ID de l'arrêt (0 car on part du principe qu'il n'y a qu'un seul ID)
            arretsPrivilegier.push_back(arret);
        }
    }
    // Afficher les arrêts à éviter
    if(arretsPrivilegier.empty()) {
        cout << "Aucun arrêt intermédiaire." << endl;
    } else {
        cout << "Arrêts intermédiaires : ";
        for (const auto& arret : arretsPrivilegier) {
            cout << arret << " ";
        }
        cout << endl;
    }

    // Demande à l'utilisateur quels arrêts il souhaite éviter
    cout << BLEU << BOLD << "Voulez vous rentrer des arrêts à éviter ? (O/N) " << RESET;
    cin >> reponse;
    vector<string> arretsEviter;
    if (reponse != "N" && reponse != "n") {
        cout << BLEU << BOLD << "Entrez les arrêts à éviter (séparés de cette manière 'Campus ; Derain') : " << RESET;
        string arretsEviterInput;
        cin.ignore(); // Ignorer le caractère de nouvelle ligne restant dans le tampon
        getline(cin, arretsEviterInput);
        stringstream ss(arretsEviterInput);
        string arret;
        while (getline(ss, arret, ';')) {
            arret = nettoyerUTF8(arret); // Nettoyer le nom de l'arrêt

            // Supprimer les espaces en début et en fin de chaîne
            arret.erase(arret.find_last_not_of(" \t") + 1); // Supprimer les espaces en fin
            arret.erase(0, arret.find_first_not_of(" \t")); // Supprimer les espaces en début

            arret = getStopIdByName(arret, *stops)[0]; // Obtenir l'ID de l'arrêt (0 car on part du principe qu'il n'y a qu'un seul ID)
            arretsEviter.push_back(arret);
        }
    }
    // Afficher les arrêts à éviter
    if(arretsEviter.empty()) {
        cout << "Aucun arrêt à éviter." << endl;
    } else {
        cout << "Arrêts à éviter : ";
        for (const auto& arret : arretsEviter) {
            cout << arret << " ";
        }
        cout << endl;
    }

    // ajouter le code de vérification de la bonne ville quand il y a plusieurs départ en retirant les autres du vector

    for(int i = 0 ; i < arretsPrivilegier.size() ; i++){
        depart.push_back(arretsPrivilegier[i]);
        arrivee.insert(arrivee.begin()+i, arretsPrivilegier[i]);
    }

    // for(int i = 0 ; i < depart.size() ; i++){
    //     cout << "depart i : " << depart[i] << endl;
    //     cout << "arrivee i : " << arrivee[i] << endl;
    // }

    vector<vector<vector<Noeud>>> cheminsFinaux;
    for(int i = 0 ; i < depart.size() ; i++){
        // cout << "depart[i] = " << depart[i] << endl;
        // for(int j = 0 ; j < arrivee.size() ; j++){
            // cout << "arrivee[j] = " << arrivee[j] << endl;
            // h1.print();
            vector<vector<Noeud>> chemin = Dijkstra(depart[i], arrivee[i], h1, stops, lignes, arretsEviter);
            if(!chemin.empty())
                cheminsFinaux.push_back(chemin);
        // }
    }

    // cout << cheminsFinaux.size() << endl;

    if(cheminsFinaux.empty()){
        cout << ROUGE << "Aucun chemin trouvé entre les arrêts spécifiés." << RESET << endl;
        return; // Sortir si aucun chemin n'est trouvé
    }

    // Afficher le chemin
    // afficherChemin(chemin, indicesChangement, stops, lignes);
    // cout << endl;
    for(int i = 0 ; i < cheminsFinaux.size() ; i++)
        afficherChemin2(cheminsFinaux[i], stops, lignes);
    cout << endl;

    // Demander à l'utilisateur s'il souhaite effectuer ce chemin
    cout << BLEU << BOLD << "Voulez-vous effectuer ce chemin ? (O/N) : " << RESET;
    char choix;
    cin >> choix;
    cin.ignore(); // Ignorer le caractère de nouvelle ligne restant dans le tampon
    if (choix == 'O' || choix == 'o') {
        int etapes = 1;
        int nb_etapes = 0;
        for(int i = 0 ; i < cheminsFinaux.size() ; i++){
            for(int j = 0 ; j < cheminsFinaux[i].size() ; j++){
                nb_etapes += cheminsFinaux[i][j].size(); // Compter le nombre total d'étapes
            }
        }
        cout << "Nombre d'étapes : " << nb_etapes << endl;
        while(etapes <= nb_etapes){
            char choixEtape;
            string nomEtape;

            // Trouver le nom de l'arrêt correspondant à l'étape actuelle
            int compteur = 0;
            for (int i = 0; i < cheminsFinaux.size(); i++) {
                for (int j = 0; j < cheminsFinaux[i].size(); j++) {
                    for (const auto& noeud : cheminsFinaux[i][j]) {

                        compteur++;
                        if (compteur == etapes) {
                            nomEtape = (*stops)[noeud.arretId].stopName; // Récupérer le nom de l'arrêt
                            break;
                        }
                    }
                    if (!nomEtape.empty()) break;
                }
                if (!nomEtape.empty()) break;
            }

            cout << "Etape (" << etapes << " / " << nb_etapes << ") " << nomEtape << " : Valider ? (O/N)" << endl;
            cin >> choixEtape;
            cin.ignore(); // Ignorer le caractère de nouvelle ligne restant dans le tampon
            if (choixEtape == 'O' || choixEtape == 'o') {
                cout << VERT << BOLD << "Etape " << etapes << " validée." << RESET << endl;
            }

            // Redéfinition du chemin
            else {
                cout << JAUNE << "Etape " << etapes << " annulée." << RESET << endl << endl;
                // Re effectue le chemin avec un nouveau départ et une nouvelle heure
                cout << BLEU << BOLD << "Entrez le nom de l'arrêt de départ : " << RESET;
                getline(cin, departName);
                departName = nettoyerUTF8(departName); // Nettoyer le nom de l'arrêt
                if(departName == arriveeName){
                    cout << "\033[31mLe départ est le même que l'arrivée.\033[0m" << endl;
                    return; // Sortir si le départ et l'arrivée sont identiques
                }
                cout << BLEU << BOLD << "Entrez l'heure de départ (HH MM) : " << RESET;
                cin >> h1.heure >> h1.minute;
                if (h1.heure < 0 || h1.heure > 23 || h1.minute < 0 || h1.minute > 59) {
                    cerr << "\033[31mErreur : Heure de départ invalide.\033[0m" << endl;
                    return; // Sortir si l'heure de départ est invalide
                }
                vector<string> depart = getStopIdByName(departName, *stops);
                if (depart.empty() || arrivee.empty()) {
                    if (depart.empty()) {
                        cerr << "\033[31mErreur : Impossible de trouver l'arrêt de départ spécifié.\033[0m" << endl;
                        erreurEntree(departName, stops); // Afficher l'erreur pour le départ
                        cout << endl;
                    }

                    if (arrivee.empty()) {
                        cerr << "\033[31mErreur : Impossible de trouver l'arrêt d'arrivée spécifié.\033[0m" << endl;
                        erreurEntree(arriveeName, stops); // Afficher l'erreur pour l'arrivée
                        cout << endl;
                    }
                    return; // Sortir avec une erreur
                }
                // Recalculer le chemin avec les nouveaux paramètres
                cheminsFinaux.clear(); // Vider les anciens chemins
                for (int i = 0; i < depart.size(); i++) {
                    for (int j = 0; j < arrivee.size(); j++) {
                        vector<vector<Noeud>> chemin = Dijkstra(depart[i], arrivee[j], h1, stops, lignes, arretsEviter);
                        if (!chemin.empty())
                            cheminsFinaux.push_back(chemin);
                    }
                }
                if (cheminsFinaux.empty()) {
                    cout << ROUGE << "Aucun chemin trouvé entre les arrêts spécifiés." << RESET << endl;
                    return; // Sortir si aucun chemin n'est trouvé
                }
                // Réinitialiser le nombre total d'étapes
                nb_etapes = 0;
                for (int i = 0; i < cheminsFinaux.size(); i++) {
                    for (int j = 0; j < cheminsFinaux[i].size(); j++) {
                        nb_etapes += cheminsFinaux[i][j].size(); // Compter le nombre total d'étapes
                    }
                }
                // Afficher le chemin
                afficherChemin2(cheminsFinaux[0], stops, lignes);
                cout << endl;
                etapes = 0; // Réinitialiser le compteur d'étapes
            }
            etapes++;
        }
        cout << endl << VERT << BOLD << "Vous êtes arrivé à votre destination !" << RESET << endl << endl;

    } else {
        cout << BLEU << BOLD << "Merci d'avoir utilisé notre service." << RESET << endl;
    }
}

// Fonction pour calculer la distance de Levenshtein
int levenshteinDistance(const string& s1, const string& s2) {
    size_t len1 = s1.size(), len2 = s2.size();
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));

    for (size_t i = 0; i <= len1; ++i) dp[i][0] = i;
    for (size_t j = 0; j <= len2; ++j) dp[0][j] = j;

    for (size_t i = 1; i <= len1; ++i) {
        for (size_t j = 1; j <= len2; ++j) {
            dp[i][j] = min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + (s1[i - 1] != s2[j - 1])});
        }
    }

    return dp[len1][len2];
}

void erreurEntree(const string& nameArret, unordered_map<string, Arret>* stops) {
    string closestStopName;
    int minDistance = numeric_limits<int>::max();

    // Parcourir tous les arrêts pour trouver le plus proche
    for (const auto& stop : *stops) {
        int distance = levenshteinDistance(nameArret, stop.second.stopName);
        if (distance < minDistance) {
            minDistance = distance;
            closestStopName = stop.second.stopName;
        }
    }

    // Afficher le résultat
    if (!closestStopName.empty()) {
        cout << JAUNE << "Aucun arrêt trouvé pour '" << BOLD << nameArret << RESET << JAUNE << "'. Voulez-vous dire : '" << BOLD << closestStopName << RESET << JAUNE << "' ?" << RESET << endl;
    } else {
        cout << JAUNE << "Aucun arrêt correspondant trouvé dans la base de données." << RESET << endl;
    }

}

void afficherChemin(const vector<Noeud>& chemin, const vector<int>& indicesChangement, unordered_map<string, Arret>* stops, vector<Ligne>* lignes) {
    if (chemin.empty()) {
        cout << "Aucun chemin trouvé entre les arrêts spécifiés." << endl;
        return;
    }
    int indexLigne = 0; // Indice de la ligne actuelle
    string ligneActuel = " " ;// Ligne de départ
    // definir la ligne actuelle
    cout << "DEBUG Ligne actuelle : " << chemin[0].ligneId << endl;
    indexLigne = trouveLigne(chemin[0].ligneId , lignes);
    if(indexLigne != -1) {
        ligneActuel = (*lignes)[indexLigne].nomLigne; // Mettre à jour la ligne actuelle
    }
    cout << "Chemin détaillé entre les arrêts :" << endl;
    for (size_t i = 0; i < chemin.size(); ++i) {
        cout << "Étape " << i + 1 << ":" << endl;
        cout << "  Arrêt : " << (*stops)[chemin[i].arretId].stopName;
        cout << "  Ligne : " << chemin[i].ligneId << " (" << ligneActuel << ")";
        cout << "  Horaire : " << chemin[i].heure.heure << ":" 
             << setw(2) << setfill('0') << chemin[i].heure.minute << endl;

        // Vérifier si un changement de ligne est nécessaire
        if (find(indicesChangement.begin(), indicesChangement.end(), static_cast<int>(i)) != indicesChangement.end()) {
            cout << "  ** Changement de ligne à la prochaine étape**" << endl;
            // Affichage de l'horaire de passage à l'arrêt suivant
            cout << "  Horaire de passage à l'arrêt suivant : " << chemin[i + 1].heure.heure << ":" 
                 << setw(2) << setfill('0') << chemin[i + 1].heure.minute << endl;
            indexLigne = trouveLigne(chemin[i + 1].ligneId , lignes);
            if(indexLigne != -1) {
                ligneActuel = (*lignes)[indexLigne].nomLigne; // Mettre à jour la ligne actuelle
            } else {
                cout << "Erreur." << endl;
                ligneActuel = "JSP";
            }
        }

        cout << endl; // Espacement entre les étapes
    }
}

// Besoin de compter les accents car ils comptent pour 2 caractères
int compterMultiOctets(const string& str) {
    int compteur = 0;

    for (unsigned char c : str) {
        // En UTF-8, un caractère multi-octets commence par un octet dont les bits les plus significatifs sont 110, 1110 ou 11110
        if ((c & 0xC0) == 0xC0) { // Vérifie si c est un octet de début pour un caractère multi-octets
            compteur++;
        }
    }

    return compteur;
}

int compterSequencesCouleurs(const std::string& str) {
    // Liste des séquences de couleurs et styles
    const std::vector<std::string> sequencesCouleurs = {
        RESET, BOLD, ROUGE, VERT, JAUNE, BLEU, ORANGE, BLANC
    };

    int compteur = 0;

    // Parcourir chaque séquence de couleur
    for (const auto& sequence : sequencesCouleurs) {
        size_t pos = str.find(sequence);
        while (pos != std::string::npos) {
            compteur += sequence.size(); // Compter la longueur de la séquence
            pos = str.find(sequence, pos + sequence.size());
        }
    }

    return compteur;
}

void afficherChemin2(const vector<vector<Noeud>>& chemin, unordered_map<string, Arret>* stops, vector<Ligne>* lignes) {
    int taille = 0;

    if (chemin.empty()) {
        cout << ROUGE << "Aucun chemin trouvé entre les arrêts spécifiés." << RESET << endl;
        return;
    }

    const size_t totalWidth = 50;      // largeur totale de l'encadré
    const size_t innerWidth = totalWidth - 4; // espace disponible entre "| " et " |"

    int add_espace = 0;

    auto ligneSeparation = [&]() {
        cout << string(totalWidth, '=') << endl;
    };

    auto printCentered = [&](const string& content) {
        string affichage = content.substr(0, innerWidth);  // tronque si trop long
        size_t totalPadding = innerWidth - affichage.size();
        size_t left = totalPadding / 2;
        size_t right = totalPadding - left;
        cout << "| " << string(left, ' ') << affichage << string(right, ' ') << " |" << endl;
    };

    auto ligneVide = [&]() {
        cout << "|";
        for(int i = 0; i < totalWidth-2; i++){
            cout << " ";
        }
        cout << "|" << endl;
    };

    auto ligneEspaces = [&]() {
        for(int i = 0; i < totalWidth; i++){
            cout << " ";
        }
        cout << endl;
    };

    auto printBoxLine = [&](const string& affichage) {
        // string affichage = content.substr(0, innerWidth);  // tronque si trop long
        int cpt_couleurs = compterSequencesCouleurs(affichage);
        taille = affichage.size() - (cpt_couleurs);
        int cpt = compterMultiOctets(affichage);
        int nb_espaces = (totalWidth - taille);
        // cout << "DEBUG espaces : " << nb_espaces << " = " << totalWidth << " - " << taille << endl;

        if(cpt == 0) nb_espaces -= 2;
        if(cpt == 1) nb_espaces -= 1;
        if(cpt > 1) nb_espaces += (cpt - 2);
        // if(cpt_couleurs != 0) nb_espaces += (5*cpt_couleurs)-1 + add_espace;
        // cout << "DEBUG new nb_espaces : " << nb_espaces << " conditions: cpt " << cpt << " cpt_couleurs " << cpt_couleurs << endl;
        cout << "|";
        cout << affichage;
        cout << RESET;
        for(int i = 0; i < static_cast<int>(nb_espaces); i++){
            cout << " ";
        }
        cout << "|" << endl;
        ligneVide();
    };

    // Choix de l'affichage
    bool afficherEtapes = true; // Par défaut, on affiche les étapes intermédiaires
    // Demander à l'utilisateur s'il veut afficher les étapes intermédiaires
    cout << BLEU << BOLD << "Voulez-vous afficher les étapes intermédiaires ? (O/N) : " << RESET;
    char choix;
    cin >> choix;
    cin.ignore(); // Ignorer le caractère de nouvelle ligne restant dans le tampon
    if (choix == 'N' || choix == 'n') {
        afficherEtapes = false; // Ne pas afficher les étapes intermédiaires
    }
    cout << endl;

    // En-tête
    ligneSeparation();
    printCentered("Chemin detaille");
    ligneSeparation();
    ligneEspaces();

    if (afficherEtapes) {
        // Afficher le départ
        string departName = (*stops)[chemin[0].begin()->arretId].stopName;
        Horaire departHoraire = chemin[0].begin()->heure;
        string departHeure = to_string(departHoraire.heure) + ":" + (departHoraire.minute < 10 ? "0" : "") + to_string(departHoraire.minute);

        ligneSeparation();
        ligneVide();
        string ligneName = "Inconnue";
        int indexLigne = trouveLigne(chemin[0].begin()->ligneId, lignes);
        if (indexLigne != -1) {
            ligneName = (*lignes)[indexLigne].nomLigne;
        }
        string tripHeadsign = (indexLigne != -1) ? (*lignes)[indexLigne].tripHeadsign : "Inconnu";

        string l = string(ORANGE) + string(BOLD) + "  Ligne : " + ligneName + " -> " + tripHeadsign;
        printBoxLine(l);
        l = string(VERT) + string(BOLD) + "  Départ : " + string(BLANC) + departName + string(BLEU) + "  Heure : " + string(BLANC) + departHeure;
        printBoxLine(l);

        // Mode avec étapes intermédiaires
        int currArret = 1;
        for (size_t i = 0; i < chemin.size(); ++i) {
            if(i != 0) {
                // Afficher la ligne de séparation entre les étapes
                ligneSeparation();
                ligneVide();
                string ligneName = "Inconnue";
                int indexLigne = trouveLigne(chemin[i].begin()->ligneId, lignes);
                if (indexLigne != -1) {
                    ligneName = (*lignes)[indexLigne].nomLigne;
                }
                string tripHeadsign = (indexLigne != -1) ? (*lignes)[indexLigne].tripHeadsign : "Inconnu";
                l = string(ORANGE) + string(BOLD) + "  " + ligneName + " -> " + tripHeadsign;
                printBoxLine(l);
            }
            for (auto it = (i == 0 ? chemin[i].begin() + 1 : chemin[i].begin()) ; it != (i == chemin.size() - 1 ? chemin[i].end() - 1 : chemin[i].end()); ++it) {
                const auto& noeud = *it;
                currArret++;
                string arretName = (*stops)[noeud.arretId].stopName;
                string horaire = to_string(noeud.heure.heure) + ":" + (noeud.heure.minute < 10 ? "0" : "") + to_string(noeud.heure.minute);

                string l = string(BLEU) + string(BOLD) +"  Étape " + to_string(currArret) + " : " + string(BLANC) + arretName + string(BLEU) + "  Heure : " + string(BLANC) + horaire;
                printBoxLine(l);
            }

            if (i == chemin.size() - 1) {
                string arriveeName = (*stops)[(chemin[i].end() - 1)->arretId].stopName;
                Horaire arriveeHoraire = (chemin[i].end() - 1)->heure;
                string arriveeHeure = to_string(arriveeHoraire.heure) + ":" + (arriveeHoraire.minute < 10 ? "0" : "") + to_string(arriveeHoraire.minute);

                string l = string(ROUGE) + string(BOLD) + "  Arrivée : " + string(BLANC) + arriveeName + string(BLEU) + "  Heure : " + string(BLANC) + arriveeHeure;
                printBoxLine(l);
            }
            ligneSeparation();
            ligneEspaces();
        }

    } else {
        // Mode par ligne (comme actuellement)
        for (size_t i = 0; i < chemin.size(); ++i) {
            string ligneName = "Inconnue";
            int indexLigne = trouveLigne(chemin[i].begin()->ligneId, lignes);
            if (indexLigne != -1) {
                ligneName = (*lignes)[indexLigne].nomLigne;
            }

            string tripHeadsign = (indexLigne != -1) ? (*lignes)[indexLigne].tripHeadsign : "Inconnu";

            string departName = (*stops)[chemin[i].begin()->arretId].stopName;
            Horaire departHoraire = chemin[i].begin()->heure;

            string arriveeName = (*stops)[(chemin[i].end() - 1)->arretId].stopName;
            Horaire arriveeHoraire = (chemin[i].end() - 1)->heure;

            string departHeure = to_string(departHoraire.heure) + ":" + (departHoraire.minute < 10 ? "0" : "") + to_string(departHoraire.minute);
            string arriveeHeure = to_string(arriveeHoraire.heure) + ":" + (arriveeHoraire.minute < 10 ? "0" : "") + to_string(arriveeHoraire.minute);

            ligneSeparation();
            ligneVide();
            string l = string(VERT) + string(BOLD) + "  Ligne : " + ligneName + " -> " + tripHeadsign;
            printBoxLine(l);
            l = string(BLEU) + string(BOLD) + "  Départ : " + string(BLANC) + departName + string(BLEU) + "  Heure : " + string(BLANC) + departHeure;
            printBoxLine(l);
            l = string(BLEU) + string(BOLD) + "  Arrivée : " + string(BLANC) + arriveeName + string(BLEU) + "  Heure : " + string(BLANC) + arriveeHeure;
            printBoxLine(l);
            ligneSeparation();
            ligneEspaces();
        }
    }
    // Pied de boîte
    ligneSeparation();
    printCentered("Arrivee");
    ligneSeparation();
}


// =================================================
// |             Ligne A -> tripHeadsign           |
// |                                               |
// |     Départ: *nomArret*     Heure: *heure*     |
// |                                               |
// |     Arrivée: *nomArret*    Heure: *heure*     |
// |                                               |
// =================================================


// =================================================
// |             Ligne C -> tripHeadsign           |
// |                                               |
// |     Départ: *nomArret*     Heure: *heure*     |
// |                                               |
// |     Arrivée: *nomArret*    Heure: *heure*     |
// |                                               |
// =================================================


// ==================== Arrivée ====================