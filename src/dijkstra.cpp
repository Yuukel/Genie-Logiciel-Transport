#include "dijkstra.hpp"


using namespace std;

Noeud::Noeud(string id,string ligne, Noeud* prec, Horaire h){
    arretId = id;
    ligneId = ligne;
    precedent = prec; // precedent = nullptr si c'est le premier noeud
    heure = h; // heure de passage a cet arret
}

int Deja_visite(string id, vector<string>* arretsVisites){
    // on verifie si l'arret est deja dans la liste des arrets visites
    for(int i = 0; i < arretsVisites.size(); i++){
        if(arretsVisites[i] == id){
            return i; // l'arret est deja dans la liste et retourne son index
        }
    }
    return -1; // l'arret n'est pas dans la liste
}

int Dans_voisins(string id, vector<Noeud>* arretsVoisin){
    // on verifie si l'arret est deja dans la liste des arrets visites
    for(int i = 0; i < arretsVoisin.size(); i++){
        if(arretsVoisin[i].arretId == id){
            return i; // l'arret est deja dans la liste et retourne son index
        }
    }
    return -1; // l'arret n'est pas dans la liste
}

int Dans_lignes(string id, vector<Ligne>* arretsLignes){
    // on verifie si l'arret est deja dans la liste des arrets visites
    for(int i = 0; i < arretsLignes.size(); i++){
        if(arretsLignes[i].idLigne == id){
            return i; // l'arret est deja dans la liste et retourne son index
        }
    }
    return -1; // l'arret n'est pas dans la liste
}

