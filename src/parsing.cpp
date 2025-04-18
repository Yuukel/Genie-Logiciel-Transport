
#include "parsing.hpp"

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
                newStops.push_back(stopId);
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
                    
                    // Mettre à jour la map `stops` pour associer l'arrêt à la ligne
                    if (stops->find(stopId) != stops->end() && find(stops->at(stopId).lignes.begin(), stops->at(stopId).lignes.end(), it->idLigne) == stops->at(stopId).lignes.end()) {
                        stops->at(stopId).addLigne(it->idLigne);
                    }
                }
            }
        }
    }

    removeDuplicateHoraire(lignes);
}

void removeDuplicateHoraire(vector<Ligne>* lignes) {
    for (int i = 0; i < lignes->size(); i++) {
        for (int j = 0; j < lignes->at(i).horaires.size(); j++) {
            // Trier les horaires pour permettre la suppression des doublons
            vector<Horaire>& horaires = lignes->at(i).horaires.at(j);
            sort(horaires.begin(), horaires.end());

            // Supprimer les doublons dans les horaires
            horaires.erase(unique(horaires.begin(), horaires.end()), horaires.end());
        }
    }
}