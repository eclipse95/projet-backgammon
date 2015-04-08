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
    assert(movements != NULL);
    assert(movements->movements != NULL);
    assert(movements->movements->size > 0);
    assert(movements->movements->first != NULL);
    assert(movements->movements->first->movement != NULL);
    assert(movements->movements->first->movement->movements != NULL);
    assert(movements->movements->first->movement->nbMoves > 0);

    //get Score
    movements = getAllScores(gameState, movements);
    assert(movements != NULL);
    assert(movements->movements->first->movement->score != NULL);

    //return first
    res = getBest(movements);
    assert(res != NULL);
    assert(res->nbMoves <= 4);

    printf("DEBUG : nbMov of best choice = %d\n", res->nbMoves);

    nbMove = &res->nbMoves;

    printf("DEBUG : returned nbMove = %d\n",*nbMove);
    printf("DEBUG : returned moves =>\n");
    int ite;
    for(ite=0 ; ite<res->nbMoves ; ite++){
        moves[ite] = res->movements[ite];

        printf("\tmove %d : from %d to %d\n",ite+1,res->movements[ite].src_point,res->movements[ite].dest_point);
    }

    free(movements);
}


// Unofficial Functions


IA* getAllMovements(const SGameState* const gameState, const unsigned char dices[2]){
    IA* allMovements = (IA*) calloc(1,sizeof(IA));
    int nbMove = 0;
    if(dices[0] == dices[1]){
        nbMove = 4;
    }
    else{
        nbMove = 2;
    }
    SMove* array = calloc(15*nbMove, sizeof(SMove));// Stocke la liste des mouvements possibles pendant ce tour (peut contenir des doublons)
    // Maximum de (15*nbMove) mouvements car il y a 15 jetons par joueur

    int* arrayTmp = calloc(nbMove, sizeof(int));
    int arraySize = 0;
    int arrayTmpSize = 0;



    // Generate all possible movements in array
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

                // Gestion des mouvements successifs
                int canContinue = 1;
                while(canContinue == 1){
                    canContinue = 0;
                    for(dice=0; dice < nbMove; dice++){
                        int val = dices[dice%2];
                        int dest = getDest(ite,val);

                        if(!inArray(dice, arrayTmp, arrayTmpSize)) {
                            if (gameState->board[dest].owner != var_globale.me &&
                                gameState->board[dest].owner != -1 &&
                                gameState->board[dest].nbDames >= 2) {
                                //NEXT !
                            }
                            else {
                                SMove* move = malloc(sizeof(SMove));
                                move->dest_point = (unsigned int) dest + 1;
                                move->src_point = (unsigned int) ite + 1;


                                mov = 0;
                                while (mov < gameState->board[ite].nbDames && mov < nbMove) {
                                    array[arraySize++] = *move;
                                    mov++;
                                }
                                arrayTmp[arrayTmpSize++] = dice;
                                //canContinue = 1; //Comment this line to disable successive movements (DEBUG)

                            }
                        }
                    }
                }

            }

            arrayTmpSize = 0; // Nouveau tableau des dés pour une nouvelle case
        }
    }

    //Fin de l'algo ici

    if(arraySize == 0)
        return NULL;

    printf("DEBUG : number of possible movements = %d\n",arraySize);

    switch(nbMove)
    {
        case 4:
            allMovements->movements = combination4(array,arraySize);
            if(allMovements->movements->size > 0){
                printf("DEBUG : Used combination of 4\n");
                break;
            }
        case 2:
            allMovements->movements = combination2(array,arraySize, gameState);
            if(allMovements->movements->size > 0){
                printf("DEBUG : Used combination of 2\n");
                break;
            }
        default:
            allMovements->movements = combination1(array,arraySize);
            printf("DEBUG : Used combination of 1\n");
            break;
    }

    assert(allMovements->movements != NULL);
    assert(allMovements->movements->size > 0);

    printf("DEBUG : Number of plays possible = %d\n", allMovements->movements->size);

    var_globale.onlyBarUsed = 0;
    return allMovements;
}

