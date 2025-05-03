
#include "parsing.hpp"
#include<set>

void readStop(char * filePath, unordered_map<string, Arret>* stops){
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << endl;
        return;
    }

    string line;
    getline(file, line); // Lire la première ligne (en-tête) et l'ignorer

    while (getline(file, line)) {
        istringstream lineStream(line);
        string cell;
        vector<string> columns;

        // Lire chaque colonne séparée par une virgule
        while (getline(lineStream, cell, ',')) {
            columns.push_back(cell);
        }

        // Vérifier que la ligne contient suffisamment de colonnes
        if (columns.size() < 6) {
            cerr << "Erreur : Ligne mal formatée ou incomplète." << endl;
            continue;
        }

        // Extraire uniquement les colonnes souhaitées (par exemple, stop_id, stop_name, stop_lat, stop_lon)
        string stopId = columns[0];
        string stopName = columns[2];

        // Ignorer les arrêts dont l'ID commence par '1' car ils n'ont pas de lignes qui passent par eux
        if (!stopId.empty() && stopId[0] == '1') {
            continue;
        }

        // Retirer le dernier caractère de l'ID de l'arrêt
        if (!stopId.empty()) {
            stopId.pop_back(); // Supprime le dernier caractère
        }

        // Si un arrêt avec le même ID existe déjà, fusionner les informations
        if (stops->find(stopId) != stops->end()) {
            continue; // On fusionne les arrêts ayant plusieurs itérations mais un numéro différent (impact potentiel sur le sens)
        }
        
        // Créer un objet Arret et l'ajouter à la map
        Arret arret(stopId, stopName);
        (*stops)[stopId] = arret;
        
    }

    file.close();
}

void readTrips(char* filePath, vector<Ligne>* lignes, unordered_map<string, string>& tripHeadsigns, unordered_map<string, string>& tripRouteIds) {
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << endl;
        return;
    }

    string line;
    getline(file, line); // Lire la première ligne (en-tête) et l'ignorer

    // Map temporaire pour éviter les doublons (clé : route_id + trip_headsign)
    unordered_map<string, bool> ligneExistante;

    while (getline(file, line)) {
        istringstream lineStream(line);
        string routeId, serviceId, tripId, tripHeadsign;

        // Lire les colonnes nécessaires
        getline(lineStream, routeId, ',');
        getline(lineStream, serviceId, ',');
        getline(lineStream, tripId, ',');
        getline(lineStream, tripHeadsign, ',');

        // Ajouter l'association tripId -> tripHeadsign
        tripHeadsigns[tripId] = tripHeadsign;

        // Ajouter l'association tripId -> routeId
        tripRouteIds[tripId] = routeId;

        // Générer une clé unique pour éviter les doublons
        string key = routeId + "_" + tripHeadsign;

        // Vérifier si cette ligne existe déjà
        if (ligneExistante.find(key) == ligneExistante.end()) {
            // Créer une nouvelle ligne
            Ligne ligne(tripId, "Ligne " + routeId, tripHeadsign);

            // Ajouter la ligne au vecteur
            lignes->push_back(ligne);

            // Marquer cette ligne comme existante
            ligneExistante[key] = true;
        }
    }

    file.close();
}

