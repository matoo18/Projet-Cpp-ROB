#include "donjon.h"
#include "aventurier.h"

#include <iostream>
#include <fstream>

#define LARGEUR 41
#define HAUTEUR 11

using namespace std;

TypeAlgoGeneration menuSelectionAlgo() {
    cout << "=====================================\n";
    cout << "    SELECTION GENERATEUR      \n";
    cout << "=====================================\n";
    cout << "1. Labyrinthe classique - Recursive Backtracking\n";
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
    return RB; 
}

bool menuChargerPartie() {
    cout << "=====================================\n";
    cout << "    COMMENCER PARTIE      \n";
    cout << "=====================================\n";
    cout << "1. Charger une partie sauvegardee\n";
    cout << "2. Nouvelle partie\n";
    cout << "Choix (1 ou 2) : ";
    
    char choix;
    cin >> choix;

    return choix == '1';
}

void boucleDeJeu(Aventurier& joueur, Donjon& don) {
    bool inGame = true;
    while(joueur.estVivant() == true && joueur.getWin() == false && inGame == true){
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
            case 'i': {
                joueur.afficherStatistiques();
                cout << "Appuyez sur une touche et entree pour reprendre...";
                char pause;
                cin >> pause;
                continue;
            }
            case 'm': {
                ofstream fichier("sauvegarde.txt");
                if (fichier.is_open()) {
                    don.sauvegarder(fichier);
                    joueur.sauvegarder(fichier);
                    fichier.close();
                    cout << "Partie sauvegardee !\n";
                    inGame = false; 
                } 
                else {
                    cout << "Erreur lors de la sauvegarde.\n";
                }
                continue;
            }
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

    if (menuChargerPartie()) {
        ifstream fichier("sauvegarde.txt");
        if (fichier.is_open()) {
            don.charger(fichier);
            joueur.charger(fichier);
            fichier.close();
            cout << "Partie chargee avec succes !\n";
        } 
        else {
            cout << "Aucune sauvegarde trouvee, demarrage d'une nouvelle partie.\n";
            TypeAlgoGeneration algoChoisi = menuSelectionAlgo();
            don.generer(LARGEUR, HAUTEUR, algoChoisi);
            joueur.generer(don); 
        }
    } 
    else {
        TypeAlgoGeneration algoChoisi = menuSelectionAlgo();
        don.generer(LARGEUR, HAUTEUR, algoChoisi);
        joueur.generer(don); 
    }

    boucleDeJeu(joueur, don);

    return 0;
}