#ifndef DIJKTRA
#define DIJKTRA
#include "trajet.hpp"
#include "arret.hpp"
#include <unordered_map>


using namespace std;

struct Noeud{
    string arretId; // id de l'arret
    string ligneId; // par quelle ligne on passe
    Noeud* precedent; // noeud precedent dans le chemin le plus court
    Horaire heure; // heure de passage a cet arret

    Noeud(string id, string name, string ligne); // constructeur
};

Noeud Dijktra(string depart, string arrivee, horaire heure, unordered_map<std::string, Arret>* stops, unordered_map<std::string, Arret>* lignes );

int Deja_visite(string id, vector<string>* arretsVisites);
int Dans_voisins(string id, vector<Noeud>* arretsVoisin);
int Dans_lignes(string id, vector<Ligne>* arretsLignes);
#endif