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

bool compare_horaires(const horaire& h1, const horaire& h2) {
    if (h1.heure > h2.heure) {
        return true;
    } else if (h1.heure == h2.heure && h1.minute > h2.minute) {
        return true;
    }
    return false;
}

#endif