#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "case.h"

using namespace std;

class Donjon {
    private:
        // Vecteur de vecteurs de pointeurs de cases
        vector<vector<Case*>> grille;
        int largeur;
        int hauteur;

    public:
        void initialiserGrille(int l, int h) {
            largeur = l;
            hauteur = h;
            
            // initialisation de la grille avec des pointeurs nuls
            grille.resize(largeur, vector<Case*>(hauteur, nullptr));

            // remplir toute la grille de murs au départ
            for (int i = 0; i < largeur; i++) {
                for (int j = 0; j < hauteur; j++) {
                    grille[i][j] = getFactory(MUR)->creerCase();
                };
            };
            
            // à définir...
            genererLabyrinthe();

            // Penser à faire l'entrée et a sortie aussi.
        };
        void genererLabyrinthe() {
            // future implémentation de l'algorithme de creusage pour le labyrinthe
        };
};

#ifdef TEST_DONJON
int main() {
    Donjon donjon;
    donjon.initialiserGrille(20, 20);
    return 0;
}
#endif