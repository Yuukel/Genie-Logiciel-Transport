#ifndef TRAJET
#define TRAJET

#include <vector>
#include <string>
#include <unordered_map>

struct horaire {
    unsigned int heure;
    unsigned int minute;
} typedef horaire;

class ligne {
    public:
    std::string id_ligne;
    std::string nom_ligne;
    std::string trip_headsign;
    std::vector<std::string> stop_ids; // IDs des arrêts
    std::vector<std::vector<horaire>> horaires;

    ligne(std::string id, std::string nom, std::string headsign);
    ligne();

    void print(); // affiche les informations de la ligne
    void print_arrets(); // affiche les arrêts de la ligne
    void print_horaires(); // affiche les horaires de la ligne    
    void add_arret(std::string arret); // ajoute un arrêt à la liste des arrêts de la ligne
    void add_horaire(std::vector<horaire> horaire); // ajoute un horaire à la liste des horaires de la ligne

    std::vector<std::string> get_arrets(); // liste des arrêts de la ligne
    std::vector<std::vector<horaire>> get_horaires(); // liste des horaires de la ligne
    int get_horaires_precis(horaire ,int); // donne l'indice de l'horaire qui passe après l'horaire donné à l'arrêt donné
    std::string get_suivant(std::string arret); // retourne l'arrêt suivant de la ligne
    int get_ind_arret(std::string arret); // retourne l'indice de l'arrêt
};

#endif