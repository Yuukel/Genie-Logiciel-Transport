#include "dijkstra.hpp"
#include <queue>

using namespace std;

struct CompareHoraire {
    bool operator()(const Noeud& a, const Noeud& b) {
        return b.heure < a.heure;
    }
};

int Dans_lignes(const string& ligneId, const vector<Ligne>* lignes) {
    for (size_t i = 0; i < lignes->size(); ++i) {
        if ((*lignes)[i].idLigne == ligneId) {
            return i;
        }
    }
    return -1;
}

int Deja_visite(const string& arretId, const vector<Noeud>* arretsVisites) {
    for (size_t i = 0; i < arretsVisites->size(); ++i) {
        if ((*arretsVisites)[i].arretId == arretId) {
            return i;
        }
    }
    return -1;
}

Noeud Dijktra(string depart, string arrivee, Horaire heure, unordered_map<string, Arret>* stops, vector<Ligne>* lignes, vector<Noeud>* arretsVoisins, vector<Noeud>* arretsVisites) {
    arretsVisites->clear();
    arretsVoisins->clear();

    std::priority_queue<Noeud, std::vector<Noeud>, CompareHoraire> filePriorite;

    Noeud dernierTraite(depart, "Depart", -1, heure);
    arretsVisites->push_back(dernierTraite);
    filePriorite.push(dernierTraite);

    if (depart == arrivee) {
        std::cout << "Le départ est le même que l'arrivée." << std::endl;
        return dernierTraite;
    }

    Horaire temps_max = {0, 0};
    std::vector<std::string> lignesDirectArrivee = (*stops)[arrivee].getLignes();

    for (const std::string& ligneId : lignesDirectArrivee) {
        int indexLigne = Dans_lignes(ligneId, lignes);
        if (indexLigne != -1) {
            int indexArret = (*lignes)[indexLigne].getIndArret(arrivee);
            Horaire h = (*lignes)[indexLigne].getDernierHoraire(indexArret);
            if (temps_max < h) temps_max = h;
        }
    }

    std::vector<std::string> lignesDirectDepart = (*stops)[depart].getLignes();

    int iteration = 0;
    while (!filePriorite.empty()) {
        if (++iteration > 200) { // Augmentation de la limite d'itérations par sécurité
            std::cout << "Sécurité : boucle infinie détectée." << std::endl;
            return Noeud("ErreurBoucle", "Erreur", -1, temps_max);
        }

        dernierTraite = filePriorite.top();
        filePriorite.pop();

        if (dernierTraite.arretId == arrivee) {
            std::cout << "Chemin trouvé avec succès." << std::endl;
            std::vector<Noeud> chemin;
            Noeud courant = dernierTraite;
            while (courant.precedent != -1) {
                chemin.push_back(courant);
                courant = (*arretsVisites)[courant.precedent];
            }
            chemin.push_back(courant); // Ajouter le nœud de départ
            std::reverse(chemin.begin(), chemin.end()); // Inverser le chemin pour avoir l'ordre de départ à arrivée

            std::cout << "Chemin : ";
            for (const auto& noeud : chemin) {
                noeud.print();
                std::cout << " -> ";
            }
            std::cout << "FIN" << std::endl;

            return dernierTraite;
        }

        for (const std::string& ligneId : lignesDirectDepart) {
            int indexLigne = Dans_lignes(ligneId, lignes);
            if (indexLigne == -1) continue;

            std::string tempoArret = (*lignes)[indexLigne].getSuivant(dernierTraite.arretId);
            if (tempoArret == "FIN" || tempoArret == "-1") continue;

            int indexArret = (*lignes)[indexLigne].getIndArret(tempoArret);
            int indexHeure = (*lignes)[indexLigne].getHorairesPrecis(dernierTraite.heure, indexArret);
            if (indexHeure == -1) continue;

            Horaire h = (*lignes)[indexLigne].horaires[indexArret][indexHeure];
            if (!(dernierTraite.heure < h)) continue;

            int indexVisite = Deja_visite(tempoArret, arretsVisites);
            if (indexVisite != -1) {
                // Si l'arrêt a déjà été visité, mais qu'on a trouvé un chemin plus rapide pour l'atteindre
                if (h < (*arretsVisites)[indexVisite].heure) {
                    (*arretsVisites)[indexVisite].heure = h;
                    (*arretsVisites)[indexVisite].precedent = arretsVisites->size() - 1; // Le précédent est maintenant le dernier traité
                    // Il faut potentiellement réinsérer ce nœud dans la file de priorité pour explorer ses voisins avec la nouvelle heure
                    filePriorite.push(Noeud(tempoArret, (*lignes)[indexLigne].idLigne, arretsVisites->size() - 1, h));
                }
                continue; // Ne pas ajouter un nouveau nœud à arretsVisites si on a juste mis à jour un existant
            }

            // Si l'arrêt n'a pas été visité
            Noeud tempo(tempoArret, (*lignes)[indexLigne].idLigne, arretsVisites->size() - 1, h);
            filePriorite.push(tempo);
        }

        arretsVisites->push_back(dernierTraite);
        lignesDirectDepart = (*stops)[dernierTraite.arretId].getLignes();
    }

    std::cout << "Aucun chemin trouvé." << std::endl;
    return Noeud("Erreur", "Erreur", -1, temps_max);
}