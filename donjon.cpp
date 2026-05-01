#include "donjon.h"
#include "case.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <queue>
#include <iostream>

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

void Donjon::genererLabyrintheBFS(int x, int y, vector<vector<bool>>& visited){
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

            genererLabyrintheBFS(nx, ny, visited);
        }
    }
}

void Donjon::placerEntreeSortie(int largeur, int hauteur){
    grille[1][1] = CaseFactory::creerCase(TypeCase::ENTREE); //Entrée
    grille[hauteur-2][largeur-2] = CaseFactory::creerCase(TypeCase::SORTIE); //Sortie
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

void Donjon::generer(int largeur, int hauteur, TypeAlgoGeneration algo){
    montrerCheminIdeal = false;
    initialiserLabyrinthe(hauteur, largeur);

    // Vecteur pour vérifier si les cases sont visitées ou pas, utilisé que pendant la génération.
    if (algo == BFS) {
        vector<vector<bool>> visited(hauteur,vector<bool>(largeur, false)); 
        genererLabyrintheBFS(1,1, visited);
    } 
    else if (algo == PRIM) {
        genererLabyrinthePrim(1, 1);
    }
    else if (algo == KRUSKAL) {
        genererLabyrintheKruskal();
        }
    placerEntreeSortie(largeur, hauteur);
    placerElements();
}

void Donjon::genererLabyrinthePrim(int x, int y) {
    // Liste des cases "murs" qui sont adjacentes au labyrinthe en cours de construction
    vector<pair<int, int>> frontieres; 
    mt19937 rng(random_device{}());

    // 1. Marquer la case de départ comme passage
    grille[y][x] = CaseFactory::creerCase(TypeCase::PASSAGE);

    // Fonction lambda (interne) pour ajouter les voisins distants de 2 cases aux frontières
    auto ajouterFrontieres = [&](int x, int y) {
        vector<pair<int, int>> dirs = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}}; // N, S, O, E
        for (auto d : dirs) {
            int nx = x + d.first;
            int ny = y + d.second;
            // Vérification des bornes (on laisse la bordure extérieure intacte)
            if (nx > 0 && nx < grille[0].size() - 1 && ny > 0 && ny < grille.size() - 1) {
                if (grille[ny][nx]->getType() == MUR) {
                    frontieres.push_back({nx, ny});
                }
            }
        }
    };

    // On ajoute les frontières de la case de départ
    ajouterFrontieres(x, y);

    while (!frontieres.empty()) {
        // Tirer une frontière au hasard
        uniform_int_distribution<int> distFront(0, frontieres.size() - 1);
        int index = distFront(rng);
        pair<int, int> current = frontieres[index];

        // Retirer cette frontière (on la remplace par la dernière puis on pop pour optimiser)
        frontieres[index] = frontieres.back();
        frontieres.pop_back();

        // Si la case a déjà été transformée en passage entre-temps, on ignore
        if (grille[current.second][current.first]->getType() == PASSAGE) continue;

        // Chercher tous les voisins (distance 2) de cette frontière qui sont DÉJÀ des passages
        vector<pair<int, int>> dirs = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};
        vector<pair<int, int>> voisinsPassages;
        for (auto d : dirs) {
            int nx = current.first + d.first;
            int ny = current.second + d.second;
            if (nx > 0 && nx < grille[0].size() - 1 && ny > 0 && ny < grille.size() - 1) {
                if (grille[ny][nx]->getType() == PASSAGE) {
                    voisinsPassages.push_back({nx, ny});
                }
            }
        }

        if (!voisinsPassages.empty()) {
            // Choisir un voisin passage au hasard auquel se connecter
            uniform_int_distribution<int> distVoisins(0, voisinsPassages.size() - 1);
            pair<int, int> voisinChoisi = voisinsPassages[distVoisins(rng)];

            // Creuser la frontière actuelle ET le mur intermédiaire entre les deux
            grille[current.second][current.first] = CaseFactory::creerCase(TypeCase::PASSAGE);
            int mx = (current.first + voisinChoisi.first) / 2;
            int my = (current.second + voisinChoisi.second) / 2;
            grille[my][mx] = CaseFactory::creerCase(TypeCase::PASSAGE);

            // Ajouter les nouvelles frontières découvertes depuis cette nouvelle case
            ajouterFrontieres(current.first, current.second);
        }
    }
}

