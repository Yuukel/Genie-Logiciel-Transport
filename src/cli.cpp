#include "cli.hpp"

// void afficherChemin(const vector<Arret>& chemin, const vector<Ligne>& lignesDistinctes, const vector<int>& indicesChangement) {
//     cout << "Chemin entre les arrêts :\n" << chemin.front().stopName << " -> " << chemin.back().stopName << endl;

//     // Afficher les lignes de transport
//     cout << "Lignes : ";
//     size_t ligneIndex = 0;
//     for (size_t i = 0; i < chemin.size(); ++i) {
//         // Afficher la ligne actuelle
//         cout << lignesDistinctes[ligneIndex].nomLigne;

//         // Vérifier si un changement de ligne est nécessaire
//         if (ligneIndex < indicesChangement.size() && i == static_cast<size_t>(indicesChangement[ligneIndex])) {
//             ++ligneIndex; // Passer à la ligne suivante
//         }

//         if (i < chemin.size() - 1) {
//             cout << "      "; // Espacement entre les lignes
//         }
//     }
//     cout << endl;

//     // Afficher les arrêts
//     cout << "Arrêts : ";
//     for (size_t i = 0; i < chemin.size(); ++i) {
//         cout << chemin[i].stopName; // Afficher le nom de l'arrêt
//         if (i < chemin.size() - 1) {
//             cout << "      "; // Espacement entre les arrêts
//         }
//     }
//     cout << endl;
// }

// void afficherChemin(const vector<Noeud>& chemin, const vector<int>& indicesChangement) {
//     if (chemin.empty()) {
//         cout << "Aucun chemin trouvé entre les arrêts spécifiés." << endl;
//         return;
//     }

//     cout << "Chemin entre les arrêts :\n" 
//               << chemin.front().arretId << " -> " << chemin.back().arretId << endl;

//     // Afficher les lignes de transport
//     cout << "Lignes : ";
//     size_t ligneIndex = 0;
//     for (size_t i = 0; i < chemin.size(); ++i) {
//         cout << chemin[i].ligneId; // Afficher l'ID de la ligne

//         // Vérifier si un changement de ligne est nécessaire
//         if (ligneIndex < indicesChangement.size() && i == static_cast<size_t>(indicesChangement[ligneIndex])) {
//             ++ligneIndex; // Passer à la ligne suivante
//         }

//         if (i < chemin.size() - 1) {
//             cout << "      "; // Espacement entre les lignes
//         }
//     }
//     cout << endl;

//     // Afficher les arrêts
//     cout << "Arrêts : ";
//     for (size_t i = 0; i < chemin.size(); ++i) {
//         cout << chemin[i].arretId; // Afficher l'ID de l'arrêt
//         if (i < chemin.size() - 1) {
//             cout << "      "; // Espacement entre les arrêts
//         }
//     }
//     cout << endl;

//     // Afficher les horaires
//     cout << "Horaires : ";
//     for (size_t i = 0; i < chemin.size(); ++i) {
//         cout << chemin[i].heure.heure << ":" << setw(2) << setfill('0') << chemin[i].heure.minute;
//         if (i < chemin.size() - 1) {
//             cout << "      "; // Espacement entre les horaires
//         }
//     }
//     cout << endl;
// }

void afficherChemin(const vector<Noeud>& chemin, const vector<int>& indicesChangement) {
    if (chemin.empty()) {
        cout << "Aucun chemin trouvé entre les arrêts spécifiés." << endl;
        return;
    }

    cout << "Chemin détaillé entre les arrêts :" << endl;

    for (size_t i = 0; i < chemin.size(); ++i) {
        cout << "Étape " << i + 1 << ":" << endl;
        cout << "  Arrêt : " << chemin[i].arretId;
        cout << "  Ligne : " << chemin[i].ligneId;
        cout << "  Horaire : " << chemin[i].heure.heure << ":" 
             << setw(2) << setfill('0') << chemin[i].heure.minute << endl;

        // Vérifier si un changement de ligne est nécessaire
        if (find(indicesChangement.begin(), indicesChangement.end(), static_cast<int>(i)) != indicesChangement.end()) {
            cout << "  ** Changement de ligne à la prochaine étape**" << endl;
        }

        cout << endl; // Espacement entre les étapes
    }
}