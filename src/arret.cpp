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
        printf("%s\n", lignes[i]->id_ligne.c_str());
    }
}

void arret::add_ligne(std::shared_ptr<ligne> new_ligne){
    lignes.push_back(new_ligne);
}

std::vector<std::shared_ptr<ligne>> arret::get_lignes(){
    return lignes;
}