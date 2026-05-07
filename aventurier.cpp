#include "case.h"
#include "donjon.h"
#include "aventurier.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <queue>
#include <chrono>
#include<string>

const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";

using namespace std;

int Aventurier::getPV(){
    return pv;
}

void Aventurier::setPV(int val){
    (val > pvMax) ? pv = pvMax + (val - pv) : pv = val;
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

bool Aventurier::getWin(){
    return win;
}

void Aventurier::setWin(bool val){
    win = val;
}

void Aventurier::afficherStatut(Donjon& donjon){
    cout << BLUE << "\n===============================\n";
    cout << "         STATUT ACTUEL          \n";
    cout << "===============================\n" << RESET;
    string COLOR = GREEN;
    if (pv < 7) COLOR = YELLOW;
    if (pv < 4) COLOR = RED;
    cout << COLOR << "Point de Vie (PV): " << pv << "/10" << RESET << endl;
    cout << YELLOW << "Tresors: " << tresors << RESET << endl;
    cout << RED << "Position Courante: (" << position.first << "," << position.second << ")" << RESET << endl;
    cout << CYAN << "Distance sortie: " << donjon.getDistanceSortie(position) << RESET << endl;
    cout << "Commandes : z (haut), s (bas), q (gauche), d (droite), i (statistiques), m (sauvegarder et quitter)\n";
    cout << BLUE << "===============================\n" << RESET;
}

void Aventurier::generer(Donjon& don){
    pv = 10;
    pvMax = 10;
    position = {1,1};
    tresors = 0;
    nbMonstresTues=0;
    nbMonstresEgalites= 0;
    nbMonstresDefaites= 0;
    nbPas = 0;
    win = false;
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

auto Aventurier::getDuration(){
    auto maintenant = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::seconds>(maintenant - debutExploration).count();
}

void Aventurier::afficherStatistiques(){
    
    cout << BLUE << "\n===============================\n";
    cout << "         STATISTIQUES          \n";
    cout << "===============================\n" << RESET;
    cout << CYAN << "Temps d'exploration : " << this->getDuration() << " secondes\n" << RESET;
    cout << "Nombre de pas : " << nbPas << "\n";
    cout << MAGENTA << "Cases uniques visitees : " << cases_visitees.size() << "\n" << RESET;
    cout << "Monstres : " << GREEN << nbMonstresTues << RESET << " | " << nbMonstresEgalites << " | " << RED << nbMonstresDefaites << RESET << " (V | E | D)\n";
    cout << "===============================\n";
}

void Aventurier::sauvegarder(ofstream& fichier) {
    // récup stat de base
    fichier << pv << " " << tresors << " " << nbMonstresTues << " " << nbPas << " " << nbMonstresDefaites << " " << nbMonstresEgalites << " " << this->getDuration() << "\n";
    // récup position
    fichier << position.first << " " << position.second << "\n";
    
    // récup cases visitées
    fichier << cases_visitees.size() << "\n";
    for (const auto& case_visitee : cases_visitees) {
        fichier << case_visitee.first << " " << case_visitee.second << " ";
    }
    fichier << "\n";
}

void Aventurier::charger(ifstream& fichier) {
    int duree_sauvegarde;
    fichier >> pv >> tresors >> nbMonstresTues >> nbPas >> nbMonstresDefaites >> nbMonstresEgalites >> duree_sauvegarde; 
    fichier >> position.first >> position.second;

    debutExploration = chrono::steady_clock::now() - chrono::seconds(duree_sauvegarde);

    int nbCases;
    fichier >> nbCases;
    cases_visitees.clear();
    for (int i = 0; i < nbCases; i++) {
        int x, y;
        fichier >> x >> y;
        cases_visitees.insert({x, y});
    }
    win = false;
}