void Donjon::genererLabyrintheKruskal() {
    int largeur = grille[0].size();
    int hauteur = grille.size();

    // 1. Structure pour représenter un mur entre deux cellules
    struct MurA_Casser {
        int x, y;         // Coordonnées du mur lui-même
        int c1_x, c1_y;   // Cellule de passage 1
        int c2_x, c2_y;   // Cellule de passage 2
    };

    vector<MurA_Casser> murs;

    // 2. Transformer toutes les cellules (impaires) en passage et lister les murs entre elles
    for (int y = 1; y < hauteur - 1; y += 2) {
        for (int x = 1; x < largeur - 1; x += 2) {
            grille[y][x] = CaseFactory::creerCase(TypeCase::PASSAGE);

            // Mur de droite
            if (x + 2 < largeur - 1) {
                murs.push_back({x + 1, y, x, y, x + 2, y});
            }
            // Mur du bas
            if (y + 2 < hauteur - 1) {
                murs.push_back({x, y + 1, x, y, x, y + 2});
            }
        }
    }

    // 3. Mélanger tous les murs de façon aléatoire
    mt19937 rng(std::random_device{}());
    shuffle(murs.begin(), murs.end(), rng);

    // 4. Structure Union-Find pour vérifier si deux cellules sont déjà reliées
    vector<int> parent(largeur * hauteur);
    for (int i = 0; i < parent.size(); i++) {
        parent[i] = i; // Chaque cellule est initialement son propre chef
    }

    // Fonction Find avec compression de chemin
    auto find_set = [&](int v) {
        int root = v;
        while (root != parent[root]) {
            root = parent[root];
        }
        int curr = v;
        while (curr != root) {
            int nxt = parent[curr];
            parent[curr] = root;
            curr = nxt;
        }
        return root;
    };

    // Fonction Union
    auto union_sets = [&](int a, int b) {
        int rootA = find_set(a);
        int rootB = find_set(b);
        if (rootA != rootB) {
            parent[rootB] = rootA;
        }
    };

    // 5. Casser les murs selon Kruskal
    for (const auto& mur : murs) {
        // On donne un ID unique à chaque cellule (y * largeur + x)
        int id1 = mur.c1_y * largeur + mur.c1_x;
        int id2 = mur.c2_y * largeur + mur.c2_x;

        // Si elles ne sont pas déjà reliées...
        if (find_set(id1) != find_set(id2)) {
            // ...on casse le mur et on fusionne leurs zones !
            grille[mur.y][mur.x] = CaseFactory::creerCase(TypeCase::PASSAGE);
            union_sets(id1, id2);
        }
    }
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
            chemin = trouverChemin(pos_Aventurier, {grille[0].size()-2, grille.size() -2});
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
                cout << *grille[y][x];
                cout << CHEMIN_COULEUR_RESET;//On Reset
                cout.flush();
            }else{
                cout << *grille[y][x];
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

int Donjon::getDistanceSortie(pair<int,int> pos_joueur){
    // On cible la sortie (largeur-2, hauteur-2)
    pair<int,int> sortie = {grille[0].size() - 2, grille.size() - 2};
    
    // utilise bfs pour trouver le chemin le plus court entre la position du joueur et la sortie
    vector<pair<int,int>> chemin = trouverChemin(pos_joueur, sortie);
    
    if (chemin.size() > 0) {
        return chemin.size() - 1; // -1 car le tableau contient la case de départ
    }
    return 0; // au cas où aucun chemin ne serait trouvé, ce qui ne devrait pas arriver...
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