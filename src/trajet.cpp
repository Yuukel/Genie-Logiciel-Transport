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
    printf("\nLigne : %s\n", nomLigne.c_str());
    printf("ID : %s\n", idLigne.c_str());
    printf("Terminus : %s\n", tripHeadsign.c_str());
}

void Ligne::printArrets() const{
    printf("Nombres d'arrêts: %ld\n", stopIds.size());
    printf("Arrets de la ligne :\n");
    for (int i = 0; i < stopIds.size(); i++) {
        printf("%s\n", stopIds[i].c_str());
        for(int j = 0 ; j < horaires[i].size() ; j++){
            printf("%02d:%02d ", horaires[i][j].heure, horaires[i][j].minute);
        }
        printf("\n");
    }
}

void Ligne::printHoraires() const{
    printf("Nombres d'horaires: %ld\n", horaires.size());
    printf("Horaires de la ligne :\n");
    for (int i = 0; i < horaires.size(); i++) {
        printf("%02d:%02d\n", horaires[i][0].heure, horaires[i][0].minute);
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
    for (int i = 0; i < horaires.size(); i++) {
        if (horaires[i][arret].heure >= h.heure && horaires[i][arret].minute >= h.minute) {
            return i; // Retourne l'indice de l'horaire correspondant
        }
    }
    return -1; // Si aucun horaire ne correspond, retourne -1
}

string Ligne::getSuivant(string arret) const{
    for (int i = 0; i < stopIds.size(); i++) {
        if (stopIds[i] == arret) {
            if (i + 1 < stopIds.size()) {
                return stopIds[i + 1]; // Retourne l'arrêt suivant
            } else {
                return "-1"; // Si c'est le dernier arrêt, retourne nullptr
            }
        }
    }
    return "FIN"; // Si l'arrêt n'est pas trouvé, retourne nullptr
}

int Ligne::getIndArret(string arret) const{
    for (int i = 0; i < stopIds.size(); i++) {
        if (stopIds[i] == arret) {
            return i; // Retourne l'arrêt correspondant
        }
    }
    return -1; // Si l'arrêt n'est pas trouvé, retourne nullptr
}




