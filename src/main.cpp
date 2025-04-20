#include "trajet.hpp"
#include "arret.hpp"
#include "parsing.hpp"
// #include "dijkstra.hpp"
#include "cli.hpp"

// Conteneurs globaux pour stocker les objets
std::vector<Ligne> lignes; // Liste des lignes
std::unordered_map<std::string, Arret> stops; // Map des arrêts (stop_id -> stop_name)
std::unordered_map<std::string, std::string> tripHeadsigns;
std::unordered_map<std::string, std::string> tripRouteIds;


// Variables pour l'algorithme de Dijkstra
vector<Noeud> arretsVoisin; //les voisins direct des arret deja visites
vector<string> arretsVisites; // liste des arrets deja visites

int main(){
    char stopsFilePath[] = "data/stops.txt";
    char tripsFilePath[] = "data/trips.txt";
    char stopTimesFilePath[] = "data/stop_times.txt";

    // Charger les arrêts depuis le fichier stops.txt
    readStop(stopsFilePath, &stops);
 
     // Charger les lignes depuis trips.txt
    readTrips(tripsFilePath, &lignes, tripHeadsigns, tripRouteIds);

    completeLignes(stopTimesFilePath, &lignes, &stops, tripHeadsigns, tripRouteIds);

    for(auto it = stops.begin(); it != stops.end(); ++it) {
        std::cout << "ID de l'arrêt : " << it->first << std::endl;
        it->second.print(); // Afficher les informations de l'arrêt
        it->second.printLignes(); // Afficher les lignes associées à chaque arrêt
    }

    // Afficher les informations des lignes
    // for (const auto& ligne : lignes) {
    //     ligne.print();
    //     ligne.printArrets();
    //     ligne.printHoraires();
    // }
    lignes[0].print();
    lignes[0].printArrets();
    // lignes[0].printHoraires();

    // // Exemple de test Dijkstra
    // std::string depart = "0:LBnaia1"; // ID de l'arrêt de départ
    // std::string arrivee = "0:LBnaut1"; // ID de l'arrêt d'arrivée
    // Horaire heureDepart = {8, 30}; // Heure de départ : 8h30

    // std::vector<Noeud> chemin;
    // std::vector<int> indicesChangement;

    // // Appeler Dijkstra pour calculer le chemin
    // Noeud resultat = Dijktra(depart, arrivee, heureDepart, &stops, &lignes, &arretsVoisin, &arretsVisites);

    // // Reconstruire le chemin à partir du résultat
    // Noeud* current = &resultat;
    // while (current != nullptr) {
    //     chemin.insert(chemin.begin(), *current); // Ajouter le noeud au début du chemin
    //     current = current->precedent;
    // }

    // // Identifier les indices de changement de ligne
    // for (size_t i = 1; i < chemin.size(); ++i) {
    //     if (chemin[i].ligneId != chemin[i - 1].ligneId) {
    //         indicesChangement.push_back(i - 1);
    //     }
    // }

    // // Afficher le chemin
    // afficherChemin(chemin, indicesChangement);

    // Exemple de nœuds reliés à la main avec stopName et nomLigne
    Noeud noeud1("Naïades", "Ligne 1", nullptr, {8, 30}); // Départ
    Noeud noeud2("Arrêt B", "Ligne 1", &noeud1, {8, 40}); 
    Noeud noeud3("Arrêt C", "Ligne 1", &noeud2, {8, 50}); 
    Noeud noeud4("Nautica", "Ligne 2", &noeud3, {9, 10}); // Arrivée

    // Construire le chemin à partir des nœuds
    std::vector<Noeud> chemin;
    Noeud* current = &noeud4;
    while (current != nullptr) {
        chemin.insert(chemin.begin(), *current); // Ajouter le nœud au début du chemin
        current = current->precedent;
    }

    // Identifier les indices de changement de ligne
    std::vector<int> indicesChangement;
    for (size_t i = 1; i < chemin.size(); ++i) {
        if (chemin[i].ligneId != chemin[i - 1].ligneId) {
            indicesChangement.push_back(i - 1);
        }
    }

    // Afficher le chemin
    afficherChemin(chemin, indicesChangement);

    return 0;
}