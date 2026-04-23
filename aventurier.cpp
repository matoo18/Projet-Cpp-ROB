#include "case.h"
#include "donjon.h"
#include "aventurier.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <queue>

using namespace std;


//#################################################################
//FONC PUBLIQUES
//#################################################################

int Aventurier::getPv(){
    return pv;
}

pair<int,int> Aventurier::getPosition(){
    return position;
}

bool Aventurier::estVivant(){
    if(pv > 0){
        return true;
    }
    else{
        return false;
    }
}

void Aventurier::afficherStatut(){
    cout << "STATUT:" << endl;
    cout << "Point de Vie (PV): " << pv << endl;
    cout << "Position Courante: (" << position.first << "," << position.second << ")" << endl;
}


void Aventurier::generer(Donjon& don){
    pv = 10;
    position = {0,0};
    deplacer(0,0, don);
}


void Aventurier::deplacer(int nx,int ny, Donjon& don){
    pair<int,int> position_courante = position;
    if(nx != position_courante.first || ny != position_courante.second){ //Il faut que la nouvelle poisition soit différente de la position courante.
        if(don.getCase(nx,ny)->getType() != MUR){
            position = {nx,ny};
            if(position_courante == make_pair(0,0)){
                don.setCase(position_courante.first, position_courante.second, CaseFactory::creerCase(TypeCase::ENTREE)); // On set la case depuis la quelle l'aventurier s'est déplace à entrée si c'est en 0,0 ou a passage vu que la case sera résolue une fois l'avanturier qui se déplace.
            }
            else{
                don.setCase(position_courante.first, position_courante.second, CaseFactory::creerCase(TypeCase::PASSAGE));
            }
            resoudreCase(don.getCase(nx,ny));
            //Après les actions terminées on set la case résolu au type Aventurier pour l'afficher sur la carte:
            don.setCase(nx,ny, CaseFactory::creerCase(TypeCase::AVENTURIER));
        }
    }
}


void Aventurier::resoudreCase(Case* cas){
    //Lancer les actions de la case sur l'aventurier, il faudra rajouter une fonction à la classe Case nomée Effet on un truc du genre.
    cout << "" << endl;
}