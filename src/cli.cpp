#include "cli.hpp"

unordered_map <string, string> villes = {
    {"Perpignan", "PE"},
    {"Rivesaltes", "RI"},
    {"Baixas", "BX"},
    {"Canohès", "CA"},
    {"Saint Laurent de la Salanque", "LS"},
    {"Toulouges", "TO"},
    {"Canet en Roussillon", "CN"},
    {"Llupia", "LL"},
    {"Saint Estève", "ES"},
    {"Sainte Marie la Mer", "MA"},
    {"Cabestany", "CY"},
    {"Le Barcarès", "LB"},
    {"Bompas", "BO"},
    {"Pollestres", "PO"},
    {"Baho", "BA"},
    {"Cassagnes", "CG"},
    {"Calce", "CC"},
    {"Saint Nazaire", "NA"},
    {"Saint Féliu d'Avall", "FE"},
    {"Villeneuve de la Raho", "RA"},
    {"Saleilles", "SA"},
    {"Villelongue de la Salanque", "VS"},
    {"Cases de Pènes", "CP"},
    {"Peyrestortes", "PY"},
    {"Pézilla la Rivière", "PZ"},
    {"Tautavel", "TA"},
    {"Le Soler", "SO"},
    {"Espire de l'Agly", "EA"},
    {"Estagel", "EL"},
    {"Ponteilla", "PT"},
    {"Torreilles", "TR"},
    {"Villeneuve la Rivière", "VI"},
    {"Vingrau", "VN"},
    {"Saint Hyppolyte", "HY"},
    {"Latour de France", "LA"},
    {"Opoul", "OP"}
};

