#ifndef CASE_H
#define CASE_H


#include <iostream>


class Aventurier; //Declaration simple sinon on a les 3 headers qui ont besoin des un les autres et ça explose TOUT.

enum TypeCase {
    MUR, 
    PASSAGE, 
    TRESOR, 
    MONSTRE, 
    PIEGE,
    ENTREE,
    SORTIE,
    AVENTURIER
};

class Case {
public:
    virtual void afficher() = 0;
    virtual ~Case() {}
    virtual TypeCase getType() const = 0;
    virtual void effet(Aventurier& joueur) = 0;
};

class Mur : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return MUR; }
    void effet(Aventurier& joueur) override {
        //Rien et de toute façon jamais on se retrouvera sur cette case
    }
};

class Passage : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return PASSAGE; }
    void effet(Aventurier& joueur) override { }
};

class Tresor : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return TRESOR; }
    void effet(Aventurier& joueur) override;
};

class Monstre : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return MONSTRE; }
    void effet(Aventurier& joueur) override;
};

class Piege : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return PIEGE; }
    void effet(Aventurier& joueur) override;
};

class Entree : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return ENTREE; }
    void effet(Aventurier& joueur) override { }
};

class Sortie : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return SORTIE; }
    void effet(Aventurier& joueur) override { }
};

class CaseAventurier : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return AVENTURIER; }
    void effet(Aventurier& joueur) override { }
};



class CaseFactory {
public:
    static Case* creerCase(TypeCase type);
};

CaseFactory* getFactory(TypeCase type_of_case);

#endif