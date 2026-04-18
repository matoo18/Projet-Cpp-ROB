# Structure du dossier

C'est facile à comprendre juste faut faire les ?h à la main c'est un peu chiant. En gros tu dois juste réécrire les noms des fonctions, classes, méthodes mais sans les implémenter SAUF SI elles sont virtuelles. Regarde le case.h normalement il est assez parlant.

# Le #ifdef
Dans les fichiers cpp pour tester l'implémentation du module solo je rajoute :

```cpp
#ifdef TEST_X
int main() {
    // Code test spécifique à ce fichier
    return 0;
}
#endif
```

C'est l'équivalent du if __name__ == "__main__": en Python en gros.

Pour le compiler :

```
g++ includes/modulex.cpp includes/moduley.cpp -DTEST_X -o modulex.exe
```
Faut penser à mettre le .exe quand on compile comme ça le gitignore fonctionnera.