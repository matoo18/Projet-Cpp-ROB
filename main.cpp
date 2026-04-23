#include "donjon.h"
#include "aventurier.h"
#include <iostream>

using namespace std;


int main(){
    Donjon don;
    Aventurier joueur;

    don.generer(21,11);//On génére le donjon.
    joueur.deplacer(0,0, don); //On déplace le joueur dans la case 0,0


    while(joueur.estVivant() == true){ //Boucle de jeu
        don.afficher(); //On affiche le donjon*
        joueur.afficherStatut(); //On affiche les infos du joueur;
        
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
            default:
                std::cout << "Tu fais quoi wsh!\n";
                continue;
        }

        //On déplace le joueur

        joueur.deplacer(nx,ny, don);

    }
    return 0;
}