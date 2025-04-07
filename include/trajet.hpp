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

    void print(); // affiche les informations de la ligne
    void printArrets(); // affiche les arrêts de la ligne
    void printHoraires(); // affiche les horaires de la ligne    
    void addArret(string arret); // ajoute un arrêt à la liste des arrêts de la ligne
    void addHoraire(vector<Horaire> horaire); // ajoute un horaire à la liste des horaires de la ligne

    vector<string> getArrets(); // liste des arrêts de la ligne
    vector<vector<Horaire>> getHoraires(); // liste des horaires de la ligne
    int getHorairesPrecis(Horaire ,int); // donne l'indice de l'horaire qui passe après l'horaire donné à l'arrêt donné
    string getSuivant(string arret); // retourne l'arrêt suivant de la ligne
    int getIndArret(string arret); // retourne l'indice de l'arrêt
};

#endif