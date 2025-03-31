#ifndef TRAJET
#define TRAJET

#define N 3

#include "arret.hpp"

using namespace std;

class trajet{
    public:
    int route_id;
    int trip_id;
    char *trip_headsign; //nom du terminus
    int * stop; // une liste des stop_id liés à la class arret
    int taille; //taille de la liste stop
};

#endif