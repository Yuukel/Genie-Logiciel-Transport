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

    // Afficher les informations des arrêts
    // for(auto it = stops.begin(); it != stops.end(); ++it) {
    //     cout << "ID de l'arrêt : " << it->first << endl;
    //     it->second.print(); // Afficher les informations de l'arrêt
    //     it->second.printLignes(); // Afficher les lignes associées à chaque arrêt
    // }

    // Afficher les informations des lignes
    // int cpt2;

    // for (const auto& ligne : lignes) {
    //     ligne.print();
    //     ligne.printArrets();
    //     ligne.printHoraires();
    //     cpt2++;
    // }
    // cout << "cpt2 = " << cpt2 << endl;
    //lignes[0].print();
    //lignes[0].printArrets();
    // lignes[0].printHoraires();

    // Exemple de nœuds reliés à la main avec stopName et nomLigne
    // Horaire h1 = {8, 50};
    // // Entrée utilisateur pour le départ et l'arrivée
    // string departName, arriveeName;
    // cout << "Entrez le nom de l'arrêt de départ : ";
    // getline(cin, departName);
    // cout << "Entrez le nom de l'arrêt d'arrivée : ";
    // getline(cin, arriveeName);
    // vector<string> depart = getStopIdByName(departName, stops);
    // vector<string> arrivee = getStopIdByName(arriveeName, stops);

    // vector<vector<vector<Noeud>>> cheminsFinaux;
    // // vector<vector<int>> indicesChangements;
    // for(int i = 0 ; i < depart.size() ; i++){
    //     cout << "depart[i] = " << depart[i] << endl;
    //     for(int j = 0 ; j < arrivee.size() ; j++){
    //         cout << "arrivee[j] = " << arrivee[j] << endl;
    //         vector<vector<Noeud>> chemin = Dijkstra(depart[i], arrivee[j], h1, &stops, &lignes);

    //         // vector<int> indicesTemp;
    //         // for (size_t k = 1; k < chemin.size(); ++k) {
    //         //     // cout << "Debug : " << chemin[k - 1].ligneId << endl;
    //         //     if (chemin[k].ligneId != chemin[k - 1].ligneId) {
    //         //         indicesTemp.push_back(k - 1);
    //         //     }
    //         // }
    //         if(!chemin.empty())
    //             cheminsFinaux.push_back(chemin);
    //         // indicesChangements.push_back(indicesTemp);
    //     }
    // }

    // cout << cheminsFinaux.size() << endl;
    // for(int i = 0 ; i < cheminsFinaux.size() ; i++){
    //     cout << "Chemin numéro " << i << endl;
    //     for(int j = 0 ; j < cheminsFinaux[i].size() ; j++){
    //         int indexLigne = trouveLigne(cheminsFinaux[i][j][0].ligneId , &lignes);
    //         cout << "Ligne " << lignes[indexLigne].nomLigne << "(" << cheminsFinaux[i][j][0].ligneId << ")" << endl;
    //         for(int k = 0 ; k < cheminsFinaux[i][j].size() ; k++){
    //             cout << "  Arrêt : " << stops[cheminsFinaux[i][j][k].arretId].stopName;
    //             cout << "  Horaire : " << cheminsFinaux[i][j][k].heure.heure << ":"
    //             << setw(2) << setfill('0') << cheminsFinaux[i][j][k].heure.minute << endl;
    //             cout << endl;
    //         }
    //     }
    //     cout << endl;
    // }



    // Afficher les chemins
    // for(int i = 0 ; i < cheminsFinaux.size() ; i++){
    //     if(cheminsFinaux[i].size() > 1)
    //         // afficherChemin(cheminsFinaux[i], indicesChangements[i], &stops, &lignes);
    // }

    // for(int i = 0 ; i < cheminsFinaux.size() ; i++){
    //     if(cheminsFinaux[i].size() > 1)
    //
    // }

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