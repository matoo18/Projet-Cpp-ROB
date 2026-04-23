#ifndef DONJON_H
#define DONJON_H

#include <vector>
#include "case.h"
#include <utility>
#include <queue>

using namespace std;

enum TypeDirection {
    Nord,
    Sud,
    Est,
    Ouest
};

class Donjon {
private:
    std::vector<std::vector<Case*>> grille;

    void initialiserLabyrinthe(int hauteur, int largeur);
    void genererLabyrinthe(int x, int y, std::vector<std::vector<bool>>& visited);
    void placerElements();
    void placerEntreeSortie(int largeur, int hauteur);
    vector<pair<int,int>> reconstruireChemin(vector<vector<pair<int,int>>> parent, pair<int,int> depart, pair<int,int>arrivee);

public:
    void generer(int largeur, int hauteur);
    void afficher();
    vector<pair<int,int>> trouverChemin(pair<int,int> depart, pair<int,int>arrivee);
    Case* getCase(int x, int y);
    void setCase(int x, int y, Case* cas);
};

#endif