#ifndef AVENTURIER_H
#define AVENTURIER_H

#include "donjon.h"
#include <vector>
#include <utility>

using namespace std;


class Aventurier{
    private:
    int pv;
    pair<int,int> position;

    public:
    bool estVivant();
    void afficherStatut();
    int getPv();
    pair<int,int> getPosition();
    void generer(Donjon& don);
    void deplacer(int x,int y, Donjon& don);
    void resoudreCase(Case* cas);
};


#endif