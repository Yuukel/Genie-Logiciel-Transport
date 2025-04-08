#include "trajet.hpp"
#include "arret.hpp"
#include "parsing.hpp"

// Conteneurs globaux pour stocker les objets
std::vector<Ligne> lignes; // Liste des lignes
std::unordered_map<std::string, Arret> stops; // Map des arrêts (stop_id -> stop_name)
std::unordered_map<std::string, std::string> tripHeadsigns;
std::unordered_map<std::string, std::string> tripRouteIds;

int main(){
    char stopsFilePath[] = "data/stops.txt";
    char tripsFilePath[] = "data/trips.txt";
    char stopTimesFilePath[] = "data/stop_times.txt";

    // Charger les arrêts depuis le fichier stops.txt
    readStop(stopsFilePath, &stops);
 
     // Charger les lignes depuis trips.txt
    readTrips(tripsFilePath, &lignes, tripHeadsigns, tripRouteIds);

    // Compléter les lignes avec les arrêts et horaires depuis stop_times.txt
    completeLignes(stopTimesFilePath, &lignes, stops, tripHeadsigns, tripRouteIds);

    // Supprimer les doublons d'arrêts
    // removeDuplicateStops(&lignes);

    // Afficher les informations des lignes
    // for (const auto& ligne : lignes) {
    //     ligne.print();
    //     ligne.printArrets();
    //     ligne.printHoraires();
    // }
    lignes[0].print();
    lignes[0].printArrets();
    // lignes[0].printHoraires();

    return 0;
}