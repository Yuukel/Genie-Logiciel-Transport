#include "arret.hpp"


Arret::Arret(string id, string nom){
    stopId = id;
    stopName = nom;
}

Arret::Arret(){
    stopId = "0";
    stopName = "Aucun nom";
}

void Arret::print() const{
    cout << stopName << endl;
    cout << stopId << endl;
}

void Arret::printLignes() const{
    cout << "Lignes qui passent par cet arret" << endl;
    for (int i = 0; i < lignes.size(); i++){
        cout << lignes[i] << endl;
    }
}

void Arret::addLigne(string newLigne){// ajoute une id a la liste
    lignes.push_back(newLigne);
}

vector<string> Arret::getLignes() const{
    return lignes;
}
