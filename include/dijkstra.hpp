#ifndef DIJKTRA
#define DIJKTRA
#include "trajet.hpp"
#include "arret.hpp"
#include <unordered_map>
#include <string>
#include <algorithm>


using namespace std;
/*
struct Noeud{
    string arretId; // id de l'arret
    string ligneId; // par quelle ligne on passe
    Noeud* precedent; // noeud precedent dans le chemin le plus court
    Horaire heure; // heure de passage a cet arret

    Noeud(string id, string name, string ligne,Horaire h); // constructeur
}typedef Noeud;
*/
int Deja_visite(const string& arretId, const vector<Noeud>* arretsVisites);
// int Dans_voisins(string id, vector<Noeud>* arretsVoisin);
int Dans_lignes(const string& ligneId, const vector<Ligne>* lignes);

Noeud Dijktra(string depart, string arrivee, Horaire heure, unordered_map<string, Arret>* stops, vector<Ligne>* lignes, vector<Noeud>* arretsVoisins, vector<Noeud>* arretsVisites);

#endif