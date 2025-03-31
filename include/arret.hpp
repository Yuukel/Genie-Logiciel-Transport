#ifndef ARRET
#define ARRET

#include<iostream>

struct horaire{
    unsigned int heure;
    unsigned int minute;
} typedef horaire;

class arret{
    public:
    char* stop_id ; // unique a chaque arret
    int *route_id ; //liste de toutes les lignes qui passe par cette arret 
    horaire heure ;

};

#endif