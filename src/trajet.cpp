#include "trajet.hpp"
#include "arret.hpp"
#include <iostream>

ligne::ligne(std::string id, std::string nom, std::string headsign) {
    id_ligne = id;
    nom_ligne = nom;
    trip_headsign = headsign;
}

ligne::ligne() {
    id_ligne = "0";
    nom_ligne = "Aucun nom";
    trip_headsign = "Aucun terminus";
}

void ligne::print() {
    printf("\nLigne : %s\n", nom_ligne.c_str());
    printf("ID : %s\n", id_ligne.c_str());
    printf("Terminus : %s\n", trip_headsign.c_str());
}

void ligne::print_arrets() {
    printf("Arrets de la ligne :\n");
    for (int i = 0; i < stop_ids.size(); i++) {
        printf("%s\n", stop_ids[i].c_str());
    }
}

void ligne::print_horaires() {
    printf("Horaires de la ligne :\n");
    for (int i = 0; i < horaires.size(); i++) {
        printf("%02d:%02d\n", horaires[i][0].heure, horaires[i][0].minute);
    }
}

void ligne::add_arret(std::string arret) {
    stop_ids.push_back(arret);
}

void ligne::add_horaire(std::vector<horaire> horaire) {
    horaires.push_back(horaire);
}

std::vector<std::string> ligne::get_arrets() {
    return stop_ids;
}

std::vector<std::vector<horaire>> ligne::get_horaires() {
    return horaires;
}

int ligne::get_horaires_precis(horaire h, int arret) {
    for (int i = 0; i < horaires.size(); i++) {
        if (horaires[i][arret].heure >= h.heure && horaires[i][arret].minute >= h.minute) {
            return i; // Retourne l'indice de l'horaire correspondant
        }
    }
    return -1; // Si aucun horaire ne correspond, retourne -1
}

std::string ligne::get_suivant(std::string arret) {
    for (int i = 0; i < stop_ids.size(); i++) {
        if (stop_ids[i] == arret) {
            if (i + 1 < stop_ids.size()) {
                return stop_ids[i + 1]; // Retourne l'arrêt suivant
            } else {
                return "-1"; // Si c'est le dernier arrêt, retourne nullptr
            }
        }
    }
    return "FIN"; // Si l'arrêt n'est pas trouvé, retourne nullptr
}

int ligne::get_ind_arret(std::string arret) {
    for (int i = 0; i < stop_ids.size(); i++) {
        if (stop_ids[i] == arret) {
            return i; // Retourne l'arrêt correspondant
        }
    }
    return -1; // Si l'arrêt n'est pas trouvé, retourne nullptr
}




