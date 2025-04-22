#ifndef CLI_HPP
#define CLI_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "dijkstra.hpp"
#include "trajet.hpp"

string getStopIdByName(const string& stopName, const unordered_map<string, Arret>& stops);
void afficherChemin(const std::vector<Noeud>& chemin, const std::vector<int>& indicesChangement ,unordered_map<std::string, Arret>* stops, vector<Ligne>* lignes);

#endif