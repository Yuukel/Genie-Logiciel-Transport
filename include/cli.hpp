#ifndef CLI_HPP
#define CLI_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <limits>

#include "dijkstra.hpp"
#include "trajet.hpp"

#define ROUGE "\033[31m"
#define VERT "\033[32m"
#define JAUNE "\033[33m"
#define BLEU "\033[34m"
#define ORANGE "\033[38;5;214m"
#define BLANC "\033[37m"
#define BOLD "\033[1m"
#define RESET "\033[0m"
#define SOULIGNE "\033[4m"

vector<string> getStopIdByName(const string& stopName, const unordered_map<string, Arret>& stops);
void entreeUtilisateur(unordered_map<string, Arret>* stops, vector<Ligne>* lignes);
int levenshteinDistance(const string& s1, const string& s2);
void erreurEntree(const string& nameArret, unordered_map<string, Arret>* stops);
int compterMultiOctets(const string& str);
int compterSequencesCouleurs(const std::string& str);

void afficherChemin(const std::vector<Noeud>& chemin, const std::vector<int>& indicesChangement ,unordered_map<std::string, Arret>* stops, vector<Ligne>* lignes);

void afficherChemin2(const vector<vector<Noeud>>& chemin, unordered_map<std::string, Arret>* stops, vector<Ligne>* lignes);
#endif