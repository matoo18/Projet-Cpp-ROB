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
public:
    void generer(int largeur, int hauteur);
    void afficher();
    vector<pair<int,int>> trouverChemin();
};

#endif