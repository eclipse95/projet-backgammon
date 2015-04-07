#include <stdio.h>
#include <stdlib.h>
#include "ia_lib.h"
typedef struct Maillon Maillon;

// IAMove
typedef struct
{
    SMove* movements;
    int nbMoves;
    IAScore* score;
} IAMove;

typedef struct
{
    int score;
    int notSafe;
} IAScore ;

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

IAMove* top(Pile*); // problème retour pile* / IAMove*

// destructeurs
void delete_maillon(Maillon*);

void delete_pile(Pile*);

int testPile();

IAMove* create_IAMove();

void delete_IAMove(IAMove*);

void realloc_IAMove(IAMove*, int);


