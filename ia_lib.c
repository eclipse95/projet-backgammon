#include "ia_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// variable globale
stock_var var_globale;

void InitLibrary(char name[50])
{
	printf("InitLibrary\n");
	strcpy(name,"Yay");
	init_stock_var(&var_globale);
}

void StartMatch(const unsigned int target_score)
{
    var_globale.score = target_score;
	printf("StartMatch\n");
}

void StartGame(Player p)
{
    var_globale.me = p;
	printf("StartGame\n");
}

void EndGame()
{
	printf("EndGame\n");
}

void EndMatch()
{
	printf("EndMatch\n");
}

int DoubleStack(const SGameState * const gameState)
{
	printf("DoubleStack\n");
	return(0);
}

int TakeDouble(const SGameState * const gameState)
{
	printf("TakeDouble\n");
	return(0);
}

void PlayTurn(const SGameState* const gameState, const unsigned char dices[2], SMove moves[4], unsigned int* nbMove, unsigned int tries)
//typedef void (*pfPlayTurn)(const SGameState * const,  const unsigned char[2], SMove[4], unsigned int*, unsigned int);
{
    IA* movements;
    IAMove* res;
    //generate All Moves Possible
    movements = getAllMovements(gameState,dices);

    //get Score
    movements = getAllScores(gameState, movements);

    //return first
    res = getBest(movements);

    nbMove = (unsigned int*) res->nbMoves;      // probleme d'affectation nbMove est paramètre
    int ite;
    for(ite=0 ; ite<nbMove ; ite++){
        moves[ite] = res->movements[ite];        // Problème de type SMove* / ? (cf struct IAMove)
    }

    free(movements);
    free(res);
}


// Unofficial Functions


IA* getAllMovements(const SGameState* const gameState, const unsigned char dices[2]){
    IA* allMovements = (IA*) calloc(1,sizeof(IA));
    SMove* array;// Stocke la liste des mouvements possibles pendant ce tour (peut contenir des doublons)
    int arraySize = 0;

    int nbMove;
    if(dices[0] == dices[1]){
        nbMove = 4;
    }
    else{
        nbMove = 2;
    }

    // Generate all possible movements in array
    // TODO
    //arraySize++
    //array = ...

    if(arraySize == 0){
    	return NULL;
    }

    // On récupère les combinaisons
    switch(nbMove){
    case 4:
        allMovements->movements = combination4(array,arraySize);
        if(allMovements->movements->size > 0){
            break;
        }
    case 2:
        allMovements->movements = combination2(array,arraySize);
        if(allMovements->movements->size > 0){
            break;
        }
    default:
        allMovements->movements = combination1(array,arraySize);
    }

    return allMovements;
}

IA* getAllScores(const SGameState* const gameState, IA* allMovements){

    int globalScore = getGlobalScore(gameState);
    Maillon* tmp = allMovements->movements->first;

    do{
        IAMove* moves = (IAMove*) tmp->suiv->movement;

        tmp->score = globalScore - getScore(gameState, moves); //! Problème type int - IAMove*
    }while(tmp->suiv != NULL);

    return allMovements;
}

int getGlobalScore(const SGameState* const gameState){

}

IAScore* getScore(const SGameState* const gameState, IAMove* moves){
    IAScore* score = calloc(1,sizeof(IAScore));         // IAScore non implémenté
    score->score = 0;
    score->notSafe = 0;

    int ite;
    for(ite = 0 ; ite < moves->nbMoves ; ite++){
        SMove* tmp = moves->movements[ite];     // Problème de type SMove* / Pile*

        if(gameState->board[tmp->src_point].owner == player){       // player non défini
            /*if(gameState->board[tmp.dest_point].nbDames == 2){
                score->notSafe++;
            }*/ // Gestion de la sécurité
            score->score += (tmp->dest_point - tmp->src_point);

            if(gameState->board[tmp->dest_point].owner != player &&      // player non défini
               gameState->board[tmp->dest_point].owner != -1 &&
               gameState->board[tmp->dest_point].nbDames == 1){
                score->score += (24 - tmp->dest_point); // TODO
            }
        }
    }

    return score;
}

IAMove* getBest(IA* allMovements){
    int maxScore = -1;
    IAMove* best;

    Maillon* tmp = allMovements->movements->first;

    do{
        IAMove* move = tmp->suiv->movement;

        if(move->score > maxScore){ // Possibilité d'utiliser une variable globale pour la tolérance niveau sécurité
            best = move;
        }
    }while(tmp->suiv != NULL);

    return best;
}


// C(n,k) recherche des combinaisons
Pile* combination1(SMove* array, int size){
    Pile* moves = createPile();
    IAMove* tmp;
    int cpt = 0;
    for(cpt=1; cpt<size ; cpt++){
        tmp = calloc(1,sizeof(IAMove));
        tmp->movements = calloc(1,sizeof(SMove));
        tmp->movements[0] = array[cpt];         // probleme type SMove* / Pile*
        tmp->nbMoves = 1;
        push(moves,tmp);
    }
    return moves;
}

Pile* combination2(SMove* array, int size){
    Pile* moves = createPile();
    IAMove* tmp;
    int cpt,a,b = 0;
    for(cpt=1; cpt<size*size ; cpt++){
        a = cpt % size;
        b = cpt / size;
        if(b>a){
            tmp = calloc(1,sizeof(IAMove));
            tmp->movements = calloc(2,sizeof(SMove));
            tmp->movements[0] = array[a];       //probleme type SMove / ?
            tmp->movements[1] = array[b];       //
            tmp->nbMoves = 2;
            push(moves,tmp);
        }
    }
    return moves;
}

Pile* combination4(SMove* array, int size){
    Pile* moves = (Pile*) createPile();
    IAMove* tmp;
    int cpt,a,b,c,d = 0;
    for(cpt=1; cpt<size*size*size*size ; cpt++){
        a = cpt % size;
        b = cpt / size;
        c = cpt / size*size;
        d = cpt / size*size*size;
        if(d>c && c>b && b>a){
            tmp = calloc(1,sizeof(IAMove));
            tmp->movements = calloc(4,sizeof(SMove));
            tmp->movements[0] = array[a];       // probleme type SMove / ?
            tmp->movements[1] = array[b];       //
            tmp->movements[2] = array[c];       //
            tmp->movements[3] = array[d];       //
            tmp->nbMoves = 4;
            push(moves,tmp);
        }
    }
    return moves;
}

void init_stock_var(stock_var* var)
{
    var->me = NOBODY;
    var->score = 0;
}
