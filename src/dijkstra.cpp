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
    vector<string> lignesDirect;

    while(chemin.empty() || chemin.end()->arretId != arrivee){
        heuresDepart.clear();
        lignesDirect.clear();

        if(!chemin.empty()){
            depart = (chemin.end()-1)->arretId;
            heure = (chemin.end()-1)->heure;
        }
        lignesDirect = (*stops)[depart].getLignes();
        for(int i = 0 ; i < lignesDirect.size() ; i++){
            int indexLigne = Dans_lignes(lignesDirect[i], lignes);
            int indexArret = (*lignes)[indexLigne].getIndArret(depart);
            int id = (*lignes)[indexLigne].getHorairesPrecis(heure, indexArret);
            if(id != -1){
                Horaire h = (*lignes)[indexLigne].horaires[indexArret][id];
                heuresDepart.emplace(lignesDirect[i], h);
            } else{
                lignesDirect.erase(lignesDirect.begin()+i);
                i--;
            }
        }

        Horaire hmin = heuresDepart[lignesDirect[0]];
        for(int i = 1 ; i < lignesDirect.size() ; i++){
            if(heuresDepart[lignesDirect[i]] < hmin){
                hmin = heuresDepart[lignesDirect[i]];
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

    vector<string> lignesDirect;

    Noeud dernierTraite(depart,"Depart",-1,heure); // dernier noeud traite
    dernierTraite.print();

    if(depart == arrivee){ // si le depart est le meme que l'arrivee
        cout << "Le depart est le meme que l'arrivee" << endl;
        return {dernierTraite}; // on retourne le noeud de depart
    }

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
    lignesDirect = (*stops)[arrivee].getLignes(); // on récupère les lignes qui passent par l'arrêt d'arrivée

    for (int i = 0; i < lignesDirect.size(); i++) {
        // cout << "on entre dans le for (du max)" << endl;
        indexLigne = Dans_lignes(lignesDirect[i], lignes); // on passe l'ID de la ligne
        if (indexLigne != -1) { // si la ligne existe
            indexArret = (*lignes)[indexLigne].getIndArret(arrivee); // on récupère l'indice de l'arrêt
            h = (*lignes)[indexLigne].getDernierHoraire(indexArret); // on récupère l'heure de passage à cet arrêt
            if (temps_max < h) temps_max = h; // on compare les heures de passage
        }
    }
    // cout << "temps_max : " << temps_max.heure << ":" << temps_max.minute << endl; // on affiche l'heure max

    lignesDirect = (*stops)[depart].getLignes();

    while(dernierTraite.arretId != arrivee){ // tant que le noeud de depart n'est pas le meme que le noeud d'arrivee
        // (*stops)[dernierTraite.arretId].print();
        // (*stops)[dernierTraite.arretId].printLignes();
        // on cherche les voisins du dernier noeud traite

        //ajout des voisins directs du dernier noeud traite
        for (int i = 0; i < lignesDirect.size(); i++) {
            indexLigne = Dans_lignes(lignesDirect[i], lignes);  // Cherche la ligne
        
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

        // on cherche le voisin avec la plus petite heure de passage (normalement le premier de la liste)

        // cout << "nb de voisin :" << arretsVoisins.size() << endl;
        // cout << endl;
        if(arretsVoisins.size() == 0){ // si il n'y a pas de voisins
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
        if(arretsVisites.begin()->ligneId != dernierTraite.ligneId) return construireChemin(arretsVisites, dernierTraite);

        dernierTraite = tempo; // on met a jour le dernier noeud traite
        lignesDirect = (*stops)[tempo.arretId].getLignes();
        indActuel++ ;

        // cout << "on retourne en haut ^^" << endl;
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