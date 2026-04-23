#include "donjon.h"
#include "case.h"

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
    //RNG
    mt19937 rng(std::random_device{}());
    uniform_int_distribution<int> dist(0, 100);


    for(int y=0; y < grille.size();y++){
        for(int x = 0; x < grille[y].size();x++){
            TypeCase type = PASSAGE;
            if(grille[y][x]->getType() == PASSAGE){
                int r = dist(rng);
                if(r < 13){
                    type = PIEGE; 
                }
                if(r < 10){
                    type = MONSTRE;
                }
                if(r < 5){
                    type = TRESOR;
                }
                grille[y][x] = CaseFactory::creerCase(type);
            }
        }
    }
}

vector<pair<int,int>> Donjon::reconstruireChemin(vector<vector<pair<int,int>>> parent, pair<int,int> depart, pair<int,int>arrivee){
    vector<pair<int,int>> chemin;
    pair<int,int>courant = arrivee;
    
    while(courant != depart){
        chemin.push_back(courant);
        courant = parent[courant.second][courant.first];
    }

    chemin.push_back(depart);
    return chemin;
}


//#################################################################
//FONC PUBLIQUES
//#################################################################

void Donjon::generer(int largeur, int hauteur){
    montrerCheminIdeal = false;
    initialiserLabyrinthe(hauteur, largeur);

    // Vecteur pour vérifier si les cases sont visitées ou pas, utilisé que pendant la génération.
    vector<vector<bool>> visited(hauteur,vector<bool>(largeur, false)); 
    genererLabyrinthe(0,0, visited);
    placerEntreeSortie(largeur, hauteur);
    placerElements();
}

void Donjon::afficher(){
    //Check pour voir si on affiche le path ideal ou pas:
    vector<vector<bool>> dansCheminIdeal(grille.size(), vector<bool>(grille[0].size(), false)); //Vecteur copie bool de la grille pour voir si une case appartient au chemin ideal ou pas, il serait peut-être plus utile de le mettre en paramètre de classe.
    if(montrerCheminIdeal == true){
        vector<pair<int,int>> chemin; //Variable pour le chemin ideal
        pair<int,int> pos_Aventurier = {-1,-1};
        for (int y = 0; y < grille.size(); y++) { //Pour trouver la position du joueur on scan, on aurait pu aussi faire passer la position du joueur en argument mais flemme.
            for (int x = 0; x < grille[y].size(); x++) {
                if (grille[y][x]->getType() == AVENTURIER) {
                    pos_Aventurier = {x,y};
                }
            }
        } 
        if(pos_Aventurier != make_pair(-1,-1)){
            chemin = trouverChemin(pos_Aventurier, {grille[0].size()-1, grille.size() -1});
        }else{
            cout << "ERREUR DU CHEMIN IDEAL" << endl; //Si par malheur on ne trouve pas le joueur sur la grille, on n'affiche pas le chemin ideal.
            toggleCheminIdeal();
        }
        for(const pair<int,int>& coord : chemin){
            dansCheminIdeal[coord.second][coord.first] = true;
        }
    }
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
            if(montrerCheminIdeal == true && dansCheminIdeal[y][x] == true){ //Les cases seront de couleur vertes si elles sont sur le chemin critique
                cout << CHEMIN_COULEUR_IDEAL; //On met le print en couleur verte
                grille[y][x]->afficher();
                cout << CHEMIN_COULEUR_RESET;//On Reset
                cout.flush();
            }else{
                grille[y][x]->afficher();
            }
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


vector<pair<int,int>> Donjon::trouverChemin(pair<int,int> depart, pair<int,int>arrivee){
    queue<pair<int,int>> file;
    vector<vector<bool>> visitee(grille.size(), vector<bool>(grille[0].size(), false));
    vector<vector<pair<int,int>>> parent(grille.size(), vector<pair<int,int>>(grille[0].size(), {-1,-1})); //On peut utiliser make_pair(-1,-1) ou {-1,-1} pour definifir une pair

    file.push(depart); //On met la case de départ dans la file
    visitee[depart.second][depart.first] = true;


    vector<TypeDirection> dirs = {Nord, Sud, Est, Ouest}; //Pour regarder les voisins

    while(file.size() > 0){
        pair<int,int> courant = file.front(); //On prend le premier element de la file
        file.pop(); //On le delete de la file après

        if(courant == arrivee){
            return reconstruireChemin(parent, depart, arrivee);
        }

        for (const TypeDirection& dir : dirs){
            int x = courant.first;
            int y = courant.second;

            if (dir == Nord) y = y - 1;
            if (dir == Sud)  y = y + 1;
            if (dir == Ouest) x = x - 1;
            if (dir == Est)   x = x + 1;

            if(x < grille[0].size() && x >= 0 && y < grille.size() && y >= 0){ //On check si le voisin est dans les bornes.
                if(visitee[y][x] == false && grille[y][x]->getType() != MUR){ //On Check si le voisin a déjà été visité et si il est de type MUR ou pas

                    visitee[y][x] = true;
                    parent[y][x] = courant;
                    file.push({x,y});
                }
            }
        }
    }

    return vector<pair<int,int>>(0); //Return de vecteur vide du même type que la fonc.

}






Case* Donjon::getCase(int x, int y){
    if(x < grille[0].size() && x >= 0 && y < grille.size() && y >= 0){
        return grille[y][x];
    }
    else{
        return CaseFactory::creerCase(TypeCase::MUR);
    }
}

void Donjon::setCase(int x, int y, Case* cas){
    grille[y][x] = cas;
}


void Donjon::toggleCheminIdeal(){
    if(montrerCheminIdeal == false){
        montrerCheminIdeal = true;
    }
    else{
        montrerCheminIdeal = false;
    }
}