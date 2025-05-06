#include "trajet.hpp"
#include "arret.hpp"
#include <iostream>

Ligne::Ligne(string id, string nom, string headsign) {
    idLigne = id;
    nomLigne = nom;
    tripHeadsign = headsign;
}

Ligne::Ligne() {
    idLigne = "0";
    nomLigne = "Aucun nom";
    tripHeadsign = "Aucun terminus";
}

void Ligne::print() const{
    cout << nomLigne << endl;
    cout << idLigne << endl;
    cout << tripHeadsign << endl;
    cout << "Terminus : " << tripHeadsign << endl;
}

void Ligne::printArrets() const{
    cout << "Nombre d'arrêts : " << stopIds.size() << endl;
    cout << "Arrêts de la ligne :" << endl;
    for (int i = 0; i < stopIds.size(); i++) {
        cout << stopIds[i] << endl;
        for(int j = 0 ; j < horaires[i].size() ; j++){
            cout << setfill('0') << setw(2) << horaires[i][j].heure << ":"
            << setfill('0') << setw(2) << horaires[i][j].minute << " ";
        }
        cout << endl;
    }
}

void Ligne::printHoraires() const{
    cout << horaires.size() << endl;
    cout << "Horaires de la ligne :" << endl;
    for (int i = 0; i < horaires.size(); i++) {
        for(int j = 0 ; j < horaires[i].size() ; j++){
            cout << setfill('0') << setw(2) << horaires[i][j].heure << ":"
            << setfill('0') << setw(2) << horaires[i][j].minute << " ";
        }
        cout << endl;
    }
}

void Ligne::addArret(string arret) {
    stopIds.push_back(arret);
}

void Ligne::addHoraire(vector<Horaire> horaire) {
    horaires.push_back(horaire);
}

vector<string> Ligne::getArrets() {
    return stopIds;
}

vector<vector<Horaire>> Ligne::getHoraires() const{
    return horaires;
}

int Ligne::getHorairesPrecis(Horaire h, int arret) const{
    for (int i = 0; i < horaires[arret].size(); i++) {
        if ((horaires[arret][i].heure == h.heure && horaires[arret][i].minute >= h.minute) || horaires[arret][i].heure > h.heure) {
            return i; // Retourne l'indice de l'horaire correspondant
        }
    }
    return -1; // Si aucun horaire ne correspond, retourne -1
}

Horaire Ligne::getDernierHoraire(int arret) const{
    if (arret >= 0 && arret < horaires.size()) {
        return horaires[arret].back(); // Retourne le dernier horaire de l'arrêt
    } else {
        return {0, 0}; // Si l'arrêt n'est pas valide, retourne un horaire par défaut
    }
}

string Ligne::getSuivant(string arret) const{
    for (int i = 0; i < stopIds.size(); i++) {
        if (stopIds[i] == arret) {
            if (i + 1 < stopIds.size()) {
                return stopIds[i + 1]; // Retourne l'arrêt suivant
            } else {
                return "FIN"; // Si c'est le dernier arrêt, renvoie FIN
            }
        }
    }
    return "-1"; // Si l'arrêt n'est pas trouvé, retourne -1
}

int Ligne::getIndArret(string arret) const{
    for (int i = 0; i < stopIds.size(); i++) {
        if (stopIds[i] == arret) {
            return i; // Retourne l'arrêt correspondant
        }
    }
    return -1; // Si l'arrêt n'est pas trouvé, retourne nullptr
}

int trouveLigne(string id, vector<Ligne>* lignes){
    for (int i = 0; i < lignes->size(); i++) {
        if ((*lignes)[i].idLigne == id) {
            return i; // Retourne l'indice de la ligne trouvée
        }
    }
    return -1; // Si la ligne n'est pas trouvée, retourne -1
}