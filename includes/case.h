#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

enum TypeCase {
    MUR, 
    PASSAGE, 
    TRESOR, 
    MONSTRE, 
    PIEGE
};

class Case {
public:
    virtual char afficher() = 0; 
    virtual ~Case() {}
};


class Mur : public Case {
public:
    char afficher() override { return '#'; }
};

class Passage : public Case {
public:
    char afficher() override { return ' '; }
};

class Tresor : public Case {
public:
    char afficher() override { return '+'; }
};

class Monstre : public Case {
public:
    char afficher() override { return 'M'; }
};

class Piege : public Case {
public:
    char afficher() override { return 'T'; }
};


class CaseFactory {
public:
    virtual Case* creerCase() = 0;
    virtual ~CaseFactory() {}
};

class MurFactory : public CaseFactory {
public:
    Case* creerCase() override { return new Mur(); }
};

class PassageFactory : public CaseFactory {
public:
    Case* creerCase() override { return new Passage(); }
};

class TresorFactory : public CaseFactory {
public:
    Case* creerCase() override { return new Tresor(); }
};

class MonstreFactory : public CaseFactory {
public:
    Case* creerCase() override { return new Monstre(); }
};

class PiegeFactory : public CaseFactory {
public:
    Case* creerCase() override { return new Piege(); }
};

CaseFactory* getFactory(TypeCase type_of_case);
