#ifndef TRAJET
#define TRAJET

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

struct Horaire {
    unsigned int heure;
    unsigned int minute;
} typedef Horaire;

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
};

#endif