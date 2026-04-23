#include "case.h"

using namespace std;

void Mur::afficher() { cout << "#"; }
void Passage::afficher() { cout << " "; }
void Tresor::afficher() { cout << "+"; }
void Monstre::afficher() { cout << "M"; }
void Piege::afficher() { cout << "T"; }
void Entree::afficher() { cout << "e"; }
void Sortie::afficher() { cout << "s"; }
void CaseAventurier::afficher() { cout << "@";}

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

