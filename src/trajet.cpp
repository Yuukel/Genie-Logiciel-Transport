#include "trajet.hpp"
#include "arret.hpp"


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
    for (int i = 0; i < stops.size(); i++) {
        printf("%s\n", stops[i]->stop_name.c_str());
    }
}

void ligne::print_horaires() {
    printf("Horaires de la ligne :\n");
    for (int i = 0; i < horaires.size(); i++) {
        printf("%02d:%02d\n", horaires[i][0].heure, horaires[i][0].minute);
    }
}

void ligne::add_arret(std::shared_ptr<arret> arret) {
    stops.push_back(arret);
}

void ligne::add_horaire(std::vector<horaire> horaire) {
    horaires.push_back(horaire);
}

std::vector<std::shared_ptr<arret>> ligne::get_arrets() {
    return stops;
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

std::shared_ptr<arret> ligne::get_suivant(std::shared_ptr<arret> arret) {
    for (int i = 0; i < stops.size(); i++) {
        if (stops[i]->stop_id == arret->stop_id) {
            if (i + 1 < stops.size()) {
                return stops[i + 1]; // Retourne l'arrêt suivant
            } else {
                return nullptr; // Si c'est le dernier arrêt, retourne nullptr
            }
        }
    }
    return nullptr; // Si l'arrêt n'est pas trouvé, retourne nullptr
}

std::shared_ptr<arret> ligne::get_id_arret(std::shared_ptr<arret> arret) {
    for (int i = 0; i < stops.size(); i++) {
        if (stops[i]->stop_id == arret->stop_id) {
            return stops[i]; // Retourne l'arrêt correspondant
        }
    }
    return nullptr; // Si l'arrêt n'est pas trouvé, retourne nullptr
}