Noeud Dijktra(string depart, string arrivee, Horaire heure, unordered_map<std::string, Arret>* stops, vector<Ligne>* lignes,vector<Noeud>* arretsVoisin, vector<string>* arretsVisites ){
    // Dijkstra's algorithm to find the shortest path between two stops
    // depart : id de l'arret de depart
    // arrivee : id de l'arret d'arrivee
    // heure : heure de depart
    // retourne le chemin le plus court entre les deux arrets

    //on vide arretsVoisin et arretsVisites avant de tout commencer
    arretsVisites.clear(); // on vide la liste
    arretsVoisin.clear(); // on vide la liste

    Noeud dernierTraite(depart,"Depart",nullptr,heure); // dernier noeud traite
    arretsVisites.push_back(dernierTraite); // on ajoute le noeud de depart au arrets visites

    if(depart == arrivee){ // si le depart est le meme que l'arrivee
        cout << "Le depart est le meme que l'arrivee" << endl;
        return dernierTraite; // on retourne le noeud de depart
    }

    // Variable utile dans le while
    
    //utile pour le premier for
    Horaire h;                                                  //   <-- ici ce trouve la ligne 69 colonne 69
    string tempoLigne;
    string tempoArret;
    noeud tempo ;
    int indexLigne ;
    int indexHeure;
    int indexArret;
    int indexVoisin;
    int j;

    Horaire temps_max;// correspond a la derniere heure de passage a l'arret final
    temps_max.heure = 0; // on initialise l'heure a 0
    temps_max.minute = 0; // on initialise les minutes a 0
    // definition du temps max
    vector<ligne> lignesDirect; // les lignes qui passent par cette arret
    lignesDirect = stops[arrivee].getLignes(); // on recupere les lignes qui passent par l'arret de depart
    for(int i = 0; i < lignesDirect.size(); i++){
        indexLigne = Dans_lignes(lignesDirect[i], lignes); // on cherche la ligne dans les lignes
        if(indexLigne != -1){ // si la ligne existe
            indexArret = lignes[indexLigne].getArret(arrivee); // on recupere l'indice de l'arret
            h = lignes[indexLigne].getDernierHoraire(indexArret); // on recupere l'heure de passage a cet arret
            if(temps_max < h) temps_max = h; // on compare les heures de passage
        }
    }

    while(dernierTraite.arretId != arrivee){ // tant que le noeud de depart n'est pas le meme que le noeud d'arrivee
        // on cherche les voisins du dernier noeud traite
        

        //ajout des voisins directs du dernier noeud traite
        for(int i = 0; i < lignesDirect.size(); i++){
            indexLigne = Dans_lignes(lignesDirect[i], lignes); // on cherche la ligne dans les lignes
            if(index != -1){ // si la ligne existe
                tempoLigne = lignes[indexLigne].idLigne; // on recupere l'id de la ligne
                tempoArret = lignes[indexLigne].getSuivant(dernierTraite.arretId); // on recupere l'arret suivant de la ligne
                if(tempoArret == "FIN") continue; // si l'arret suivant est le dernier arret de la ligne, on passe au voisin suivant
                if(Deja_visite(tempoArret, arretsVisites) != -1) continue; // si l'arret est deja visite, on passe au voisin suivant
                if(tempoArret != "-1"){ // si l'arret suivant n'est pas le dernier arret de la ligne
                    indexArret = lignes[indexLigne].getArret(tempoArret); // on recupere l'indice de l'arret
                    indexHeure = lignes[indexLigne].getHorairesPrecis(dernierTraite.heure, indexArret); // on recupere l'heure de passage a cet arret
                    if(indexHeure == -1){ // si l'heure de passage n'est pas trouvee/c'est trop tard
                        cout << "L'heure de passage n'est pas trouvee" << endl;
                    }
                    else{ // si l'heure de passage est trouvee
                        h = lignes[indexLigne].horaires[indexArret][indexHeure]; // on recupere l'heure de passage a cet arret
                        indexVoisin = Dans_voisins(tempoArret, arretsVoisin); // on cherche si l'arret est deja dans la liste des voisins
                        if(indexVoisin != -1){ // si l'arret est dans la liste des voisins 
                            // on compare les heures de passage
                            if(arretsVoisins[indexVoisin].heure < h) continue; // si l'heure de passage est plus grande que l'heure du voisin, on passe au voisin suivant
                            arretsVoisins.erase(arretsVoisins.begin() + indexVoisin); // on supprime le voisin de la liste des voisins
                            cout << "L'arret est supprime de la liste des voisins" << endl;
                        }
                        tempo = Noeud(idArret, idLigne, &dernierTraite, h); // on cree un nouveau noeud avec l'arret suivant et la ligne
                        arretsVoisin.push_back(tempo); // on ajoute le noeud a la liste des voisins
                        
                    }
                }
            }
        }

        // on cherche le voisin avec la plus petite heure de passage (normalement le premier de la liste)

        if(arretsVoisin.size() == 0){ // si il n'y a pas de voisins
            cout << "Il n'y a pas de voisins" << endl;
            return dernierTraite; // on retourne le dernier noeud traite
        }

        // on ajoute meilleur voisin dans la liste des arrets visites
        arretsVisites.push_back(dernierTraite.arretId); // on ajoute le dernier noeud traite a la liste des arrets visites

        // on cherche le voisin avec la plus petite heure de passage

        tempo = arretsVoisin[0]; // on initialise le meilleur voisin avec le premier voisin
        for(j = 1; j < arretsVoisin.size(); j++){ // pour chaque voisin
            if(arretsVoisin[j].heure < tempo.heure){ // si l'heure de passage est plus petite que l'heure du meilleur voisin
                tempo = arretsVoisin[j]; // on met a jour le meilleur voisin
            }
        }
        // si le meilleur voisin a une heure plus grande que heureMax renvoyer une erreur
        if(tempo.heure > temps_max){ // si l'heure de passage est plus grande que l'heure max
            cout << "L'heure de passage est plus grande que l'heure max" << endl;
            noeud erreur("Erreur", "Erreur", nullptr, temps_max); // on cree un noeud d'erreur
            return erreur; // on retourne le noeud d'erreur
        }

        // on supprime le meilleur voisin de la liste des voisins
        indexVoisin = Dans_voisins(meilleurVoisin.arretId, arretsVoisin); // on cherche si le meilleur voisin est dans la liste des voisins
        if(indexVoisin != -1){ // si le meilleur voisin est dans la liste des voisins
            arretsVoisin.erase(arretsVoisin.begin() + indexVoisin); // on supprime le meilleur voisin de la liste des voisins
        }

        arretsVisites.push_back(meilleurVoisin.arretId); // on ajoute le meilleur voisin aux arrets visites

        dernierTraite = tempo; // on met a jour le dernier noeud traite

    }

    cout << "L'algo a trouver un chemin ^^ " << endl;
    return dernierTraite; // on retourne le dernier noeud traite
}

