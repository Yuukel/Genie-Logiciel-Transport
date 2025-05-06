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

void readStop(char *, unordered_map<string, Arret>*);

void readTrips(char*, vector<Ligne>*, unordered_map<string, string>&, unordered_map<string, string>&);

void completeLignes(char*, vector<Ligne>*, unordered_map<string, Arret>*, unordered_map<string, string>&, unordered_map<string, string>&);

void nettoyerHorairesEtArrets(vector<Ligne>*);

#endif