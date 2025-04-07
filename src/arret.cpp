#include "arret.hpp"


arret::arret(std::string id, std::string nom){
    stop_id = id;
    stop_name = nom;
}

arret::arret(){
    stop_id = "0";
    stop_name = "Aucun nom";
}

void arret::print(){
    printf("\nArret : %s\n", stop_name.c_str());
    printf("ID : %s\n", stop_id.c_str());
}

void arret::print_lignes(){
    printf("Lignes qui passent par cet arret :\n");
    for (int i = 0; i < lignes.size(); i++){
        printf("%s\n", lignes[i].c_str());
    }
}

void arret::add_ligne(std::string new_ligne){// ajoute une id a la liste
    lignes.push_back(new_ligne);
}

std::vector<std::string> arret::get_lignes(){
    return lignes;
}
