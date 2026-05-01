#include "case.h"
#include "donjon.h"
#include "aventurier.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <queue>
#include <chrono>

using namespace std;


//#################################################################
//FONC PUBLIQUES
//#################################################################

int Aventurier::getPV(){
    return pv;
}

void Aventurier::setPV(int val){
    pv = val;
}
int Aventurier::getTresors(){
    return tresors;
}

void Aventurier::setTresors(int val){
    tresors = val;
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

void Aventurier::afficherStatut(Donjon& donjon){
    cout << "STATUT:" << endl;
    cout << "Point de Vie (PV): " << pv << endl;
    cout << "TrEsors: " << tresors << endl;
    cout << "Position Courante: (" << position.first << "," << position.second << ")" << endl;
    cout << "Distance sortie: " << donjon.getDistanceSortie(position) << endl;
}


void Aventurier::generer(Donjon& don){
    pv = 10;
    position = {1,1};
    tresors = 0;
    nbMonstresTues=0;
    nbMonstresEgalites= 0;
    nbMonstresDefaites= 0;
    nbPas = 0;
    // on démarre le chrono dans generer comme c'est le début de l'exploration du donjon pour le joueur
    debutExploration = chrono::steady_clock::now();
    don.setCase(1, 1, CaseFactory::creerCase(TypeCase::AVENTURIER));
}


void Aventurier::deplacer(int nx,int ny, Donjon& don){
    pair<int,int> position_courante = position;
    if(nx != position_courante.first || ny != position_courante.second){ //Il faut que la nouvelle poisition soit différente de la position courante.
        if(don.getCase(nx,ny)->getType() != MUR){
            position = {nx,ny};
            cases_visitees.insert(position);
            if(position_courante == make_pair(1,1)){
                don.setCase(position_courante.first, position_courante.second, CaseFactory::creerCase(TypeCase::ENTREE)); // On set la case depuis laquelle l'aventurier s'est déplace à entrée si c'est en 0,0 ou a passage vu que la case sera résolue une fois l'avanturier qui se déplace.
            }
            else{
                don.setCase(position_courante.first, position_courante.second, CaseFactory::creerCase(TypeCase::PASSAGE));
            }
            resoudreCase(don.getCase(nx,ny));
            //Après les actions terminées on set la case résolu au type Aventurier pour l'afficher sur la carte:
            don.setCase(nx,ny, CaseFactory::creerCase(TypeCase::AVENTURIER));
            nbPas++;
        }
    }
}

void Aventurier::ajouterKill(){
    nbMonstresTues++;
}
void Aventurier::ajouterEqualite(){
    nbMonstresEgalites++;
}
void Aventurier::ajouterDefaite(){
    nbMonstresDefaites++;
}
    

void Aventurier::resoudreCase(Case* cas){
    cas->effet(*this);
}

int Aventurier::getNbCasesVisitees(){
    return cases_visitees.size();
}

void Aventurier::afficherStatistiques(){
    auto maintenant = chrono::steady_clock::now();
    auto duree = chrono::duration_cast<chrono::seconds>(maintenant - debutExploration).count();
    
    cout << "\n===============================\n";
    cout << "         STATISTIQUES          \n";
    cout << "===============================\n";
    cout << "Temps d'exploration : " << duree << " secondes\n";
    cout << "Nombre de pas : " << nbPas << "\n";
    cout << "Cases uniques visitees : " << cases_visitees.size() << "\n";
    cout << "Monstres : " << nbMonstresTues << " | " << nbMonstresEgalites << " | " << nbMonstresDefaites << " (V | E | D)\n";
    cout << "===============================\n";
}