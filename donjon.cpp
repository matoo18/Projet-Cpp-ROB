#include "donjon.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <queue>

using namespace std;


void Donjon::initialiserLabyrinthe(int hauteur, int largeur){
    grille.resize(hauteur);

    for (int y = 0; y < hauteur; y++) {
        grille[y].resize(largeur);
    }

    for (int y = 0; y < hauteur; y++){
        for (int x = 0; x < largeur; x++){
            grille[y][x] = CaseFactory::creerCase(TypeCase::MUR);
        }
    }
}

void Donjon::genererLabyrinthe(int x, int y, vector<vector<bool>>& visited){
    visited[y][x] = true; //Case visitée

    vector<TypeDirection> dirs = {Nord, Sud, Est, Ouest};

    static mt19937 rng(std::random_device{}());
    shuffle(dirs.begin(), dirs.end(), rng);

    for (const TypeDirection& dir : dirs)
    { //comme dans python on peut loop dans les élements d'un array, c'est pas très utile comme d'hab juste cool a implementer
        int nx = x;
        int ny = y; 

        if (dir == Nord) ny = ny - 2;
        if (dir == Sud)  ny = ny + 2;
        if (dir == Ouest) nx = nx - 2;
        if (dir == Est)   nx = nx + 2;

        //nx = clamp(nx,0,grille[0].size());

        if(nx < grille[0].size() && nx >= 0 &&
           ny < grille.size() && ny >= 0 &&
           visited[ny][nx] == false)
        {
            int my = (ny + y) / 2;
            int mx = (nx + x) / 2;

            grille[my][mx] = CaseFactory::creerCase(TypeCase::PASSAGE); 
            grille[ny][nx] = CaseFactory::creerCase(TypeCase::PASSAGE);
            // Création passage entre cases (x,y) et (nx,ny)

            genererLabyrinthe(nx, ny, visited);
        }
    }
}

void Donjon::placerEntreeSortie(int largeur, int hauteur){
    grille[0][0] = CaseFactory::creerCase(TypeCase::ENTREE); //Entrée
    grille[hauteur-1][largeur-1] = CaseFactory::creerCase(TypeCase::SORTIE); //Sortie
}
void Donjon::placerElements(){

}


//#################################################################
//FONC PUBLIQUES
//#################################################################

void Donjon::generer(int largeur, int hauteur){   
    initialiserLabyrinthe(hauteur, largeur);

    // Vecteur pour vérifier si les cases sont visitées ou pas, utilisé que pendant la génération.
    vector<vector<bool>> visited(hauteur,vector<bool>(largeur, false)); 
    genererLabyrinthe(0,0, visited);
    placerEntreeSortie(largeur, hauteur);
    //placerElements();
}

void Donjon::afficher(){
    //Print bordure du haut
    cout << "+";
    for (int i = 0; i < grille[0].size(); i++){
        cout << "-";
    }
    cout << "+";

    cout << "\n";

    for(int y = 0; y < grille.size(); y++){
        cout << "|";

        for(int x = 0; x < grille[0].size(); x++){
            grille[y][x]->afficher();
        }

        cout << "|";
        cout << "\n";
    }

    //Print bordure du bas
    cout << "+";
    for (int i = 0; i < grille[0].size(); i++){
        cout << "-";
    }
    cout << "+";
    cout << "\n";
}