#include "dijkstra.hpp"
/*
Noeud::Noeud(string id,string ligne, Noeud* prec, Horaire h){
    arretId = id;
    ligneId = ligne;
    precedent = prec; // precedent = nullptr si c'est le premier noeud
    heure = h; // heure de passage a cet arret
}
*/

int Deja_visite(string id, vector<Noeud>* arretsVisites){
    // on verifie si l'arret est deja dans la liste des arrets visites
    for(int i = 0; i < arretsVisites->size(); i++){
        if(((*arretsVisites)[i]).arretId == id){
            return i; // l'arret est deja dans la liste et retourne son index
        }
    }
    return -1; // l'arret n'est pas dans la liste
}

int Dans_voisins(string id, string ligne, vector<Noeud>* arretsVoisin){
    // on verifie si l'arret est deja dans la liste des arrets visites
    for(int i = 0; i < arretsVoisin->size(); i++){
        if((*arretsVoisin)[i].arretId == id && (*arretsVoisin)[i].ligneId == ligne){
            return i; // l'arret est deja dans la liste et retourne son index
        }
    }
    return -1; // l'arret n'est pas dans la liste
}

int Dans_lignes(string id, vector<Ligne>* arretsLignes){
    // on verifie si l'arret est deja dans la liste des arrets visites
    for(int i = 0; i < arretsLignes->size(); i++){
        if((*arretsLignes)[i].idLigne == id){
            return i; // l'arret est deja dans la liste et retourne son index
        }
    }
    return -1; // l'arret n'est pas dans la liste
}
vector<vector<Noeud>> Dijkstra(string depart, string arrivee, Horaire heure, unordered_map<string, Arret>* stops, vector<Ligne>* lignes){
    vector<vector<Noeud>> cheminFinal;
    vector<Noeud> chemin;

    unordered_map<string, Horaire> heuresDepart;
    vector<string> lignesDirectes;

    while(chemin.empty() || (chemin.end()-1)->arretId != arrivee){
        heuresDepart.clear();
        lignesDirectes.clear();

        if(!chemin.empty()){
            depart = (chemin.end()-1)->arretId;
            heure = (chemin.end()-1)->heure;
            heure.minute += 1;
            if(heure.minute == 60){
                heure.heure += 1;
                heure.minute = 0;
            }
        }
        lignesDirectes = (*stops)[depart].getLignes();
        for(int i = 0 ; i < lignesDirectes.size() ; i++){
            int indexLigne = Dans_lignes(lignesDirectes[i], lignes);
            int indexArret = (*lignes)[indexLigne].getIndArret(depart);
            int id = (*lignes)[indexLigne].getHorairesPrecis(heure, indexArret);
            if(id != -1){
                Horaire h = (*lignes)[indexLigne].horaires[indexArret][id];
                heuresDepart.emplace(lignesDirectes[i], h);
            } else{
                lignesDirectes.erase(lignesDirectes.begin()+i);
                i--;
            }
        }

        Horaire hmin = heuresDepart[lignesDirectes[0]];
        for(int i = 1 ; i < lignesDirectes.size() ; i++){
            if(heuresDepart[lignesDirectes[i]] < hmin){
                hmin = heuresDepart[lignesDirectes[i]];
            }
        }

        chemin = DijkstraAlgo(depart, arrivee, hmin, stops, lignes);
        if(chemin.empty()){
            break;
        }
        cheminFinal.push_back(chemin);
        if(cheminFinal.size() > 5) break;
        cout << cheminFinal.size() << endl;
    }
    cout << "Allez ciao monsieur" << endl;
    if(!chemin.empty() && (chemin.end()-1)->arretId == arrivee)
        return cheminFinal;
    else
        return {};
}

