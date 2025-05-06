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

string normaliserNom(const string& str);
vector<string> getStopIdByName(const string& stopName, const unordered_map<string, Arret>& stops);
string getCityNameById(const string& cityId, const unordered_map<string, string>& villes);
string getCityIdByName(const string& cityName, const unordered_map<string, string>& villes);
void choixVille(const string& stopName, const unordered_map<string, Arret>& stops);
void entreeUtilisateur(unordered_map<string, Arret>* stops, vector<Ligne>* lignes);
int levenshteinDistance(const string& s1, const string& s2);
void erreurEntree(const string& nameArret, unordered_map<string, Arret>* stops);
int compterMultiOctets(const string& str);
int compterSequencesCouleurs(const std::string& str);

void afficherChemin2(const vector<vector<Noeud>>& chemin, unordered_map<std::string, Arret>* stops, vector<Ligne>* lignes);
#endif