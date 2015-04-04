#include <stdio.h>
#include <stdlib.h>

typedef struct Maillon Maillon;

// IAMove

struct Maillon{
    Maillon* prec;
    IAMove* movement;
    Maillon* suiv;
};

typedef struct{
    Maillon* first;
    Maillon* last;
    int size;
}Pile;

/* Constructeur de pile */
Pile* createPile();

/* Constructeur de maillon */
Maillon* createMaillon();

short estVide(Pile* pile);

void push(Pile*, IAMove*);

void pop(Pile*);

IAMove* top(Pile*);

// destructeurs
void delete_maillon(Maillon*);

void delete_pile(Pile*);

int testPile();

