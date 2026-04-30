#include "case.h"
#include "aventurier.h"

#include <random>
#include <string>
#include <iostream>

using namespace std;

char Mur::afficher() const { return '#'; }
char Passage::afficher() const { return ' '; }
char Tresor::afficher() const { return '+'; }
char Monstre::afficher() const { return 'M'; }
char Piege::afficher() const { return 'T'; }
char Entree::afficher() const { return 'e'; }
char Sortie::afficher() const { return 's'; }
char CaseAventurier::afficher() const { return '@';}

// surcharge de << pour afficher les cases
ostream& operator<<(ostream& os, const Case& c) {
    os << c.afficher();
    return os;
}

Case* CaseFactory::creerCase(TypeCase type_of_case) {
    if (type_of_case == MUR) return new Mur();
    if (type_of_case == PASSAGE) return new Passage();
    if (type_of_case == TRESOR) return new Tresor();
    if (type_of_case == MONSTRE) return new Monstre();
    if (type_of_case == PIEGE) return new Piege();
    if (type_of_case == ENTREE) return new Entree();
    if (type_of_case == SORTIE) return new Sortie();
    if (type_of_case == AVENTURIER) return new CaseAventurier();
    return nullptr;
}



void Monstre::effet(Aventurier& joueur) {
    cout << "\n=========================================\n";
    cout << " UN MONSTRE APARAIT : RIEN NE VA PLUS !" << endl;
    cout << "=========================================\n";

    // init aléatoire
    static mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(1, 13); // 1 = as, 11=valet, 12=dame, 13=roi

    // fonction pour tirer une carte 
    auto tirerCarte = [&](int& score, int& nbAs, bool cache = false) {
        int carte = dist(rng);
        int valeur = (carte > 10) ? 10 : carte; // si la valeur de la carte est une tete alors on la borne à 10 sinon valeur de la carte
        
        // l'as est un peu différent car vaut 1 ou 11.
        if (carte == 1) {
            nbAs++;
            valeur = 11;
        }
        
        score += valeur;
        
        // si on dépasse 21 et qu'on a une as compté comme 11, on le passe à 1. 
        if (score > 21 && nbAs > 0) {
            score -= 10;
            nbAs--;
        }
        
        if (!cache) {
            cout << "[";
            if (carte == 1) cout << "As";
            else if (carte == 11) cout << "Valet";
            else if (carte == 12) cout << "Dame";
            else if (carte == 13) cout << "Roi";
            else cout << valeur;
            cout << "] ";
        }
        return valeur;
    };

    int aventurierScore = 0, monstreScore = 0;
    int asJoueur = 0, asMonstre = 0;

    // MONSTRE
    cout << "\nCartes du Monstre : \n";
    cout << "[?] ";
    
    // tirage carte cachée
    int carteCachee = dist(rng);
    int valeurCachee = (carteCachee > 10) ? 10 : carteCachee;

    if (carteCachee == 1) {
        asMonstre++; 
        valeurCachee = 11; 
    }
    
    // tirage carte visible
    tirerCarte(monstreScore, asMonstre, false);
    cout << " -> Score Monstre : " << monstreScore << "\n";

    // à mettre après le tirage de la carte visible pour que le score du monstre puisse être affiché avant de révéler la carte cachée
    monstreScore += valeurCachee;

    // JOUEUR 
    cout << "\nVos cartes : \n";
    tirerCarte(aventurierScore, asJoueur, false);
    tirerCarte(aventurierScore, asJoueur, false);
    cout << " -> Votre score : " << aventurierScore << "\n";


    // résolution du tour du joueur
    bool enJeu = true;

    while (enJeu && aventurierScore <= 21) {
        cout << "\n--- Votre tour ---\n";
        cout << "\nAction ? (1: Tirer, 2: Rester, 3: Cashout/Fuir (Chances de perdre 1 PV)) : ";
        // type char car avec int si on met autre chose qu'un int ça fait planter le cin.
        char choix;
        cin >> choix;

        if (choix == '1') {
            cout << "Vous tirez : ";
            tirerCarte(aventurierScore, asJoueur, false);
            
            if (aventurierScore > 21) {
                cout << "Vous avez sauté.\n";
                enJeu = false;
            }
            cout << " -> Votre score : " << aventurierScore << "\n";
        } 
        else if (choix == '2') {
            enJeu = false;
        } 
        else if (choix == '3') {
            uniform_int_distribution<int> chanceFuite(0, 1);
            
            if (chanceFuite(rng) == 0) {
                cout << "Vous tentez de fuir... mais vous trebuchez lourdement sur une racine ! Le monstre vous rattrape et vous griffe (-1 PV).\n";
                joueur.setPV(joueur.getPV() - 1); 
            } else {
                cout << "Vous prenez la fuite ! Le monstre vous perd de vue dans les couloirs du donjon. (-0 PV)\n";
            }
            return;
        }
        else {
            cout << "Choix invalide.\n";
        }
    }

    // résolution du tour du monstre 
    // si le joueur n'a pas bust
    if (aventurierScore <= 21) {
        cout << "\n--- Tour du Monstre ---\n\n";
        cout << "Le monstre revele sa carte cachee : ";
        if (carteCachee == 1) cout << "As";
        else cout << valeurCachee;
        cout << " -> Total : " << monstreScore << "\n";

        // le croupier tire tant qu'il n'a pas au moins 17
        while (monstreScore < 17) {
            cout << "Le monstre tire : ";
            tirerCarte(monstreScore, asMonstre, false);
            cout << " -> Total : " << monstreScore << "\n";
        }

        // Vérif winner
        if (monstreScore > 21 || aventurierScore > monstreScore) {
            cout << "\nVICTOIRE ! Vous avez battu le monstre.\n\n";
            uniform_int_distribution<int> chanceLoot(0, 10);
            if (0<chanceLoot(rng)<3) {
                cout << "Le monstre laisse tomber une potion de heal ! (+1 PV) \n\n";
                joueur.setPV(joueur.getPV() + 1); 
            }
            return;
        } else if (aventurierScore == monstreScore) {
            cout << "\nEGALITE ! Vous vous regardez dans les yeux, le malaise est palpable et chacun repart de son cote.\n\n";
        } else {
            cout << "\nDEFAITE ! Le monstre a gagne (-2 PV)...\n";
            joueur.setPV(joueur.getPV() - 2); 
        }
    } else {
        cout << "\nDEFAITE ! La gourmandise est un vilain defaut, vous en payez le prix fort ...\n\n";
        joueur.setPV(joueur.getPV() - 2); 
    }
    
    cout << "Appuyez sur une touche et entree pour continuer...\n";
    char pause;
    cin >> pause;
}


void Tresor::effet(Aventurier& joueur) {
    joueur.setTresors(joueur.getTresors() + 1);
}

void Piege::effet(Aventurier& joueur) {
    joueur.setPV(joueur.getPV() - 1);
}

