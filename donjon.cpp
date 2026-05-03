#include "donjon.h"
#include "case.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <queue>
#include <fstream>

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

void Donjon::genererLabyrintheRB(int x, int y, vector<vector<bool>>& visited){
    visited[y][x] = true; //Case visitée

    vector<TypeDirection> dirs = {Nord, Sud, Est, Ouest};

    static mt19937 rng(random_device{}());
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

            genererLabyrintheRB(nx, ny, visited);
        }
    }
}

void Donjon::genererLabyrinthePrim(int x, int y) {
    // contient toutes les cases murs adjacentes à une case déjà passage
    vector<pair<int, int>> frontieres; 
    mt19937 rng(random_device{}());

    // case de départ transformée en passage car on est déjà dessus
    grille[y][x] = CaseFactory::creerCase(TypeCase::PASSAGE);

    // on regarde dans les 4 directions pour ajouter les murs adjacents à la case de départ dans la liste des frontières
    // Le [&] permet d'accéder à toutes les variables de la méthode genérerLabyrinthePrim
    auto ajouterFrontieres = [&](int x, int y) {
        vector<pair<int, int>> dirs = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}}; // N, S, O, E
        for (auto d : dirs) {
            int nx = x + d.first;
            int ny = y + d.second;
            // si c'est dans les limites de la map et que c'est un mur, on l'ajoute à la liste des frontières
            if (nx > 0 && nx < grille[0].size() - 1 && ny > 0 && ny < grille.size() - 1) {
                if (grille[ny][nx]->getType() == MUR) {
                    frontieres.push_back({nx, ny});
                }
            }
        }
    };
    ajouterFrontieres(x, y);

    while (!frontieres.empty()) {
        // tant que des frontières existent, on en choisit une au hasard
        uniform_int_distribution<int> distFront(0, frontieres.size() - 1);
        int index = distFront(rng);
        pair<int, int> current = frontieres[index];

        // pour supprimer la frontière choisie de la liste, on la remplace par la dernière parce qu'on se fiche de l'ordre, 
        // et on pop_back pour supprimer la dernière qui est maintenant un doublon
        frontieres[index] = frontieres.back();
        frontieres.pop_back();

        // si la frontière choisie est déjà un passage, on ne fait rien
        if (grille[current.second][current.first]->getType() == PASSAGE) continue;

        // on cherche les voisins de la frontière qui sont des passages, il doit y en avoir au moins un sinon on ne serait pas arrivé à cette frontière
        // ensuite on choisira au hasard un de ces voisins pour connecter la frontière à ce voisin et créer un nouveau passage dans le labyrinthe
        vector<pair<int, int>> dirs = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};
        vector<pair<int, int>> voisinsPassages;
        for (auto d : dirs) { // on boucle dans les 4 directions pour trouver les voisins passages de la frontière courante
            int nx = current.first + d.first; 
            int ny = current.second + d.second;
            if (nx > 0 && nx < grille[0].size() - 1 && ny > 0 && ny < grille.size() - 1) {
                if (grille[ny][nx]->getType() == PASSAGE) {
                    voisinsPassages.push_back({nx, ny});
                }
            }
        }

        if (!voisinsPassages.empty()) {
            // choisir un voisin passage au hasard auquel se connecter
            uniform_int_distribution<int> randomVoisins(0, voisinsPassages.size() - 1);
            pair<int, int> voisinChoisi = voisinsPassages[randomVoisins(rng)];

            // la frontière choisie devient un passage
            grille[current.second][current.first] = CaseFactory::creerCase(TypeCase::PASSAGE);
            // on calcule les coordonnées du mur entre la frontière choisie et le voisin passage choisi juste au dessus
            int mx = (current.first + voisinChoisi.first) / 2;
            int my = (current.second + voisinChoisi.second) / 2;
            // ce mur devient aussi un passage pour connecter la frontière au voisin
            grille[my][mx] = CaseFactory::creerCase(TypeCase::PASSAGE);

            // ajout des nouvelles frontières de la case passage qui vient d'être créée entre les 2
            ajouterFrontieres(current.first, current.second);
        }
    }
}