void completeLignes(char* filePath, vector<Ligne>* lignes, unordered_map<string, Arret>* stops, unordered_map<string, string>& tripHeadsigns, unordered_map<string, string>& tripRouteIds) {
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << endl;
        return;
    }

    string line;
    getline(file, line); // Lire la première ligne (en-tête) et l'ignorer

    // Map pour regrouper les arrêts et horaires par `trip_id`
    unordered_map<string, vector<pair<string, Horaire>>> tripStops;

    // Lire toutes les lignes du fichier et regrouper les arrêts par `trip_id`
    while (getline(file, line)) {
        istringstream lineStream(line);
        string tripId, arrivalTime, departureTime, stopId, stopSequence;

        // Lire les colonnes nécessaires
        getline(lineStream, tripId, ',');        // trip_id
        getline(lineStream, arrivalTime, ',');   // arrival_time
        getline(lineStream, departureTime, ','); // departure_time
        getline(lineStream, stopId, ',');        // stop_id
        getline(lineStream, stopSequence, ',');  // stop_sequence

        // Extraire l'heure et la minute de l'arrivée
        unsigned int heure, minute;
        if (sscanf(arrivalTime.c_str(), "%u:%u", &heure, &minute) != 2) {
            cerr << "Erreur : Format d'heure invalide pour " << arrivalTime << endl;
            continue;
        }
        Horaire horaire = {heure, minute};

        // Ajouter l'arrêt et l'horaire à la liste pour ce `trip_id`
        tripStops[tripId].emplace_back(stopId, horaire);
    }

    file.close();

    // Parcourir chaque `trip_id` et comparer les séquences d'arrêts
    for (const auto& [tripId, stopsAndHoraires] : tripStops) {
        // Obtenir `route_id` et `tripHeadsign` pour ce `trip_id`
        string routeId = tripRouteIds[tripId];
        string tripHeadsign = tripHeadsigns[tripId];

        // Trouver la ligne correspondante
        auto it = find_if(lignes->begin(), lignes->end(), [&routeId, &tripHeadsign](const Ligne& ligne) {
            return ligne.nomLigne == "Ligne " + routeId && ligne.tripHeadsign == tripHeadsign;
        });

        if (it != lignes->end()) {
            // Comparer la séquence d'arrêts
            const auto& existingStops = it->stopIds;
            vector<string> newStops;
            for (const auto& [stopId, _] : stopsAndHoraires) {
                string adjustedStopId = stopId;

                // Retirer le dernier caractère de l'ID de l'arrêt
                if (!adjustedStopId.empty()) {
                    adjustedStopId.pop_back();
                }

                newStops.push_back(adjustedStopId);
            }

            if (existingStops.empty()) {
                // Si la ligne n'a pas encore d'arrêts, initialiser avec la nouvelle séquence
                it->stopIds = newStops;
                it->horaires.resize(newStops.size());
            }

            if (existingStops == newStops) {
                // Ajouter les horaires pour chaque arrêt
                for (size_t i = 0; i < stopsAndHoraires.size(); ++i) {
                    const auto& [stopId, horaire] = stopsAndHoraires[i];
                    it->horaires[i].push_back(horaire);

                    // Retirer le dernier caractère de l'ID de l'arrêt pour la comparaison
                    string adjustedStopId = stopId;
                    if (!adjustedStopId.empty()) {
                        adjustedStopId.pop_back();
                    }

                    // Mettre à jour la map `stops` pour associer l'arrêt à la ligne
                    if (stops->find(adjustedStopId) != stops->end() && 
                    find(stops->at(adjustedStopId).lignes.begin(), stops->at(adjustedStopId).lignes.end(), it->idLigne) == stops->at(adjustedStopId).lignes.end()) {
                        stops->at(adjustedStopId).addLigne(it->idLigne);
                    }
                }
            }
        }
    }

    nettoyerHorairesEtArrets(lignes);
}


void nettoyerHorairesEtArrets(vector<Ligne>* lignes) {
    for (Ligne& ligne : *lignes) {
        // Nettoyer les horaires (suppression de doublons dans chaque vecteur de Horaire)
        for (vector<Horaire>& h : ligne.horaires) {
            sort(h.begin(), h.end());
            h.erase(unique(h.begin(), h.end()), h.end());
        }

        // Supprimer les arrêts en doublon selon leur stopId
        vector<string>& stopIds = ligne.stopIds;  // Utiliser les stopIds (strings)
        vector<vector<Horaire>>& horaires = ligne.horaires;

        vector<string> stopIds_uniques;
        vector<vector<Horaire>> horaires_uniques;
        set<string> ids_vus;

        for (size_t i = 0; i < stopIds.size(); ++i) {
            const string& id = stopIds[i];
            if (ids_vus.find(id) == ids_vus.end()) {
                ids_vus.insert(id);
                stopIds_uniques.push_back(id);
                horaires_uniques.push_back(horaires[i]);
            }
        }

        ligne.stopIds = std::move(stopIds_uniques);
        ligne.horaires = std::move(horaires_uniques);
    }
}