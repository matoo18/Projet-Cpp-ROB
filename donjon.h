#ifndef DONJON_H
#define DONJON_H

#define CHEMIN_COULEUR_RESET "\033[0m"
#define CHEMIN_COULEUR_IDEAL "\033[42m"

#include <vector>
#include <utility>
#include <queue>
#include <fstream>

using namespace std;

class Case;

enum TypeDirection {
    Nord,
    Sud,
    Est,
    Ouest
};

enum TypeAlgoGeneration {
    BFS,
    PRIM,
    KRUSKAL
};

class Donjon {
private:
    bool montrerCheminIdeal;
    std::vector<std::vector<Case*>> grille;
    void initialiserLabyrinthe(int hauteur, int largeur);
    void genererLabyrintheBFS(int x, int y, std::vector<std::vector<bool>>& visited);
    void genererLabyrinthePrim(int x, int y);
    void genererLabyrintheKruskal();
    void placerElements();
    void placerEntreeSortie(int largeur, int hauteur);
    vector<pair<int,int>> reconstruireChemin(vector<vector<pair<int,int>>> parent, pair<int,int> depart, pair<int,int>arrivee);


public:
    void generer(int largeur, int hauteur, TypeAlgoGeneration algo = BFS);
    void afficher();
    vector<pair<int,int>> trouverChemin(pair<int,int> depart, pair<int,int>arrivee);
    Case* getCase(int x, int y);
    void setCase(int x, int y, Case* cas);
    void toggleCheminIdeal();
    int getDistanceSortie(pair<int,int> pos_joueur);
    void sauvegarder(ofstream& fichier);
    void charger(ifstream& fichier);
};

#endif