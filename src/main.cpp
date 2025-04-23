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

    // Afficher les informations des arrêts
    // for(auto it = stops.begin(); it != stops.end(); ++it) {
    //     cout << "ID de l'arrêt : " << it->first << endl;
    //     it->second.print(); // Afficher les informations de l'arrêt
    //     it->second.printLignes(); // Afficher les lignes associées à chaque arrêt
    // }


    // Afficher les informations des lignes
    for (const auto& ligne : lignes) {
        ligne.print();
        ligne.printArrets();
        ligne.printHoraires();
    }
    //lignes[0].print();
    //lignes[0].printArrets();
    // lignes[0].printHoraires();

    // Exemple de nœuds reliés à la main avec stopName et nomLigne
    Horaire h1 = {7, 0};
    // Entrée utilisateur pour le départ et l'arrivée
    string departName, arriveeName;
    cout << "Entrez le nom de l'arrêt de départ : ";
    getline(cin, departName);
    cout << "Entrez le nom de l'arrêt d'arrivée : ";
    getline(cin, arriveeName);
    vector<string> depart = getStopIdByName(departName, stops);
    vector<string> arrivee = getStopIdByName(arriveeName, stops);

    // string depart = "0:PEgrou1"; // ID de l'arrêt de départ
    // string arrivee = "0:BXcayr1"; // ID de l'arrêt d'arrivée
    vector<vector<Noeud>> chemin;
    vector<vector<int>> indicesChangement;
    for(int i = 0 ; i < depart.size() ; i++){
        for(int j = 0 ; j < arrivee.size() ; j++){
            Noeud Fin = Dijktra(depart[i], arrivee[j], h1, &stops, &lignes, &arretsVoisin, &arretsVisites);

            // Construire le chemin à partir des nœuds
            cout << "Itération i " << i << endl;
            vector<Noeud> temp_chemin;
            Noeud current = Fin;
            cout << "On va rentre dans le whiiiiiiiile" << endl;
            while (current.precedent != -1) {
                cout << "Bah là on est toujours dans le while" << endl;
                temp_chemin.insert(temp_chemin.begin(), current); // Ajouter le nœud au début du chemin
                cout << "C'est le current qui pose problème" << endl;
                current = arretsVisites[current.precedent];
                cout << "Fin du while, itération suivante ?" << endl;
            }
            cout << "Le while est fini, quitter la boucle !" << endl;
            temp_chemin.insert(temp_chemin.begin(), arretsVisites[0]); // Ajouter le nœud de départ
            // Identifier les indices de changement de ligne
            cout << "On va rentrer dans le 2e for" << endl;
            vector<int> indices_temp;
            for (size_t k = 1; k < temp_chemin.size(); ++k) {
                cout << "Itération k " << k << endl;
                if (temp_chemin[k].ligneId != temp_chemin[k - 1].ligneId) {
                    cout << "On est rentré dans le if " << i << " " << k << endl;
                    indices_temp.push_back(k - 1);
                }
                cout << "Fin itération k " << k << endl;
            }
            cout << "Fin itération i " << i << endl;
            chemin.push_back(temp_chemin);
            indicesChangement.push_back(indices_temp);
        }
    }

    cout << "Dijkstra finito !!! et chemins aussi" << endl;

    // Afficher le chemin
    for(int i = 0 ; i < chemin.size() ; i++)
        if(chemin[i].size() > 1)
            afficherChemin(chemin[i], indicesChangement[i], &stops, &lignes);

    return 0;
}