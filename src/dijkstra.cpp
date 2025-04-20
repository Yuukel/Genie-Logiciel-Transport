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

    Noeud dernierTraite(depart,"Depart",nullptr,heure); // dernier noeud traite
    arretsVisites.push_back(dernierTraite); // on ajoute le noeud de depart au arrets visites

    if(depart == arrivee){ // si le depart est le meme que l'arrivee
        cout << "Le depart est le meme que l'arrivee" << endl;
        return dernierTraite; // on retourne le noeud de depart
    }

    vector<ligne> lignesDirect; // les lignes qui passent par cette arret
    lignesDirect = stops[depart].getLignes(); // on recupere les lignes qui passent par l'arret de depart

    // Variable utile dans le while
    Horaire h;
    string tempoLigne;
    string tempoArret;
    noeud tempo ;
    int indexLigne ;
    int indexHeure;
    int indexArret;
    while(dernierTraite.arretId != arrivee){ // tant que le noeud de depart n'est pas le meme que le noeud d'arrivee
        // on cherche les voisins du dernier noeud traite
        

        //ajout des voisins directs du dernier noeud traite
        for(int i = 0; i < lignesDirect.size(); i++){
            indexLigne = Dans_lignes(lignesDirect[i], lignes); // on cherche la ligne dans les lignes
            if(index != -1){ // si la ligne existe
                tempoLigne = lignes[indexLigne].idLigne; // on recupere l'id de la ligne
                tempoArret = lignes[indexLigne].getSuivant(dernierTraite.arretId); // on recupere l'arret suivant de la ligne
                if(tempoArret != "-1"){ // si l'arret suivant n'est pas le dernier arret de la ligne
                    indexHeure = lignes[index].getHorairesPrecis(dernierTraite.heure, dernierTraite.arretId); // on recupere l'heure de passage a cet arret
                    if(indexHeure == -1){ // si l'heure de passage n'est pas trouvee
                        cout << "L'heure de passage n'est pas trouvee" << endl;
                    }
                    else{ // si l'heure de passage est trouvee
                        h = lignes[index].horaires[indexHeure][]; // on recupere l'heure de passage a cet arret
                    }
                    tempo = Noeud(idArret, idLigne, &dernierTraite, h); // on cree un nouveau noeud avec l'arret suivant et la ligne
                    arretsVoisin.push_back(tempo); // on ajoute le noeud a la liste des voisins
                }
            }
        }
        vector<Noeud> voisins = dernierTraite.getVoisins(); // on recupere les voisins du dernier noeud traite


        for(int i = 0; i < voisins.size(); i++){ // pour chaque voisin
            if(find(arretsVisites.begin(), arretsVisites.end(), voisins[i].arretId) == arretsVisites.end()){ // si le voisin n'a pas deja ete visite
                arretsVoisin.push_back(voisins[i]); // on l'ajoute a la liste des voisins
            }
        }

        if(arretsVoisin.size() == 0){ // si il n'y a pas de voisins
            cout << "Il n'y a pas de voisins" << endl;
            return dernierTraite; // on retourne le dernier noeud traite
        }

        // on cherche le voisin avec la plus petite heure de passage
        Noeud meilleurVoisin = arretsVoisin[0]; // on initialise le meilleur voisin avec le premier voisin

        for(int i = 1; i < arretsVoisin.size(); i++){ // pour chaque voisin
            if(arretsVoisin[i].heure < meilleurVoisin.heure){ // si l'heure de passage est plus petite que l'heure du meilleur voisin
                meilleurVoisin = arretsVoisin[i]; // on met a jour le meilleur voisin
            }
        }

        chemin.push_back(meilleurVoisin); // on ajoute le meilleur voisin au chemin
        arretsVisites.push_back(meilleurVoisin.arretId); // on ajoute le meilleur voisin aux arrets visites

        dernierTraite = meilleurVoisin; // on met a jour le dernier noeud traite

    }
}

