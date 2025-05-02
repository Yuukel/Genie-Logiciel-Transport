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

void readStop(char * filePath, unordered_map<string, Arret>* stops);

void readTrips(char* filePath, vector<Ligne>* lignes, unordered_map<string, string>& tripHeadsigns, unordered_map<string, string>& tripRouteIds);

void completeLignes(char* filePath, vector<Ligne>* lignes, unordered_map<string, Arret>* stops, unordered_map<string, string>& tripHeadsigns, unordered_map<string, string>& tripRouteIds);

void nettoyerHorairesEtArrets(vector<Ligne>* lignes);

#endif