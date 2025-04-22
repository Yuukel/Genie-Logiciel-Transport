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


// Variables pour l'algorithme de Dijkstra
vector<Noeud> arretsVoisin; //les voisins direct des arret deja visites
vector<Noeud> arretsVisites; // liste des arrets deja visites

int main(){
    char stopsFilePath[] = "data/stops.txt";
    char tripsFilePath[] = "data/trips.txt";
    char stopTimesFilePath[] = "data/stop_times.txt";

    // Charger les arrêts depuis le fichier stops.txt
    readStop(stopsFilePath, &stops);
 
     // Charger les lignes depuis trips.txt
    readTrips(tripsFilePath, &lignes, tripHeadsigns, tripRouteIds);

    completeLignes(stopTimesFilePath, &lignes, &stops, tripHeadsigns, tripRouteIds);

    /*
    for(auto it = stops.begin(); it != stops.end(); ++it) {
        cout << "ID de l'arrêt : " << it->first << endl;
        it->second.print(); // Afficher les informations de l'arrêt
        it->second.printLignes(); // Afficher les lignes associées à chaque arrêt
    }
    */

    // Afficher les informations des lignes
    // for (const auto& ligne : lignes) {
    //     ligne.print();
    //     ligne.printArrets();
    //     ligne.printHoraires();
    // }
    //lignes[0].print();
    //lignes[0].printArrets();
    // lignes[0].printHoraires();

    // Exemple de nœuds reliés à la main avec stopName et nomLigne
    Horaire h1 = {8, 30};
    // Entrée utilisateur pour le départ et l'arrivée
    string departName, arriveeName;
    cout << "Entrez le nom de l'arrêt de départ : ";
    getline(cin, departName);
    cout << "Entrez le nom de l'arrêt d'arrivée : ";
    getline(cin, arriveeName);
    string depart = getStopIdByName(departName, stops);
    string arrivee = getStopIdByName(arriveeName, stops);

    // string depart = "0:PEgrou1"; // ID de l'arrêt de départ
    // string arrivee = "0:BXcayr1"; // ID de l'arrêt d'arrivée
    Noeud Fin = Dijktra(depart, arrivee, h1, &stops, &lignes, &arretsVoisin, &arretsVisites);

    // Construire le chemin à partir des nœuds
    vector<Noeud> chemin;
    Noeud current = Fin;
    while (current.precedent != -1) {
        chemin.insert(chemin.begin(), current); // Ajouter le nœud au début du chemin
        current = arretsVisites[current.precedent];
    }
    chemin.insert(chemin.begin(), arretsVisites[0]); // Ajouter le nœud de départ
    // Identifier les indices de changement de ligne
    vector<int> indicesChangement;
    for (size_t i = 1; i < chemin.size(); ++i) {
        if (chemin[i].ligneId != chemin[i - 1].ligneId) {
            indicesChangement.push_back(i - 1);
        }
    }

    // Afficher le chemin
    afficherChemin(chemin, indicesChangement, &stops, &lignes);


    return 0;
}