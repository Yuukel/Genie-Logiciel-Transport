#include<iostream>
#include "parsing.hpp"

void readStop(char * filePath, unordered_map<std::string, Arret>* stops){
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Lire la première ligne (en-tête) et l'ignorer

    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string cell;
        std::vector<std::string> columns;

        // Lire chaque colonne séparée par une virgule
        while (std::getline(lineStream, cell, ',')) {
            columns.push_back(cell);
        }

        // Vérifier que la ligne contient suffisamment de colonnes
        if (columns.size() < 6) {
            std::cerr << "Erreur : Ligne mal formatée ou incomplète." << std::endl;
            continue;
        }

        // Extraire uniquement les colonnes souhaitées (par exemple, stop_id, stop_name, stop_lat, stop_lon)
        std::string stopId = columns[0];
        std::string stopName = columns[2];

        // Créer un objet Arret et l'ajouter à la map
        Arret arret(stopId, stopName);
        (*stops)[stopId] = arret;
        
    }

    file.close();
}

void readTrips(char* filePath, std::vector<Ligne>* lignes) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Lire la première ligne (en-tête) et l'ignorer

    // Map temporaire pour éviter les doublons (clé : route_id + trip_headsign)
    std::unordered_map<std::string, bool> ligneExistante;

    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string routeId, serviceId, tripId, tripHeadsign;

        // Lire les colonnes nécessaires
        std::getline(lineStream, routeId, ',');
        std::getline(lineStream, serviceId, ',');
        std::getline(lineStream, tripId, ',');
        std::getline(lineStream, tripHeadsign, ',');

        // Générer une clé unique pour éviter les doublons
        std::string key = routeId + "_" + tripHeadsign;

        // Vérifier si cette ligne existe déjà
        if (ligneExistante.find(key) == ligneExistante.end()) {
            // Créer une nouvelle ligne
            Ligne ligne(routeId, "Ligne " + routeId, tripHeadsign);

            // Ajouter la ligne au vecteur
            lignes->push_back(ligne);

            // Marquer cette ligne comme existante
            ligneExistante[key] = true;
        }
    }

    file.close();
}

void completeLignes(char* filePath, std::vector<Ligne>& lignes, std::unordered_map<std::string, Arret>& stops) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Lire la première ligne (en-tête) et l'ignorer

    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string tripId, arrivalTime, departureTime, stopId, stopSequence;

        // Lire les colonnes nécessaires
        std::getline(lineStream, tripId, ',');        // trip_id
        std::getline(lineStream, arrivalTime, ',');   // arrival_time
        std::getline(lineStream, departureTime, ','); // departure_time
        std::getline(lineStream, stopId, ',');        // stop_id
        std::getline(lineStream, stopSequence, ',');  // stop_sequence

        // Extraire l'heure et la minute de l'arrivée
        unsigned int heure, minute;
        if (sscanf(arrivalTime.c_str(), "%u:%u", &heure, &minute) != 2) {
            std::cerr << "Erreur : Format d'heure invalide pour " << arrivalTime << std::endl;
            continue;
        }
        Horaire horaire = {heure, minute};

        // Trouver la ligne correspondante en fonction de trip_id
        for (auto& ligne : lignes) {
            // Vérifier si le trip_id correspond à la ligne (idLigne) et au terminus (tripHeadsign)
            if (tripId.find(ligne.idLigne) != std::string::npos && tripId.find(ligne.tripHeadsign) != std::string::npos) {
                // Ajouter l'arrêt à la ligne
                ligne.addArret(stopId);

                // Ajouter l'horaire à la ligne en utilisant addHoraire
                ligne.addHoraire({horaire});

                // Ajouter la ligne à l'arrêt dans la map des arrêts
                if (stops.find(stopId) != stops.end()) {
                    stops[stopId].addLigne(ligne.idLigne);
                }
                break;
            }
        }
    }

    file.close();
}