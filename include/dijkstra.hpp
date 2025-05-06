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

int Deja_visite(string, vector<Noeud>*);
int Dans_voisins(string, string, vector<Noeud>*);
int Dans_lignes(string, vector<Ligne>*);

vector<vector<Noeud>> Dijkstra(string, string, Horaire, unordered_map<string, Arret>*, vector<Ligne>*, vector<string>);
vector<Noeud> DijkstraAlgo(string, string, Horaire, unordered_map<string, Arret>*, vector<Ligne>*, vector<string>);
vector<Noeud> construireChemin(vector<Noeud>, Noeud);

#endif