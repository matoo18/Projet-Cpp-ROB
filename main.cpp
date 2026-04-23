#include "donjon.h"
#include "aventurier.h"
#include <iostream>

#define LARGEUR 11
#define HAUTEUR 11
using namespace std;

void clearScreen() { //Fonction simple pour clear la console car c'est pas ouf de garder les anciennes impressions.
    cout << "\033[2J\033[1;1H";
}


int main(){
    Donjon don;
    Aventurier joueur;


    //Initialisation Donjon + Joueur
    don.generer(LARGEUR,HAUTEUR);//On génére le donjon.
    joueur.generer(don); //On génére le joueur dans la donjon


    while(joueur.estVivant() == true){ //Boucle de jeu
        joueur.afficherStatut(); //On affiche les infos du joueur;
        don.afficher(); //On affiche le donjon
        
        int nx = joueur.getPosition().first;
        int ny = joueur.getPosition().second;

        //On detecte l'input du joueur pour le déplacement
        char input;
        std::cin >> input;
        
        switch (input) {
            case 'z':
                ny -= 1;
                break;
            case 's':
                ny += 1;
                break;
            case 'q':
                nx -= 1;
                break;
            case 'd':
                nx += 1;
                break;
            case 'p':
                don.toggleCheminIdeal();
                break;
            default:
                cout << "Tu fais quoi wsh!\n";
                continue;
        }

        //On déplace le joueur
        joueur.deplacer(nx,ny, don);

    }
    return 0;
}