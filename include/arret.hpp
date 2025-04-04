#ifndef ARRET
#define ARRET

#include "trajet.hpp" // Inclure le fichier d'en-tête de la classe trajet
#include <iostream> // Pour std::cout et std::endl
#include <cstdio> // Pour printf
#include <string>
#include <vector>
#include <string>
#include <memory> // Pour std::shared_ptr
// shared_ptr est utilisé pour gérer la mémoire plus facilement et éviter les fuites de mémoire

// Arret d'une ligne de transport en commun
class arret {
    public:
    std::string stop_name; // nom de l'arret
    std::string stop_id;   // unique à chaque arret
    std::vector<std::shared_ptr<ligne>> lignes; // liste des IDs des lignes qui passent par cet arret

    arret(std::string id, std::string nom); // constructeur
    arret(); // constructeur par défaut
    
    void print(); // affiche les informations de l'arret
    void print_lignes(); // affiche les lignes qui passent par cet arret
    std::vector<std::shared_ptr<ligne>> get_lignes(); // liste des lignes qui passent par cet arret
    void add_ligne(std::shared_ptr<ligne> ligne); // ajoute une ligne à la liste des lignes qui passent par cet arret 
};

#endif