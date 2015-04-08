#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "librairy.h"
#include <assert.h>

// compile avec Code::Blocks    // mon CMake est foireux
// 1 warning

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
    var_globale.score = target_score; // Pas vraiment utile mais bon si besoin
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

    int score = getGlobalScore(gameState);

    if(score > 10){
        return 1;
    }
    else{
        return 0;
    }

}

int TakeDouble(const SGameState * const gameState)
{
    printf("TakeDouble\n");

    int score = getGlobalScore(gameState);

    if(score < -10){
        return 0;
    }
    else{
        return 1;
    }
}

void PlayTurn(const SGameState* const gameState, const unsigned char dices[2], SMove moves[4], unsigned int* nbMove, unsigned int tries)
//typedef void (*pfPlayTurn)(const SGameState * const,  const unsigned char[2], SMove[4], unsigned int*, unsigned int);
{
    IA* movements = NULL;
    IAMove* res = NULL;
    //generate All Moves Possible
    movements = getAllMovements(gameState,dices);

    //get Score
    movements = getAllScores(gameState, movements);

    //return first
    res = getBest(movements);

    res->nbMoves = *nbMove;

    assert(movements != NULL);
    assert(res != NULL);

    int ite;
    for(ite=0 ; ite<res->nbMoves ; ite++){
        moves[ite] = res->movements[ite];        // Problème de type SMove* / ? (cf struct IAMove)
    }

    free(movements);
}


// Unofficial Functions


IA* getAllMovements(const SGameState* const gameState, const unsigned char dices[2]){
    IA* allMovements = (IA*) calloc(1,sizeof(IA));
    SMove* array = calloc(16, sizeof(SMove));// Stocke la liste des mouvements possibles pendant ce tour (peut contenir des doublons)
    // Maximum de 15 mouvements car il y a 15 jetons par joueur
    int arraySize = 0;

    int nbMove = 0;
    if(dices[0] == dices[1]){
        nbMove = 4;
    }
    else{
        nbMove = 2;
    }

    // Generate all possible movements in array
    assert(gameState != NULL);
    assert(allMovements != NULL);
    assert(array != NULL);
    int ite,dice,mov;
    if(gameState->bar[var_globale.me] > 0){ // Recherche des mouvements depuis la barre
        if(gameState->bar[var_globale.me] >= nbMove){
            var_globale.onlyBarUsed = 1;
        }

        for(dice=0; dice < nbMove; dice++){
            int val = dices[dice%2];
            int dest = getDest(0,val);

            if(gameState->board[dest].owner != var_globale.me &&
               gameState->board[dest].owner != -1 &&
               gameState->board[dest].nbDames >= 2){
                //NEXT !
            }
            else{
                SMove* move = malloc(sizeof(SMove));
                move->dest_point = 0;
                move->src_point = (unsigned int) dest+1;

                mov = 0;
                while(mov < gameState->bar[var_globale.me] && mov<nbMove) {
                    array[arraySize] = *move;
                    arraySize++;
                    mov++;
                }
            }
        }
    }
    if(var_globale.onlyBarUsed == 0){ // Recherche des mouvements possibles sur le terrain
        for(ite=0; ite<24; ite++){
            Square tmp = gameState->board[ite];
            if(tmp.owner == var_globale.me){
                for(dice=0; dice < nbMove; dice++){
                    int val = dices[dice%2];
                    int dest = getDest(ite,val);

                    if(gameState->board[dest].owner != var_globale.me &&
                       gameState->board[dest].owner != -1 &&
                       gameState->board[dest].nbDames >= 2){
                        //NEXT !
                    }
                    else{
                        SMove* move = malloc(sizeof(SMove));
                        move->dest_point = (unsigned int) ite+1;
                        move->src_point = (unsigned int) dest+1;

                        mov = 0;
                        while(mov<gameState->board[ite].nbDames && mov<nbMove) {
                            array[arraySize] = *move;
                            arraySize++;
                            mov++;
                        }
                    }
                }
            }

        }
    }

    //Fin de l'algo ici

    // if(arraySize == 0)
    // 	return NULL;

    switch(nbMove)
    {
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
            break;
    }

    //free(array); Not a good idea...
    var_globale.onlyBarUsed = 0;
    return allMovements;
}

IA* getAllScores(const SGameState* const gameState, IA* allMovements){

    int globalScore = getGlobalScore(gameState);
    Maillon* tmp = allMovements->movements->first;

    do{
        IAMove* moves = tmp->movement;

        tmp->movement->score->score = globalScore - getScore(gameState, moves)->score;

        tmp = tmp->suiv;
    }while(tmp != NULL);

    return allMovements;
}

