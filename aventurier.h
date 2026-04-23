#ifndef AVENTURIER_H
#define AVENTURIER_H


#include <vector>
#include <utility>

using namespace std;

class Donjon;

class Aventurier{
    private:
    int pv;
    int tresors;
    pair<int,int> position;

    public:
    bool estVivant();
    void afficherStatut();
    int getPV();
    void setPV(int val);
    int getTresors();
    void setTresors(int val);
    pair<int,int> getPosition();
    void generer(Donjon& don);
    void deplacer(int x,int y, Donjon& don);
    void resoudreCase(Case* cas);
};


#endif