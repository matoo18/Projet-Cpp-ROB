#ifndef CASE_H
#define CASE_H

#include <iostream>

enum TypeCase {
    MUR, 
    PASSAGE, 
    TRESOR, 
    MONSTRE, 
    PIEGE,
    ENTREE,
    SORTIE
};

class Case {
public:
    virtual void afficher() = 0;
    virtual ~Case() {}
    virtual TypeCase getType() const = 0;
};

class Mur : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return MUR; }
};

class Passage : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return PASSAGE; }
};

class Tresor : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return TRESOR; }
};

class Monstre : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return MONSTRE; }
};

class Piege : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return PIEGE; }
};

class Entree : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return ENTREE; }
};

class Sortie : public Case {
public:
    void afficher() override;
    TypeCase getType() const override { return SORTIE; }
};



class CaseFactory {
public:
    static Case* creerCase(TypeCase type);
};

CaseFactory* getFactory(TypeCase type_of_case);

#endif