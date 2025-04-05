#ifndef TRAJET
#define TRAJET


#include <vector>
#include <string>
#include <memory> // Pour std::shared_ptr
// shared_ptr est utilisé pour gérer la mémoire plus facilement et éviter les fuites de mémoire

class arret; // Déclaration anticipée de la classe arret
// pour éviter les dépendances circulaires entre les classes arret et ligne


struct horaire {
    unsigned int heure;
    unsigned int minute;
} typedef horaire;

// Trajet d'une ligne de transport en commun
class ligne {
    public:
    std::string id_ligne;       // ID de la ligne
    std::string nom_ligne;      // nom de la ligne
    std::string trip_headsign;  // nom du terminus
    std::vector<std::shared_ptr<arret>> stops; // liste des arrêts liés à la ligne
    std::vector<std::vector<horaire>> horaires; // liste des horaires de passage de la ligne

    ligne(std::string id, std::string nom, std::string headsign);
    ligne();

    void print(); // affiche les informations de la ligne
    void print_arrets(); // affiche les arrêts de la ligne
    void print_horaires(); // affiche les horaires de la ligne
    void add_arret(std::shared_ptr<arret> arret); // ajoute un arrêt à la liste des arrêts de la ligne
    void add_horaire(std::vector<horaire> horaire); // ajoute un horaire à la liste des horaires de la ligne

    std::vector<std::shared_ptr<arret>> get_arrets(); // liste des arrêts de la ligne
    std::vector<std::vector<horaire>> get_horaires(); // liste des horaires de la ligne
    int get_horaires_precis(horaire ,int); // donne l'indice de l'horaire qui passe après l'horaire donné à l'arrêt donné
    std::shared_ptr<arret> get_suivant(std::shared_ptr<arret> arret); // retourne l'arrêt suivant de la ligne
    std::shared_ptr<arret> get_id_arret(std::shared_ptr<arret> arret); // retourne l'indice de l'arrêt 
};

#endif