void Donjon::genererLabyrintheKruskal() {
    int largeur = grille[0].size();
    int hauteur = grille.size();

    // structure pour représenter un mur entre 2 cases de passage, avec les coordonnées du mur lui même et des deux cellules qu'il sépare
    struct MurACasser {
        int x, y;
        int c1_x, c1_y;
        int c2_x, c2_y;
    };

    vector<MurACasser> murs;

    // transformer toutes les cases impaires en passage et on remplit la liste des murs potentiels à casser entre ces passages
    // en gros on boucle sur toute la grille et on transforme les cases impaires en passage
    for (int y = 1; y < hauteur - 1; y += 2) {
        for (int x = 1; x < largeur - 1; x += 2) {
            grille[y][x] = CaseFactory::creerCase(TypeCase::PASSAGE);
            
            if (x + 2 < largeur - 1) { // si on est encore dans la map
                // on ajoute le mur entre la case (x,y) et la case (x+2,y) dans la liste des murs à casser potentiels
                // les coordonnées du mur sont (x+1,y) et il sépare les cellules (x,y) et (x+2,y)
                murs.push_back({x + 1, y, x, y, x + 2, y});
            }
            // same pour le mur entre (x,y) et (x,y+2)
            if (y + 2 < hauteur - 1) {
                murs.push_back({x, y + 1, x, y, x, y + 2});
            }
        }
    }
    // on mélange la liste des murs à casser pour que l'algorithme de Kruskal soit aléatoire à chaque génération
    mt19937 rng(random_device{}());
    // on mélange l'intégralité du vecteur des murs
    shuffle(murs.begin(), murs.end(), rng);

    // on crée un vecteur parent de taille largeur*hauteur pour avoir toute la map
    vector<int> parent(largeur * hauteur);
    for (int i = 0; i < parent.size(); i++) {
        // chaque cellule est son propre parent au début
        parent[i] = i; 
    }

    // encore [&] pour accéder à toutes les variables de la méthode genererLabyrintheKruskal
    auto find_set = [&](int v) {
        // on check si la cellule v est son propre parent où si elle fait partir d'un groupe de cases déjà connectées
        // si elle fait partie d'un groupe, on remonte jusqu'à trouver le parent de ce groupe qui est la racine
        int root = v;
        while (root != parent[root]) {
            root = parent[root];
        }
        // pour éviter d'avoir à faire la longue remontée à chaque fois, on dit à toutes les cases rencontrées pendant la remontée
        // que leur boss est root comme ça, à la prochaine itération find_set tombe direct sur le boss et voit qu'il n'y a personne au dessus.
        int curr = v;
        while (curr != root) {
            int nxt = parent[curr];
            parent[curr] = root;
            curr = nxt;
        }
        return root;
    };

    // permet de fusionner 2 groupes de cases connectées 
    auto union_sets = [&](int a, int b) {
        int rootA = find_set(a); // on convoque le boss de la cellule a
        int rootB = find_set(b); // et celui de la cellule b
        if (rootA != rootB) { // et si c'est pas les mêmes, on les fusionne en disant que le boss de rootB devient rootA
            // si c'était les mêmes boss ça veut dire que les cases a et b font déjà partie du même groupe de cases connectées, 
            // et donc on ne veut pas les connecter entre elles pour éviter de créer des cycles dans le labyrinthe
            parent[rootB] = rootA;
        }
    };

    // jusqu'ici, on a juste crée les fonctions pour changer les boss de certains groupes de cases connectées mais y'a tjr les murs entre elles.
    for (const auto& mur : murs) {
        // on boucle sur tous les murs à casser potentiels dans un ordre aléatoire, et pour chacun on regarde les 2 cases de passage qu'il sépare
        // on donne un id à chaque cellule pour pouvoir utiliser la structure de Kruskal.
        int id1 = mur.c1_y * largeur + mur.c1_x;
        int id2 = mur.c2_y * largeur + mur.c2_x;

        // si leur boss est différent, ça veut dire que les 2 cases ne font pas partie du même groupe de cases connectées, donc on peut casser le mur entre elles sans créer de cycle
        if (find_set(id1) != find_set(id2)) {
            // on casse le mur en transformant la case du mur en passage
            grille[mur.y][mur.x] = CaseFactory::creerCase(TypeCase::PASSAGE);
            // et on fusionne les 2 groupes de cases connectées en changeant le boss de l'un des groupes pour qu'il devienne le même que celui de l'autre groupe
            union_sets(id1, id2);
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
    if (algo == RB) {
        vector<vector<bool>> visited(hauteur,vector<bool>(largeur, false)); 
        genererLabyrintheRB(1,1, visited);
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

void Donjon::sauvegarder(ofstream& fichier) {
    int hauteur = grille.size();
    int largeur = grille[0].size();
    
    // On écrit les dimensions
    fichier << largeur << " " << hauteur << "\n";
    
    // On parcourt la grille et on écrit la valeur Enum de chaque type de case
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            fichier << grille[y][x]->getType() << " ";
        }
        fichier << "\n";
    }
}

void Donjon::charger(ifstream& fichier) {
    // On récup les dimensions
    int largeur, hauteur;
    fichier >> largeur >> hauteur;
    
    initialiserLabyrinthe(hauteur, largeur);
    
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            int typeInt;
            fichier >> typeInt;
            TypeCase type = static_cast<TypeCase>(typeInt);
            grille[y][x] = CaseFactory::creerCase(type);
        }
    }
}