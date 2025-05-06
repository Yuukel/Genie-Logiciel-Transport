#include "dijkstra.hpp"

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

vector<vector<Noeud>> Dijkstra(string depart, string arrivee, Horaire heure, unordered_map<string, Arret>* stops, vector<Ligne>* lignes, vector<string> arretsEviter){
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
            heure.ajouterMinutes(1);
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

        chemin = DijkstraAlgo(depart, arrivee, hmin, stops, lignes, arretsEviter);
        if(chemin.empty()){
            break;
        }
        cheminFinal.push_back(chemin);
        if(cheminFinal.size() > 5) break;
    }
    if(!chemin.empty() && (chemin.end()-1)->arretId == arrivee)
        return cheminFinal;
    else
        return {};
}

vector<Noeud> DijkstraAlgo(string depart, string arrivee, Horaire heure, unordered_map<string, Arret>* stops, vector<Ligne>* lignes, vector<string> arretsEviter){
    vector<Noeud> arretsVisites;
    vector<Noeud> arretsVoisins;

    vector<string> lignesDirectes;
    vector<string> lignesDirectesDepart;
    vector<string> lignesDirectesArrivee;

    Noeud dernierTraite(depart,"Depart",-1,heure); // dernier noeud traite

    if(depart == arrivee){ // si le depart est le meme que l'arrivee
        cerr << "Le depart est le meme que l'arrivee" << endl;
        return {}; // on retourne le noeud de depart
    }

    lignesDirectesDepart = (*stops)[depart].getLignes();
    lignesDirectesArrivee = (*stops)[arrivee].getLignes();

    bool interPlein = true;
    sort(lignesDirectesDepart.begin(), lignesDirectesDepart.end());
    sort(lignesDirectesArrivee.begin(), lignesDirectesArrivee.end());
    set_intersection(lignesDirectesDepart.begin(), lignesDirectesDepart.end(),lignesDirectesArrivee.begin(), lignesDirectesArrivee.end(), back_inserter(lignesDirectes));

    // checker avec les arrêts à éviter
    for(int i = 0 ; i < lignesDirectes.size() ; i++){
        int indexLigne = Dans_lignes(lignesDirectes[i], lignes);
        int indexArretDepart = (*lignes)[indexLigne].getIndArret(depart);
        Horaire hDepart = (*lignes)[indexLigne].getDernierHoraire(indexArretDepart);
        int indexArretArrivee = (*lignes)[indexLigne].getIndArret(arrivee);
        Horaire hArrivee = (*lignes)[indexLigne].getDernierHoraire(indexArretArrivee);

        if(hDepart > hArrivee){
            lignesDirectes.erase(lignesDirectes.begin()+i);
            i--;
        } else{
            for(int j = 0 ; j < arretsEviter.size() ; j++){
                for(int k = 0 ; k < (*lignes)[indexLigne].stopIds.size() ; k++){
                    if((*lignes)[indexLigne].stopIds[k] == arretsEviter[j]){
                        lignesDirectes.erase(lignesDirectes.begin()+i);
                        i--;
                        j = arretsEviter.size();
                        k = (*lignes)[indexLigne].stopIds.size();
                    }
                }
            }
        }

    }

    if(lignesDirectes.empty()){
        interPlein = false;
    }

    // Variable utile dans le while
    int indActuel = 0; // indice de l'arret actuel dans la liste des arrets visites
    //utile pour le premier for
    Horaire h, temps_max;
    string tempoLigne, tempoArret;
    Noeud tempo ;
    int indexLigne, indexHeure, indexArret, indexVoisin, j;

    if(!interPlein)
        lignesDirectes = (*stops)[arrivee].getLignes(); // on récupère les lignes qui passent par l'arrêt d'arrivée

    for (int i = 0; i < lignesDirectes.size(); i++) {
        indexLigne = Dans_lignes(lignesDirectes[i], lignes); // on passe l'ID de la ligne
        if (indexLigne != -1) { // si la ligne existe
            indexArret = (*lignes)[indexLigne].getIndArret(arrivee); // on récupère l'indice de l'arrêt
            h = (*lignes)[indexLigne].getDernierHoraire(indexArret); // on récupère l'heure de passage à cet arrêt
            if (temps_max < h) temps_max = h; // on compare les heures de passage
        }
    }
    if(!interPlein)
        lignesDirectes = (*stops)[depart].getLignes();

    while(dernierTraite.arretId != arrivee){ // tant que le noeud de depart n'est pas le meme que le noeud d'arrivee
        // on cherche les voisins du dernier noeud traite
        //ajout des voisins directs du dernier noeud traite
        for (int i = 0; i < lignesDirectes.size() ; i++) {
            if(!arretsVisites.empty() && interPlein){
                break;
            }

            indexLigne = Dans_lignes(lignesDirectes[i], lignes);  // Cherche la ligne

            if (indexLigne != -1) {
                tempoLigne = (*lignes)[indexLigne].idLigne;
                tempoArret = (*lignes)[indexLigne].getSuivant(dernierTraite.arretId); // Trouve l'arrêt suivant

                for(int j = 0 ; j < arretsEviter.size() ; j++){
                    if(tempoArret == arretsEviter[i]) tempoArret = "FIN";
                }

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

        if(!arretsVisites.empty() && interPlein){
            indexLigne = Dans_lignes(arretsVisites[0].ligneId, lignes);

            tempoLigne = (*lignes)[indexLigne].idLigne;
            tempoArret = (*lignes)[indexLigne].getSuivant(dernierTraite.arretId);

            indexArret = (*lignes)[indexLigne].getIndArret(tempoArret);
            indexHeure = (*lignes)[indexLigne].getHorairesPrecis(dernierTraite.heure, indexArret);

            if (indexHeure != -1) {
                h = (*lignes)[indexLigne].horaires[indexArret][indexHeure];
                tempo = Noeud(tempoArret, tempoLigne, indActuel, h);
                arretsVoisins.push_back(tempo);
            }
        }

        // on cherche le voisin avec la plus petite heure de passage (normalement le premier de la liste)

        if(arretsVoisins.empty()){ // si il n'y a pas de voisins
            cout << "Il n'y a pas de voisins" << endl;
            return {};
        }

        // on cherche le voisin avec la plus petite heure de passage

        tempo = arretsVoisins[0]; // on initialise le meilleur voisin avec le premier voisin
        for(j = 0; j < arretsVoisins.size(); j++){ // pour chaque voisin
            if(arretsVoisins[j].ligneId == dernierTraite.ligneId){
                tempo = arretsVoisins[j];
                break;
            }
            if(arretsVoisins[j].heure < tempo.heure){ // si l'heure de passage est plus petite que l'heure du meilleur voisin
                tempo = arretsVoisins[j]; // on met a jour le meilleur voisin
            }
        }
        // si le meilleur voisin a une heure plus grande que heureMax renvoyer une erreur
        if(tempo.heure > temps_max){ // si l'heure de passage est plus grande que l'heure max
            cerr << "L'heure de passage est plus grande que l'heure max" << endl;
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
            arretsVisites.pop_back();
            dernierTraite.arretId = (arretsVisites.end()-1)->arretId;
            dernierTraite.ligneId = (arretsVisites.end()-1)->ligneId;
            dernierTraite.precedent = (arretsVisites.end()-1)->precedent;
            dernierTraite.heure = (arretsVisites.end()-1)->heure;
            return construireChemin(arretsVisites, dernierTraite);
        }

        dernierTraite = tempo; // on met a jour le dernier noeud traite
        lignesDirectes = (*stops)[tempo.arretId].getLignes();
        indActuel++ ;

        arretsVoisins.clear();
    }

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