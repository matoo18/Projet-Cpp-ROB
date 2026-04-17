#include<iostream>
#include<vector>
#include<string>
#include<cmath>

using namespace std;

enum TypeCase {
    MUR, 
    PASSAGE, 
    TRESOR, 
    MONSTRE, 
    PIEGE
};

// TOUTES LES CASES
class Case {
    public:
        virtual void afficher()=0;
        virtual ~Case(){};
};

class Mur : public Case {
    public:
        void afficher() override {
            cout << "#";
        };
};
class Passage : public Case {
    public:
        void afficher() override {
            cout << " ";
        };
};
class Tresor : public Case {
    public:
        void afficher() override {
            cout << "+";
        };
};
class Monstre : public Case {
    public:
        void afficher() override {
            cout << "M";
        };
};
class Piege : public Case {
    public:
        void afficher() override {
            cout << "T";
        };
};

// TOUTES LES FACTORIES
class CaseFactory {
    public:
        virtual Case* creerCase()=0;
        virtual ~CaseFactory() {};
};
class MurFactory : public CaseFactory {
    public:
        Case* creerCase() override {
            return new Mur();
        };
};
class PassageFactory : public CaseFactory {
    public:
        Case* creerCase() override {
            return new Passage();
        };
};
class TresorFactory : public CaseFactory {
    public:
        Case* creerCase() override {
            return new Tresor();
        };
};
class MonstreFactory : public CaseFactory {
    public:
        Case* creerCase() override {
            return new Monstre();
        };
};
class PiegeFactory : public CaseFactory {
    public:
        Case* creerCase() override {
            return new Piege();
        };
};

CaseFactory* getFactory(enum TypeCase type_of_case) {
    if (type_of_case==MUR)          return new MurFactory();
    else if (type_of_case==PASSAGE) return new PassageFactory();
    else if (type_of_case==TRESOR)  return new TresorFactory();
    else if (type_of_case==MONSTRE) return new MonstreFactory();
    else if (type_of_case==PIEGE)   return new PiegeFactory();
    return nullptr;
};


int main() {
    enum TypeCase myVar(MUR);
    CaseFactory* f=getFactory(myVar);
    
    if (f == nullptr) {
        cerr << "TypeCase inexistante." << endl;
        return 0;
    }
    Case* c=f->creerCase();
    c->afficher();
    return 0;
};