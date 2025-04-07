#include "arret.hpp"


Arret::arret(string id, string nom){
    stopId = id;
    stopName = nom;
}

Arret::arret(){
    stopId = "0";
    stopName = "Aucun nom";
}

void Arret::print(){
    printf("\nArret : %s\n", stopName.c_str());
    printf("ID : %s\n", stopId.c_str());
}

void Arret::printLignes(){
    printf("Lignes qui passent par cet arret :\n");
    for (int i = 0; i < lignes.size(); i++){
        printf("%s\n", lignes[i].c_str());
    }
}

void Arret::addLigne(string newLigne){// ajoute une id a la liste
    lignes.push_back(newLigne);
}

vector<string> Arret::getLignes(){
    return lignes;
}