int getGlobalScore(const SGameState* const gameState){
    int ite,score =0;
    for(ite=0; ite<24; ite++) {
        Square tmp = gameState->board[ite];

        if(tmp.owner == var_globale.me){
            score += (24*var_globale.me) + (-1*var_globale.me)*(ite+1);
        }
        else if(tmp.owner != -1){
            score -= (24*var_globale.me) + (-1*var_globale.me)*(ite+1);
        }
    }

    return score;
}

IAScore* getScore(const SGameState* const gameState, IAMove* moves){
    IAScore* score = calloc(1,sizeof(IAScore)); // TODO Il semble y avoir une erreur par ici
    score->score = 0;
    score->notSafe = 0;

    int ite;
    for(ite = 0 ; ite < moves->nbMoves ; ite++){
        SMove* tmp = &moves->movements[ite];

        if(gameState->board[tmp->src_point].owner == var_globale.me){
            /*if(gameState->board[tmp.dest_point].nbDames == 2){
                score->notSafe++;
            }*/ // Gestion de la sécurité

            // Check de la source
            if(gameState->board[tmp->dest_point].owner == var_globale.me){
                score->score += (-gameState->board[tmp->dest_point].owner+1 *(-1))*(tmp->src_point - tmp->dest_point);
            }
            else if(gameState->board[tmp->dest_point].owner != -1){
                score->score -= (-gameState->board[tmp->dest_point].owner+1 *(-1))*(tmp->src_point - tmp->dest_point);
            }

            // Check de la destination
            if(gameState->board[tmp->dest_point].owner != var_globale.me &&
               gameState->board[tmp->dest_point].owner != -1 &&
               gameState->board[tmp->dest_point].nbDames == 1){
                score->score += (24*((var_globale.me+1)%2)) + tmp->dest_point;
            }
        }
    }

    return score;
}

IAMove* getBest(IA* allMovements){
    int maxScore = -1;
    IAMove* best = NULL;
    IAMove* move = NULL;
    Maillon* tmp = allMovements->movements->first;

    do{
        move = tmp->movement;
        assert(move != NULL);
        if(move->score->score > maxScore && move->score->notSafe == 0){ // Possibilité d'utiliser une variable globale pour la tolérance niveau sécurité
            best = move;
        }

        tmp = tmp->suiv;
    }while(tmp != NULL);


    return best;
}


// C(n,k) recherche des combinaisons
Pile* combination1(SMove* array, int size){
    Pile* moves = createPile();
    IAMove* tmp = NULL;
    int cpt = 0;
    assert(array != NULL);
    for(cpt=1; cpt<size ; cpt++){
        tmp = calloc(1,sizeof(IAMove));
        tmp->movements = calloc(1,sizeof(SMove));
        tmp->movements[0] = array[cpt];
        tmp->nbMoves = 1;
        push(moves,tmp);         // probleme type SMove* / IAMove
    }
    return moves;
}

Pile* combination2(SMove* array, int size){
    Pile* moves = createPile();
    IAMove* tmp = NULL;
    int cpt,a,b = 0;
    for(cpt=1; cpt<size*size ; cpt++){
        a = cpt % size;
        b = cpt / size;
        if(b>a &&
       (array[a].dest_point - array[a].src_point) != (array[b].dest_point - array[b].src_point)){
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
    Pile* moves = createPile();
    IAMove* tmp = NULL;
    int cpt,a,b,c,d = 0;
    for(cpt=1; cpt<size*size*size*size ; cpt++){
        a = cpt % size;
        b = cpt / size;
        c = cpt / size*size;
        d = cpt / size*size*size;
        if(d>c && c>b && b>a){
            tmp = create_IAMove();
            tmp->movements[0] = array[a];
            tmp->movements[1] = array[b];
            tmp->movements[2] = array[c];
            tmp->movements[3] = array[d];
            tmp->nbMoves = 4;
            push(moves,tmp);
        }
    }
    return moves;
}

int getDest(int src, int length){
    int dest = src - (-1*var_globale.me)*length;
    if(dest > 24){
        dest = 24;
    }
    else if(dest < -1){
        dest = -1;
    }

    return dest;
}

void init_stock_var(stock_var* var)
{
    var->me = NOBODY;
    var->score = 0;
    var->onlyBarUsed = 0;
}

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
