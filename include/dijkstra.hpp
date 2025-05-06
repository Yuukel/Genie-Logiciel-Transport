#ifndef DIJKTRA
#define DIJKTRA
#include "trajet.hpp"
#include "arret.hpp"
#include <unordered_map>
#include <string>
#include <deque>
#include <queue>
#include <algorithm>
#include <iterator>

using namespace std;

struct Noeud {
    string arretId; // id de l'arrêt
    string ligneId; // par quelle ligne on passe
    int precedent; // indice du nœud précédent dans le chemin le plus court
    Horaire heure; // heure de passage à cet arrêt

    // Constructeur par défaut
    Noeud() : arretId("Vide"), ligneId(""), precedent(-1), heure({0, 0}) {};

    // Nouveau constructeur avec l'arg horaire
    Noeud(string id, string ligne, int prec, Horaire h) : arretId(id), ligneId(ligne), precedent(prec), heure(h) {}


    void operator=(const Noeud& other){
        arretId = other.arretId;
        ligneId = other.ligneId;
        precedent = other.precedent;
        heure = other.heure;
    }

    void print(){
        cout << "Arret: " << arretId << ", Ligne: " << ligneId << ", Heure: " << setfill('0') << setw(2) << heure.heure << ":" << setfill('0') << setw(2) << heure.minute << endl;
    }
};

int Deja_visite(string id, vector<Noeud>* arretsVisites);
int Dans_voisins(string id, string ligne, vector<Noeud>* arretsVoisin);
int Dans_lignes(string id, vector<Ligne>* arretsLignes);

vector<vector<Noeud>> Dijkstra(string depart, string arrivee, Horaire heureDepart, unordered_map<string, Arret>* stops, vector<Ligne>* lignes, vector<string> arretsEviter);
vector<Noeud> DijkstraAlgo(string depart, string arrivee, Horaire heure, unordered_map<string, Arret>* stops, vector<Ligne>* lignes, vector<string> arretsEviter);
vector<Noeud> construireChemin(vector<Noeud> arretsVisites, Noeud fin);

#endif