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

string normaliserNom(const string&);
vector<string> getStopIdByName(const string&, const unordered_map<string, Arret>&);
string getCityNameById(const string&, const unordered_map<string, string>&);
string getCityIdByName(const string&, const unordered_map<string, string>&);
void choixVille(const string&, const unordered_map<string, Arret>&);
void entreeUtilisateur(unordered_map<string, Arret>*, vector<Ligne>*);
int levenshteinDistance(const string&, const string&);
void erreurEntree(const string&, unordered_map<string, Arret>*);
int compterMultiOctets(const string&);
int compterSequencesCouleurs(const string&);

void afficherChemin2(const vector<vector<Noeud>>&, unordered_map<string, Arret>*, vector<Ligne>*);
#endif