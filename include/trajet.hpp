#ifndef TRAJET
#define TRAJET

#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <iostream>

using namespace std;

struct Horaire {
    unsigned int heure;
    unsigned int minute;

    // Surcharge de l'opérateur ==
    bool operator==(const Horaire& other) const {
        return heure == other.heure && minute == other.minute;
    }

    // Surcharge de l'opérateur <
    bool operator<(const Horaire& other) const {
        return (heure < other.heure) || (heure == other.heure && minute < other.minute);
    }

    // Surcharge de l'opérateur >
    bool operator>(const Horaire& other) const {
        return (heure > other.heure) || (heure == other.heure && minute > other.minute);
    }
 
    void operator=(const Horaire& other) {
        heure = other.heure;
minute = other.minute;
        }
} typedef Horaire;

// Copie Dijkstra.hpp (à retirer si on test Dijkstra)
struct Noeud {
    string arretId; // id de l'arrêt
    string ligneId; // par quelle ligne on passe
    Noeud* precedent; // nœud précédent dans le chemin le plus court
    Horaire heure; // heure de passage à cet arrêt

    // Constructeur par défaut
  Noeud() : arretId("Vide"), ligneId(""), precedent(nullptr), heure({0, 0}) {};

    // Nouveau constructeur avec l'arg horaire
    Noeud(string id, string ligne, Noeud* prec, Horaire h)
        : arretId(id), ligneId(ligne), precedent(prec), heure(h) {}


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

class Ligne {
    public:
    string idLigne;
    string nomLigne;
    string tripHeadsign;
    vector<string> stopIds; // IDs des arrêts
    vector<vector<Horaire>> horaires;

    Ligne(string id, string nom, string headsign);
    Ligne();

    void print() const; // affiche les informations de la ligne
    void printArrets() const; // affiche les arrêts de la ligne
    void printHoraires() const; // affiche les horaires de la ligne    
    void addArret(string arret); // ajoute un arrêt à la liste des arrêts de la ligne
    void addHoraire(vector<Horaire> horaire); // ajoute un horaire à la liste des horaires de la ligne

    vector<string> getArrets(); // liste des arrêts de la ligne
    vector<vector<Horaire>> getHoraires() const; // liste des horaires de la ligne
    int getHorairesPrecis(Horaire ,int) const; // donne l'indice de l'horaire qui passe après l'horaire donné à l'arrêt donné
    string getSuivant(string arret) const; // retourne l'arrêt suivant de la ligne
    int getIndArret(string arret) const; // retourne l'indice de l'arrêt
    Horaire getDernierHoraire(int arret) const; // renvoie l'horaire du dernier passage à l'arrêt donné
 // retourne l'indice de l'arrêt
};

#endif