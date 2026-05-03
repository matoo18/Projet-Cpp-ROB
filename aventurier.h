#ifndef AVENTURIER_H
#define AVENTURIER_H

#include <vector>
#include <utility>
#include <set>
#include <chrono>
#include <fstream>

using namespace std;

class Donjon;

class Aventurier{
private:
    int pv;
    int tresors;
    int nbMonstresTues;
    int nbMonstresEgalites;
    int nbMonstresDefaites;
    bool win;
    pair<int,int> position;
    // on va stocker les cases visitées dans un set pour éviter les doublons (c'est comme en Python)
    set<pair<int,int>> cases_visitees;
    int nbPas;
    chrono::steady_clock::time_point debutExploration;
public:
    bool estVivant();
    void afficherStatut(Donjon& donjon);
    int getPV();
    void setPV(int val);
    int getTresors();
    void setTresors(int val);
    pair<int,int> getPosition();
    int getNbCasesVisitees();
    auto getDuration();
    void generer(Donjon& don);
    void deplacer(int x,int y, Donjon& don);
    void resoudreCase(Case* cas);
    void afficherStatistiques();
    void ajouterKill();
    void ajouterEqualite();
    void ajouterDefaite();
    void setWin(bool val);
    bool getWin();
    void sauvegarder(ofstream& fichier);
    void charger(ifstream& fichier);
};

#endif