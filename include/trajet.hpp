#ifndef TRAJET
#define TRAJET

#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <iostream>

using namespace std;

struct Horaire {
    unsigned int heure = 0;
    unsigned int minute = 0;

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

    void print() const{
        cout << heure << ":" << minute << endl;
    }

    void ajouterMinutes(int nb){
        minute += nb;
        while(minute >= 60){
            minute -= 60;
            heure++;
        }
        while(heure >= 24) heure -= 24;
    }
} typedef Horaire;

class Ligne {
    public:
        string idLigne;
        string nomLigne;
        string tripHeadsign;
        vector<string> stopIds; // IDs des arrêts
        vector<vector<Horaire>> horaires;

        Ligne(string, string, string);
        Ligne();

        void print() const; // affiche les informations de la ligne
        void printArrets() const; // affiche les arrêts de la ligne
        void printHoraires() const; // affiche les horaires de la ligne
        void addArret(string); // ajoute un arrêt à la liste des arrêts de la ligne
        void addHoraire(vector<Horaire>); // ajoute un horaire à la liste des horaires de la ligne

        vector<string> getArrets(); // liste des arrêts de la ligne
        vector<vector<Horaire>> getHoraires() const; // liste des horaires de la ligne
        int getHorairesPrecis(Horaire ,int) const; // donne l'indice de l'horaire qui passe après l'horaire donné à l'arrêt donné
        string getSuivant(string) const; // retourne l'arrêt suivant de la ligne
        int getIndArret(string) const; // retourne l'indice de l'arrêt
        Horaire getDernierHoraire(int) const; // renvoie l'horaire du dernier passage à l'arrêt donné
};

int trouveLigne(string, vector<Ligne>*); // retourne l'indice de la ligne dans le tableau des lignes

#endif