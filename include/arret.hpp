#ifndef ARRET
#define ARRET

#include "trajet.hpp" // Inclure le fichier d'en-tête de la classe trajet
#include <iostream> // Pour std::cout et std::endl
#include <cstdio> // Pour printf
#include <string>
#include <vector>
#include <string>

using namespace std;

// Arret d'une ligne de transport en commun
class Arret {
    public:
    string stopName; // nom de l'arret
    string stopId;   // unique à chaque arret
    vector<string> lignes; // liste des IDs des lignes qui passent par cet arret

    Arret(string id, string nom); // constructeur
    Arret(); // constructeur par défaut
    
    void print(); // affiche les informations de l'arret
    void printLignes(); // affiche les lignes qui passent par cet arret
    vector<string> getLignes(); // liste des lignes qui passent par cet arret
    void addLigne(string ligne); // ajoute une ligne à la liste des lignes qui passent par cet arret 
};

#endif