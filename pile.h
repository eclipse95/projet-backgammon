#include <stdio.h>
#include <stdlib.h>

typedef struct Maillon Maillon;

// IAMove
typedef struct				// <!> A modifier // TODO
{
    int* movements;		// Changer le type
    int nbMoves;
} IAMove;

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

IAMove* top(Pile*); // probl�me retour pile* / IAMove*

// destructeurs
void delete_maillon(Maillon*);

void delete_pile(Pile*);

int testPile();

