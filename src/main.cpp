#include "trajet.hpp"
#include "arret.hpp"
#include "parsing.hpp"
#include "dijkstra.hpp"
#include "cli.hpp"

// Conteneurs globaux pour stocker les objets
vector<Ligne> lignes; // Liste des lignes
unordered_map<string, Arret> stops; // Map des arrêts (stop_id -> stop_name)
unordered_map<string, string> tripHeadsigns;
unordered_map<string, string> tripRouteIds;

int main(){
    system("clear");

    char stopsFilePath[] = "data/stops.txt";
    char tripsFilePath[] = "data/trips.txt";
    char stopTimesFilePath[] = "data/stop_times.txt";

    // Charger les arrêts depuis le fichier stops.txt
    readStop(stopsFilePath, &stops);

     // Charger les lignes depuis trips.txt
    readTrips(tripsFilePath, &lignes, tripHeadsigns, tripRouteIds);

    completeLignes(stopTimesFilePath, &lignes, &stops, tripHeadsigns, tripRouteIds);

    char choix;
    do {
        entreeUtilisateur(&stops, &lignes);
        cout << "Voulez-vous entrer un trajet ? (O/N) : ";
        cin >> choix;
        system("clear");
        cin.ignore(); // Ignore le caractère de nouvelle ligne restant dans le tampon
    } while (choix == 'O' || choix == 'o');

    return 0;
}