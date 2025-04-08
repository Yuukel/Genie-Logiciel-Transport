#ifndef PARSING_HPP
#define PARSING_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

#include "arret.hpp"
#include "trajet.hpp"

using namespace std;

void readStop(char * filePath, unordered_map<std::string, Arret>* stops);

void readTrips(char* filePath, std::vector<Ligne>* lignes, std::unordered_map<std::string, std::string>& tripHeadsigns, std::unordered_map<std::string, std::string>& tripRouteIds);

void completeLignes(char* filePath, std::vector<Ligne>* lignes, std::unordered_map<std::string, Arret>& stops, std::unordered_map<std::string, std::string>& tripHeadsigns, std::unordered_map<std::string, std::string>& tripRouteIds);

// void removeDuplicateStops(std::vector<Ligne>* lignes);

#endif