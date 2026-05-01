#include "donjon.h"
#include "aventurier.h"
#include <iostream>

#define LARGEUR 31
#define HAUTEUR 11

using namespace std;

TypeAlgoGeneration menuSelection() {
    cout << "=====================================\n";
    cout << "    SELECTION GENERATEUR      \n";
    cout << "=====================================\n";
    cout << "1. Labyrinthe classique - BFS\n";
    cout << "2. Labyrinthe chaotique - PRIM\n";
    cout << "3. Labyrinthe equilibre - Kruskal\n";
    cout << "Choix de l'algorithme (1, 2 ou 3) : ";
    
    char choixAlgo;
    cin >> choixAlgo;

    if (choixAlgo == '2') {
        return PRIM;
    }
    else if (choixAlgo == '3') {
        return KRUSKAL;
    }
    return BFS; 
}

void boucleDeJeu(Aventurier& joueur, Donjon& don) {
    while(joueur.estVivant() == true){
        joueur.afficherStatut(don); 
        don.afficher(); 
        
        int nx = joueur.getPosition().first;
        int ny = joueur.getPosition().second;

        char input;
        std::cin >> input;
        
        switch (input) {
            case 'z': ny -= 1; break;
            case 's': ny += 1; break;
            case 'q': nx -= 1; break;
            case 'd': nx += 1; break;
            case 'p': don.toggleCheminIdeal(); break;
            case 'i': 
                joueur.afficherStatistiques();
                cout << "Appuyez sur une touche et Entrée pour reprendre...";
                char pause;
                cin >> pause;
                continue;
            default:
                continue;
        }

        joueur.deplacer(nx,ny, don);
    }
    cout << "\n=== FIN DE LA PARTIE ===\n";
    joueur.afficherStatistiques();
}

int main(){
    Donjon don;
    Aventurier joueur;

    TypeAlgoGeneration algoChoisi = menuSelection();
    don.generer(LARGEUR, HAUTEUR, algoChoisi);
    joueur.generer(don); 
    boucleDeJeu(joueur, don);

    return 0;
}