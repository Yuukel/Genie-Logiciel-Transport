#ifndef ARRET
#define ARRET

#include "trajet.hpp"
#include <iostream>
#include <cstdio>
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

        Arret(string, string); // constructeur
        Arret(); // constructeur par défaut

        void print() const; // affiche les informations de l'arret
        void printLignes() const; // affiche les lignes qui passent par cet arret
        vector<string> getLignes() const; // liste des lignes qui passent par cet arret
        void addLigne(string); // ajoute une ligne à la liste des lignes qui passent par cet arret
};

#endif