#include <stdio.h>
#include <stdlib.h>
#include "pile.h"

/* Constructeur de pile */
Pile* createPile(){
    Pile* tmp = calloc(1,sizeof(Pile));
    tmp->first = NULL;
    tmp->last = NULL;
    tmp->size = 0;

    return tmp;
}

/* Constructeur de maillon */
Maillon* createMaillon(){
    Maillon* tmp = calloc(1,sizeof(Maillon));
    tmp->prec = NULL;
    tmp->suiv = NULL;
    tmp->movement = NULL;

    return tmp;
}

short estVide(Pile* pile){
    return (pile->size == 0);
}

void push(Pile* pile, IAMove* movement){
    Maillon* tmp = createMaillon();

    tmp->movement = movement;

    if(estVide(pile)){
        pile->first = tmp;
        pile->last = tmp;
    }
    else{
        pile->last->suiv = tmp;
        tmp->prec = pile->last;
        pile->last = tmp;
    }
    pile->size++;
}

void pop(Pile* pile){
    if(!estVide(pile)){
        Maillon* tmp = pile->last;

        pile->last = tmp->prec;
        pile->size--;
        free(tmp);  //libère la mémoire
    }
}

IAMove* top(Pile* pile){            // problème retour pile* / IAMove*
    if(!estVide(pile)){
        return pile->last->movement;
    }
    else{
        return NULL;
    }
}

void delete_maillon(Maillon* maillon)
{
    free(maillon);
}


void delete_pile(Pile* pile)
{
    int i;
    for(i = 0; i < pile->size; i++)
        pop(pile);
    free(pile);
}

int testPile(){
    int test = 0;
    Pile* pile = createPile();
    if(!estVide(pile)){
        printf("ERROR - echec dans la creation de la pile\n");
        test = 1;
    }

    //push(pile,"test1");     // ne peut pas marcher -> push(Pile*,IAMove*)

    //push(pile,"test2");

    if(estVide(pile)){
        printf("ERROR - echec de la fonction push\n");
        test = 1;
    }

    printf("%d\n",pile->size);

    pop(pile);

/*
    push(&pile,"test2");
    printf("%s\n",(&pile)->last->balise);

    pop(&pile);
    printf("%s\n",(&pile)->last->balise);

    if((&pile)->last->balise != "test2"){
        printf("ERROR - echec de top\n");
        test = 1;
    }

    pop(&pile);

    if(top(&pile) != "test1"){
        printf("ERROR - echec de pop dans sa récupération\n");
        test = 1;
    }


    pop(&pile);

    if(!estVide(&pile)){
        printf("ERROR - echec de pop dans sa suppression\n");
        test = 1;
    }
*/
    return test;
}


IAMove* create_IAMove()
{
    IAMove* tmp = malloc(sizeof(IAMove));
    tmp->nbMoves = 4;
    tmp->movements = calloc((size_t) tmp->nbMoves, sizeof(SMove));
    return tmp;
}

void realloc_IAMove(IAMove* move, int new_size )
{
    if(move != NULL)
    {
        if (move->nbMoves > new_size)
            move->nbMoves += new_size;
        else
            move->nbMoves = new_size;
        //move = realloc()
    }
}

void delete_IAMove(IAMove* move)
{
    if(move != NULL)
    {
        if(move->movements != NULL)
            free(move->movements);
        free(move);
    }
}