IA* getAllScores(const SGameState* const gameState, IA* allMovements){

    printf("DEBUG : Starting to calculate all scores\n");

    assert(allMovements != NULL);

    int globalScore = getGlobalScore(gameState);
    Maillon* tmp = allMovements->movements->first;
    do{
        IAMove* moves = tmp->movement;
        moves->score = malloc(sizeof(IAScore));
        moves->score->score = 0;
        moves->score->notSafe = 0;

        assert(tmp->movement->score != NULL);

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
    IAScore* score = calloc(1,sizeof(IAScore));
    score->score = 0;
    score->notSafe = 0;

    int ite;
    for(ite = 0 ; ite < moves->nbMoves ; ite++){
        SMove* tmp = &moves->movements[ite];

        if(gameState->board[tmp->src_point].owner == var_globale.me){
            /*if(gameState->board[tmp.dest_point].nbDames == 2){
                score->notSafe++;
            }*/ // Gestion de la securite

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
        if(move->score->score > maxScore && move->score->notSafe == 0){ // Possibilit� d'utiliser une variable globale pour la tol�rance niveau s�curit�
            best = move;
        }

        tmp = tmp->suiv;
    }while(tmp != NULL);

    if(best == NULL){
        do{
            move = tmp->movement;
            assert(move != NULL);
            if(move->score->score > maxScore){ // Dans le cas où on a pas de movement safe
                best = move;
            }

            tmp = tmp->suiv;
        }while(tmp != NULL);
    }

    return best;
}


// C(n,k) recherche des combinaisons
Pile* combination1(SMove* array, int size){
    Pile* moves = createPile(); // WHY ARE YOU FUCKING FAILING ?!

    IAMove* tmp = NULL;
    int cpt;

    for(cpt=1; cpt<size ; cpt++){
        tmp->movements = calloc(1,sizeof(SMove));
        tmp->movements[0] = array[cpt];
        tmp->nbMoves = 1;
        push(moves,tmp);
    }

    return moves;
}

Pile* combination2(SMove* array, int size, const SGameState* const gameState){ //TODO use a dictionnary
    Pile* moves = createPile();
    IAMove* tmp = NULL;
    int cpt,a,b = 0;
    Dictionnary* dico = createDico();

    for(cpt=1; cpt<size*size ; cpt++){
        a = cpt % size;
        b = cpt / size;
        if(b>a &&
       (array[a].dest_point - array[a].src_point) != (array[b].dest_point - array[b].src_point) &&
       isPossible(gameState, dico, array[a].src_point) &&
       isPossible(gameState, dico, array[b].src_point)){
            tmp = calloc(1,sizeof(IAMove));
            tmp->movements = calloc(2,sizeof(SMove));
            tmp->movements[0] = array[a];
            tmp->movements[1] = array[b];
            tmp->nbMoves = 2;
            push(moves,tmp);
            resetDico(dico);
        }
    }

    //free(dico);
    return moves;
}

int isPossible(const SGameState* const gameState, Dictionnary* dico, int key){
    if((gameState->board[key].nbDames - dico->values[key]) > 0){
        dico->values[key]++;
        return 1;
    }
    return 0;
}

Dictionnary* createDico(){
    Dictionnary* dico = malloc(sizeof(dico));
    resetDico(dico);

    return dico;
}

void resetDico(Dictionnary* dico){
    int i;
    for(i=0; i<24; i++){
        dico->values[i] = 0;
    }
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

int inArray(int needle,int* haystack, int length){
    int i;
    for(i=0; i<length; i++) {
        if (haystack[i] == needle) {
            return 1;
        }
    }
    return 0;
}

void init_stock_var(stock_var* var)
{
    var->me = NOBODY;
    var->score = 0;
    var->onlyBarUsed = 0;
}









//// ================== [Gestion de la pile]
Pile* createPile(){
    Pile* tmp = malloc(sizeof(Pile));
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
        free(tmp);  //lib�re la m�moire
    }
}

IAMove* top(Pile* pile){
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

void realloc_IAMove(IAMove* move, int new_size ) // C'est quoi ça ?
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

void delete_IAMove(IAMove* move) // C'est quoi ça ?
{
    if(move != NULL)
    {
        if(move->movements != NULL)
            free(move->movements);
        free(move);
    }
}


int main(){ // Test des fonctions une à une
    // TODO
    init_stock_var(&var_globale);
    var_globale.me = WHITE;

    SGameState* fakeGame = malloc(sizeof(SGameState));
    int i;
    for(i=0; i<24; i++){
        fakeGame->board[i].owner = NOBODY;
        fakeGame->board[i].nbDames = 0;

        if(i == 5){
            fakeGame->board[i].owner = WHITE;
            fakeGame->board[i].nbDames = 1;
        }

        /*
        if(i == 9){
            fakeGame->board[i].owner = WHITE;
            fakeGame->board[i].nbDames = 3;
        }
        */

    }
    fakeGame->bar[0]=0,fakeGame->bar[1]=0;
    fakeGame->out[0]=0,fakeGame->out[1]=0;
    fakeGame->whiteScore = 0;
    fakeGame->blackScore = 0;
    fakeGame->turn = 0;
    fakeGame->stake = 1;

    unsigned char dices[2];
    SMove moves[4];
    unsigned int nbMoves;

    /*
    int j;
	for(j=0;j<2;j++)
	{
		dices[j]=(rand() % 6)+1;		// peut-être cast nécessaire
	}
	*/

    dices[0]=1;
    dices[1]=3;



    printf("Init successful\n");

    PlayTurn(fakeGame,dices,moves,&nbMoves,0);


    return 0;
}