string nettoyerUTF8(const string& str) {
    string resultat;
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

string normaliserNom(const string& str) {
    string resultat;
    for (char c : str) {
        if (isalnum(c) || isspace(c)) { // Conserver uniquement les lettres, chiffres et espaces
            resultat += tolower(c); // Convertir en minuscule
        } else if (c == '-') {
            resultat += ' '; // Remplacer les tirets par des espaces
        }
    }
    return resultat;
}

vector<string> getStopIdByName(const string& stopName, const unordered_map<string, Arret>& stops) {
    vector<string> stopsList;
    string stopNameNormalise = normaliserNom(stopName); // Normaliser le nom de l'arrêt
    for (const auto& stop : stops) {
        string stopNameActuelNormalise = normaliserNom(stop.second.stopName); // Normaliser le nom de l'arrêt
        if (stopNameActuelNormalise == stopNameNormalise) {
            stopsList.push_back(stop.first);
        }
    }
    if(stopsList.empty())
        cerr << "Erreur : Aucun arrêt trouvé pour le nom '" << stopName << "'." << endl;
    return stopsList; // Retourne une chaîne vide si aucun arrêt n'est trouvé
}

string getCityNameById(const string& cityId, const unordered_map<string, string>& villes) {
    for (const auto& ville : villes) {
        if (ville.second == cityId) {
            return ville.first; // Retourne le nom de la ville correspondant à l'ID
        }
    }
    return "Inconnu"; // Retourne "Inconnu" si l'ID n'est pas trouvé
}

string getCityIdByName(const string& cityName, const unordered_map<string, string>& villes) {
    for (const auto& ville : villes) {
        if (ville.first == cityName) {
            return ville.second; // Retourne l'ID de la ville correspondant au nom
        }
    }
    return "Inconnu"; // Retourne "Inconnu" si le nom n'est pas trouvé
}

void choixVille(vector<string>& stopIds, const unordered_map<string, Arret>& stops){
    // Traduis les villes de stopName en id de villes
    vector<string> villeName;
    for (const auto& villeId : stopIds) {
        // Récupère les 2 caractères de l'ID de la ville pour obtenir ville.second
        string stopIdVille = villeId.substr(2, 2); // Récupérer les 2 premiers caractères de l'ID de l'arrêt
        string stopNameVille = getCityNameById(stopIdVille, villes); // Récupérer le nom de la ville
        villeName.push_back(stopNameVille); // Ajouter le nom de la ville à la liste
    }

    // Récup nom de l'arrêt
    string stopName;
    for(const auto& stopId : stopIds) {
        stopName = stops.at(stopId).stopName; // Récupérer le nom de l'arrêt
        break;
    }

    // Vérifie si la ville est déjà présente dans le vecteur
    auto it = unique(villeName.begin(), villeName.end());
    villeName.erase(it, villeName.end()); // Supprime les doublons

    if(villeName.size() == 1) {
        // cout << "Ville unique trouvée : " << villeName[0] << endl;
        return; // Sortir si une seule ville est trouvée
    }

    // Choix utilisateur de la ville
    cout << BLEU << BOLD << "Veuillez choisir une ville pour l'arrêt " << BLANC <<stopName << RESET << BLEU << BOLD <<" : " << RESET;
    for (const auto& stop : villeName) {
        if(stop == villeName.back()){
            cout << stop;
        }
        else{
            cout << stop << ", ";
        }
    }
    cout << endl;
    string choixVilleName;
    getline(cin, choixVilleName); // Demander à l'utilisateur de choisir une ville

    // Vérifie si la ville choisie est valide
    bool villeValide = false;
    for (const auto& stop : villeName) {
        if (normaliserNom(stop) == normaliserNom(choixVilleName)) {
            villeValide = true;
            break; // Sortir de la boucle si la ville est valide
        }
    }
    cout << "Sortie du for" << endl;
    if (!villeValide) {
        cerr << ROUGE << "Erreur : Ville choisie invalide." << RESET << endl;
        return; // Sortir si la ville choisie n'est pas valide
    }

    // Ajouter l'ID de l'arrêt correspondant à la ville choisie
    string bonStopId;
    for (const auto& stopId : stopIds) {
        string stopIdVille = stopId.substr(2, 2); // Récupérer les 2 premiers caractères de l'ID de l'arrêt
        string stopNameVille = getCityNameById(stopIdVille, villes); // Récupérer le nom de la ville
        if (normaliserNom(stopNameVille) == normaliserNom(choixVilleName)) {
            bonStopId = stopId; // Récupérer l'ID de l'arrêt correspondant à la ville choisie
            break; // Sortir de la boucle une fois que l'ID est trouvé
        }
    }

    // Vide stopIds et ajoute uniquement l'id de la ville choisie (donc un seul ajout)
    stopIds.clear(); // Vider le vecteur d'IDs d'arrêts

    stopIds.push_back(bonStopId); // Ajouter l'ID de l'arrêt correspondant à la ville choisie

}

void entreeUtilisateur(unordered_map<string, Arret>* stops, vector<Ligne>* lignes) {
    string departName, arriveeName, villeName;
    Horaire h1; // Exemple d'heure de départ
    cout << BLEU << BOLD << "Entrez le nom de l'arrêt de départ : " << RESET;
    getline(cin, departName);
    departName = nettoyerUTF8(departName); // Nettoyer le nom de l'arrêt
    cout << BLEU << BOLD << "Entrez le nom de l'arrêt d'arrivée : " << RESET;
    getline(cin, arriveeName);
    arriveeName = nettoyerUTF8(arriveeName); // Nettoyer le nom de l'arrêt
    cout << BLEU << BOLD << "Entrez l'heure de départ (HH MM) : " << RESET;
    // Demander à l'utilisateur de saisir l'heure de départ sous un bon format
    while (true) {
        cin >> h1.heure >> h1.minute;

        // Vérifier si l'entrée est valide
        if (cin.fail() || h1.heure < 0 || h1.heure > 23 || h1.minute < 0 || h1.minute > 59) {
            cin.clear(); // Réinitialiser l'état de cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorer les caractères restants dans le tampon
            cerr << ROUGE << "Erreur : Heure invalide. Veuillez entrer une heure au format HH MM (ex : 07 30)." << RESET << endl;
            cout << BLEU << BOLD << "Entrez l'heure de départ (HH MM) : " << RESET;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorer les caractères restants dans le tampon
            break; // Sortir de la boucle si l'entrée est valide
        }
    }

    vector<string> depart = getStopIdByName(departName, *stops);
    vector<string> arrivee = getStopIdByName(arriveeName, *stops);

    if (depart.empty() || arrivee.empty()) {
        if (depart.empty()) {
            cerr << "\033[31mErreur : Impossible de trouver l'arrêt de départ spécifié.\033[0m" << endl;
            erreurEntree(departName, stops); // Afficher l'erreur pour le départ
            cerr << endl;
        }

        if (arrivee.empty()) {
            cerr << "\033[31mErreur : Impossible de trouver l'arrêt d'arrivée spécifié.\033[0m" << endl;
            erreurEntree(arriveeName, stops); // Afficher l'erreur pour l'arrivée
            cerr << endl;
        }
        return; // Sortir avec une erreur
    }

    if (depart == arrivee) {
        cerr << "\033[31mLe départ est le même que l'arrivée.\033[0m" << endl;
        return; // Sortir si le départ et l'arrivée sont identiques
    }

    if (depart.size() > 1){
        choixVille(depart, *stops); // Demander à l'utilisateur de choisir une ville si plusieurs arrêts sont trouvés
    }

    if (arrivee.size() > 1){
        choixVille(arrivee, *stops); // Demander à l'utilisateur de choisir une ville si plusieurs arrêts sont trouvés
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

            vector<string> arretIds = getStopIdByName(arret, *stops); // Obtenir l'ID de l'arrêt (0 car on part du principe qu'il n'y a qu'un seul ID)
            if(arretIds.empty()){
                cerr << "Aucun arrêt trouvé pour le nom '" << arret << "'." << endl;
                erreurEntree(arret, stops); // Afficher l'erreur pour l'arrêt à éviter
                return; // Sortir si aucun arrêt n'est trouvé
            }
            else if(arretIds.size() > 1){
                choixVille(arretIds, *stops); // Demander à l'utilisateur de choisir une ville si plusieurs arrêts sont trouvés
                arret = arretIds[0]; // Prendre le premier ID trouvé
            }
            else{
                arret = arretIds[0]; // Prendre le premier ID trouvé
            }

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

    for(int i = 0 ; i < arretsPrivilegier.size() ; i++){
        depart.push_back(arretsPrivilegier[i]);
        arrivee.insert(arrivee.begin()+i, arretsPrivilegier[i]);
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
            string tempName = arret; // Conserver le nom de l'arrêt pour l'affichage
            arret = nettoyerUTF8(arret); // Nettoyer le nom de l'arrêt

            // Supprimer les espaces en début et en fin de chaîne
            arret.erase(arret.find_last_not_of(" \t") + 1); // Supprimer les espaces en fin
            arret.erase(0, arret.find_first_not_of(" \t")); // Supprimer les espaces en début

            vector<string> arretIds = getStopIdByName(arret, *stops); // Obtenir l'ID de l'arrêt (0 car on part du principe qu'il n'y a qu'un seul ID)
            if(arretIds.empty()){
                cerr << "Aucun arrêt trouvé pour le nom '" << arret << "'." << endl;
                erreurEntree(arret, stops); // Afficher l'erreur pour l'arrêt à éviter
                return; // Sortir si aucun arrêt n'est trouvé
            }
            else if(arretIds.size() > 1){
                choixVille(arretIds, *stops); // Demander à l'utilisateur de choisir une ville si plusieurs arrêts sont trouvés
                arret = arretIds[0]; // Prendre le premier ID trouvé
            }
            else{
                arret = arretIds[0]; // Prendre le premier ID trouvé
            }

            // Véfifie si l'arret à éviter est déjà dans le vector d'arrêts à privilégier
            int trouve = 0;
            for(int i = 0 ; i < arretsPrivilegier.size() ; i++){
                if(arret == arretsPrivilegier[i]){
                    cout << "L'arrêt '" << arret << "' est déjà dans la liste des arrêts à privilégier." << endl;
                    trouve = 1; // Sortir si l'arrêt à éviter est déjà dans la liste des arrêts à privilégier
                }
            }
            if(trouve == 0){
                // Ajouter l'arrêt à éviter au vector d'arrêts à éviter
                // cout << "L'arrêt '" << arret << "' a été ajouté à la liste des arrêts à éviter." << endl;
                arretsEviter.push_back(arret);
            }
            else{
                cout << JAUNE << "Cet arrêt '" << BOLD << tempName << RESET << JAUNE << "' ne peut pas être évité." << RESET << endl;
            }
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

    vector<vector<vector<Noeud>>> cheminsFinaux;
    for(int i = 0 ; i < depart.size() ; i++){
        vector<vector<Noeud>> chemin = Dijkstra(depart[i], arrivee[i], h1, stops, lignes, arretsEviter);
        if(!chemin.empty()){
            cheminsFinaux.push_back(chemin);
            h1 = cheminsFinaux[i][cheminsFinaux[i].size()-1][cheminsFinaux[i][cheminsFinaux[i].size()-1].size()-1].heure;
            h1.ajouterMinutes(1);
        } else{
            cerr << ROUGE << "Aucun chemin trouvé entre les arrêts spécifiés." << RESET << endl;
            return; // Sortir si aucun chemin n'est trouvé
        }
    }

    if(cheminsFinaux.empty()){
        cerr << ROUGE << "Aucun chemin trouvé entre les arrêts spécifiés." << RESET << endl;
        return; // Sortir si aucun chemin n'est trouvé
    }
    else{
        for (size_t i = 1; i < cheminsFinaux.size(); ++i) {
            cheminsFinaux[0].insert(cheminsFinaux[0].end(), cheminsFinaux[i].begin(), cheminsFinaux[i].end());
        }
        cheminsFinaux.resize(1);
    }

    // Afficher le chemin
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
                    cerr << "\033[31mLe départ est le même que l'arrivée.\033[0m" << endl;
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
                        cerr << endl;
                    }

                    if (arrivee.empty()) {
                        cerr << "\033[31mErreur : Impossible de trouver l'arrêt d'arrivée spécifié.\033[0m" << endl;
                        erreurEntree(arriveeName, stops); // Afficher l'erreur pour l'arrivée
                        cerr << endl;
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
                    cerr << ROUGE << "Aucun chemin trouvé entre les arrêts spécifiés." << RESET << endl;
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

int compterSequencesCouleurs(const string& str) {
    // Liste des séquences de couleurs et styles
    const vector<string> sequencesCouleurs = {
        RESET, BOLD, ROUGE, VERT, JAUNE, BLEU, ORANGE, BLANC
    };

    int compteur = 0;

    // Parcourir chaque séquence de couleur
    for (const auto& sequence : sequencesCouleurs) {
        size_t pos = str.find(sequence);
        while (pos != string::npos) {
            compteur += sequence.size(); // Compter la longueur de la séquence
            pos = str.find(sequence, pos + sequence.size());
        }
    }

    return compteur;
}

void afficherChemin2(const vector<vector<Noeud>>& chemin, unordered_map<string, Arret>* stops, vector<Ligne>* lignes) {
    int taille = 0;

    if (chemin.empty()) {
        cerr << ROUGE << "Aucun chemin trouvé entre les arrêts spécifiés." << RESET << endl;
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