vector<Noeud> DijkstraAlgo(string depart, string arrivee, Horaire heure, unordered_map<string, Arret>* stops, vector<Ligne>* lignes){
    // Dijkstra's algorithm to find the shortest path between two stops
    // depart : id de l'arret de depart
    // arrivee : id de l'arret d'arrivee
    // heure : heure de depart
    // retourne le chemin le plus court entre les deux arrets

    vector<Noeud> arretsVisites;
    vector<Noeud> arretsVoisins;

    vector<string> lignesDirectes;
    vector<string> lignesDirectesDepart;
    vector<string> lignesDirectesArrivee;

    Noeud dernierTraite(depart,"Depart",-1,heure); // dernier noeud traite
    dernierTraite.print();

    if(depart == arrivee){ // si le depart est le meme que l'arrivee
        cout << "Le depart est le meme que l'arrivee" << endl;
        return {dernierTraite}; // on retourne le noeud de depart
    }

    lignesDirectesDepart = (*stops)[depart].getLignes();
    lignesDirectesArrivee = (*stops)[arrivee].getLignes();

    bool interPlein = true;
    sort(lignesDirectesDepart.begin(), lignesDirectesDepart.end());
    sort(lignesDirectesArrivee.begin(), lignesDirectesArrivee.end());
    set_intersection(lignesDirectesDepart.begin(), lignesDirectesDepart.end(),lignesDirectesArrivee.begin(), lignesDirectesArrivee.end(), back_inserter(lignesDirectes));

    cout << "Print avant la destruction" << endl;
    for(int i = 0 ; i < lignesDirectes.size() ; i++)
        cout << lignesDirectes[i] << endl;

    for(int i = 0 ; i < lignesDirectes.size() ; i++){
        int indexLigne = Dans_lignes(lignesDirectes[i], lignes);
        int indexArretDepart = (*lignes)[indexLigne].getIndArret(depart);
        Horaire hDepart = (*lignes)[indexLigne].getDernierHoraire(indexArretDepart);
        int indexArretArrivee = (*lignes)[indexLigne].getIndArret(arrivee);
        Horaire hArrivee = (*lignes)[indexLigne].getDernierHoraire(indexArretArrivee);

        if(hDepart > hArrivee){
            lignesDirectes.erase(lignesDirectes.begin()+i);
            i--;
        }
    }

    cout << "Print après la destruction" << endl;
    for(int i = 0 ; i < lignesDirectes.size() ; i++)
        cout << lignesDirectes[i] << endl;

    if(lignesDirectes.empty()){
        interPlein = false;
    }
    // exit(0);

    // Variable utile dans le while
    int indActuel = 0; // indice de l'arret actuel dans la liste des arrets visites
    //utile pour le premier for
    Horaire h, temps_max;
    string tempoLigne, tempoArret;
    Noeud tempo ;
    int indexLigne, indexHeure, indexArret, indexVoisin, j;

    // definition du temps max
    // (*stops)[arrivee].print();
    // (*stops)[arrivee].printLignes();
    if(!interPlein)
        lignesDirectes = (*stops)[arrivee].getLignes(); // on récupère les lignes qui passent par l'arrêt d'arrivée

    for (int i = 0; i < lignesDirectes.size(); i++) {
        // cout << "on entre dans le for (du max)" << endl;
        indexLigne = Dans_lignes(lignesDirectes[i], lignes); // on passe l'ID de la ligne
        if (indexLigne != -1) { // si la ligne existe
            indexArret = (*lignes)[indexLigne].getIndArret(arrivee); // on récupère l'indice de l'arrêt
            h = (*lignes)[indexLigne].getDernierHoraire(indexArret); // on récupère l'heure de passage à cet arrêt
            if (temps_max < h) temps_max = h; // on compare les heures de passage
        }
    }
    // cout << "temps_max : " << temps_max.heure << ":" << temps_max.minute << endl; // on affiche l'heure max
    if(!interPlein)
        lignesDirectes = (*stops)[depart].getLignes();

    while(dernierTraite.arretId != arrivee){ // tant que le noeud de depart n'est pas le meme que le noeud d'arrivee
        // (*stops)[dernierTraite.arretId].print();
        // (*stops)[dernierTraite.arretId].printLignes();
        // on cherche les voisins du dernier noeud traite
        dernierTraite.print();
        //ajout des voisins directs du dernier noeud traite
        for (int i = 0; (i < lignesDirectes.size()) ; i++) {
            if(!arretsVisites.empty() && interPlein){
                cout << "Je break le for" << endl;
                break;
            }

            indexLigne = Dans_lignes(lignesDirectes[i], lignes);  // Cherche la ligne

            if (indexLigne != -1) {
                tempoLigne = (*lignes)[indexLigne].idLigne;
                tempoArret = (*lignes)[indexLigne].getSuivant(dernierTraite.arretId); // Trouve l'arrêt suivant

                // Si c'est un terminus, on passe au voisin suivant
                if (tempoArret == "FIN") continue;

                // Si l'arrêt est déjà visité, on passe au suivant
                if (Deja_visite(tempoArret, &arretsVisites) != -1) continue;

                if (tempoLigne == dernierTraite.ligneId) {
                    // Si c'est la même ligne, on préfère cette option
                    indexArret = (*lignes)[indexLigne].getIndArret(tempoArret);
                    indexHeure = (*lignes)[indexLigne].getHorairesPrecis(dernierTraite.heure, indexArret);

                    if (indexHeure != -1) {
                        h = (*lignes)[indexLigne].horaires[indexArret][indexHeure];
                        tempo = Noeud(tempoArret, tempoLigne, indActuel, h);
                        arretsVoisins.push_back(tempo);
                    }
                    break;
                }

                // Vérifie si l'arrêt suivant n'est pas déjà dans la liste des voisins
                if (tempoArret != "-1") {
                    // Si on peut continuer sur la même ligne, on privilégie cette option
                    if (tempoLigne == dernierTraite.ligneId) {
                        indexArret = (*lignes)[indexLigne].getIndArret(tempoArret);
                        indexHeure = (*lignes)[indexLigne].getHorairesPrecis(dernierTraite.heure, indexArret);

                        if (indexHeure != -1) {
                            h = (*lignes)[indexLigne].horaires[indexArret][indexHeure];
                            // On rajoute ce voisin à la liste s'il est plus optimal
                            tempo = Noeud(tempoArret, tempoLigne, indActuel, h);
                            arretsVoisins.push_back(tempo);
                        }
                    } else {
                        // Si le voisin est dans une autre ligne, on peut ajouter, mais ça doit être traité avec attention
                        // Par exemple, on pourrait vérifier si ce changement de ligne est nécessaire pour une meilleure optimisation
                        // (par exemple, si la ligne change mais nous rapproche de l'arrivée plus rapidement).
                        indexArret = (*lignes)[indexLigne].getIndArret(tempoArret);
                        indexHeure = (*lignes)[indexLigne].getHorairesPrecis(dernierTraite.heure, indexArret);

                        if (indexHeure != -1) {
                            h = (*lignes)[indexLigne].horaires[indexArret][indexHeure];
                            tempo = Noeud(tempoArret, tempoLigne, indActuel, h);
                            arretsVoisins.push_back(tempo);
                        }
                    }
                }
            }
        }

        if(!arretsVisites.empty() && interPlein){
            indexLigne = Dans_lignes(arretsVisites[0].ligneId, lignes);

            tempoLigne = (*lignes)[indexLigne].idLigne;
            tempoArret = (*lignes)[indexLigne].getSuivant(dernierTraite.arretId);

            indexArret = (*lignes)[indexLigne].getIndArret(tempoArret);
            // cout << "indexArret = " << indexArret << endl;
            indexHeure = (*lignes)[indexLigne].getHorairesPrecis(dernierTraite.heure, indexArret);
            // cout << "indexHeure = " << indexHeure << endl;

            if (indexHeure != -1) {
                h = (*lignes)[indexLigne].horaires[indexArret][indexHeure];
                tempo = Noeud(tempoArret, tempoLigne, indActuel, h);
                arretsVoisins.push_back(tempo);
            }
        }

        // on cherche le voisin avec la plus petite heure de passage (normalement le premier de la liste)

        // cout << "nb de voisin :" << arretsVoisins.size() << endl;
        // cout << endl;
        if(arretsVoisins.empty()){ // si il n'y a pas de voisins
            cout << "Il n'y a pas de voisins" << endl;
            return {};
        }

        // on cherche le voisin avec la plus petite heure de passage

        tempo = arretsVoisins[0]; // on initialise le meilleur voisin avec le premier voisin
        for(j = 0; j < arretsVoisins.size(); j++){ // pour chaque voisin
            if(arretsVoisins[j].ligneId == dernierTraite.ligneId){
                tempo = arretsVoisins[j];
                // cout << "On reste dans la ligne :";
                // tempo.print();
                break;
            }
            if(arretsVoisins[j].heure < tempo.heure){ // si l'heure de passage est plus petite que l'heure du meilleur voisin
                tempo = arretsVoisins[j]; // on met a jour le meilleur voisin
            }
        }
        // si le meilleur voisin a une heure plus grande que heureMax renvoyer une erreur
        if(tempo.heure > temps_max){ // si l'heure de passage est plus grande que l'heure max
            cout << "L'heure de passage est plus grande que l'heure max" << endl;
            Noeud erreur("Erreur", "Erreur", -1, temps_max); // on cree un noeud d'erreur
        }

        // on supprime le meilleur voisin de la liste des voisins
        indexVoisin = Dans_voisins(tempo.arretId, tempo.ligneId, &arretsVoisins); // on cherche si le meilleur voisin est dans la liste des voisins
        if(indexVoisin != -1){ // si le meilleur voisin est dans la liste des voisins
            arretsVoisins.erase(arretsVoisins.begin() + indexVoisin); // on supprime le meilleur voisin de la liste des voisins
        }

        if(dernierTraite.precedent == -1){
            dernierTraite.ligneId = tempo.ligneId;
        }
        arretsVisites.push_back(dernierTraite); // on ajoute le dernier aux arrets visites
        if(arretsVisites.begin()->ligneId != dernierTraite.ligneId){
            cout << "Je vais pop_back ça : ";
            (arretsVisites.end()-1)->print();
            arretsVisites.pop_back();
            cout << "J'ai pop_back ça : ";
            dernierTraite.print();
            cout << "Le dernier c'est ça : ";
            (arretsVisites.end()-1)->print();
            dernierTraite.arretId = (arretsVisites.end()-1)->arretId;
            dernierTraite.ligneId = (arretsVisites.end()-1)->ligneId;
            dernierTraite.precedent = (arretsVisites.end()-1)->precedent;
            dernierTraite.heure = (arretsVisites.end()-1)->heure;
            return construireChemin(arretsVisites, dernierTraite);
        }

        dernierTraite = tempo; // on met a jour le dernier noeud traite
        lignesDirectes = (*stops)[tempo.arretId].getLignes();
        indActuel++ ;

        // cout << "on retourne en haut ^^" << endl;
        arretsVoisins.clear();
    }

    // cout << "L'algo a trouver un chemin ^^ " << endl;
    return construireChemin(arretsVisites, dernierTraite);
}

vector<Noeud> construireChemin(vector<Noeud> arretsVisites, Noeud fin){
    vector<Noeud> chemin;
    Noeud current = fin;
    while (current.precedent != -1) {
        chemin.insert(chemin.begin(), current); // Ajouter le nœud au début du chemin
        current = arretsVisites[current.precedent];
    }
    chemin.insert(chemin.begin(), arretsVisites[0]); // Ajouter le nœud de départ
    return chemin;
}