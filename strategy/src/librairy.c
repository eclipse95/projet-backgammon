#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "librairy.h"
#include <assert.h>


// Version finale

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
    var_globale.score = target_score; // Contient la limite de score pour finir victorieux
    printf("StartMatch\n");
}

void StartGame(Player p)
{
    var_globale.me = p; // Attribution du numéro/couleur de joueur
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

    int score = getGlobalScore(gameState); // cette valeur correspond à l'écart de cases restantes pour meetre tous les pions dans la maison entre les joueurs

    if(score > 15){ //  Cette valeur correspond à la limite requise pour faire le double
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

    if(score < -20){  // Cette valeur correspond à la limite avant d'abandonner la partie en cas de double
        return 0;
    }
    else{
        return 1;
    }
}

void PlayTurn(const SGameState* const gameState, const unsigned char dices[2], SMove moves[4], unsigned int* nbMove, unsigned int tries)
{
    IA* movements = NULL;
    IAMove* res = NULL;
    SMove* array = NULL;

    //generate All Moves Possible
    movements = getAllMovements(gameState,dices, array);

    // assertions pour les tests
    assert(movements != NULL);
    assert(movements->movements != NULL);
    assert(movements->movements->size > 0);
    assert(movements->movements->first != NULL);
    assert(movements->movements->first->movement != NULL);
    assert(movements->movements->first->movement->movements != NULL);
    assert(movements->movements->first->movement->nbMoves > 0);

    //get Score
    movements = getAllScores(gameState, movements);

    // assertions pour les tests
    assert(movements != NULL);
    assert(movements->movements->first->movement->score != NULL);

    //return first
    res = getBest(movements);

    assert(res != NULL);
    assert(res->nbMoves <= 4);

    printf("DEBUG : nbMov of best choice = %d\n", res->nbMoves);

    *nbMove = res->nbMoves; // Renvoi du nombre de mouvement

    printf("DEBUG : returned nbMove = %d\n",*nbMove);
    printf("DEBUG : returned moves =>\n");
    int ite;
    for(ite=0 ; ite<res->nbMoves ; ite++){
        moves[ite] = res->movements[ite]; // Renvoi de chaque mouvement

        printf("\tmove %d : from %d to %d\n",ite+1,res->movements[ite].src_point,res->movements[ite].dest_point);
        printf("\tmove %d : from %d to %d\n",moves[ite].src_point,moves[ite].dest_point);
    }

    freeAll(movements, array, res);
}


// Unofficial Functions


    /**
     * Récupère le liste de tous les mouvements possibles
     * Puis génère les combinaisons possibles
     */
IA* getAllMovements(const SGameState* const gameState, const unsigned char dices[2], SMove* array){
    IA* allMovements = malloc(sizeof(IA));
    int nbMove = 0;
    if(dices[0] == dices[1]){
        nbMove = 4;
    }
    else{
        nbMove = 2;
    }
    array = calloc(15*nbMove, sizeof(SMove));// Stocke la liste des mouvements possibles pendant ce tour (peut contenir des doublons)
    // Maximum de (15*nbMove) mouvements car il y a 15 jetons par joueur


    int arraySize = 0;



    // Generate all possible movements in array
    int /*ite,*/dice,mov;
    if(gameState->bar[var_globale.me] > 0){ // Recherche des mouvements depuis la barre
        if(gameState->bar[var_globale.me] >= nbMove){
            var_globale.onlyBarUsed = 1;
        }
        else{
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
    if(var_globale.onlyBarUsed == 0) { // Recherche des mouvements possibles sur le terrain
        free(array); //suppression de la mémoire avant nouvelle allocation
        array = getAllMove(gameState, dices, &arraySize);

        printf("DEBUG : number of possible movements = %d\n", arraySize);

        //DEBUG Parcours
        int j;
        for (j = 0; j < arraySize; j++) {
            printf("DEBUG : move %d => from %d to %d\n", j, array[j].src_point, array[j].dest_point);
        }
    }

    switch(nbMove)
    {
    case 4:
        printf("DEBUG : Used combination of 4\n");
        allMovements->movements = combination4(array,arraySize);
        if(allMovements->movements->size > 0){
            break;
        }
        delete_pile(allMovements->movements);
    case 2:
        printf("DEBUG : Used combination of 2\n");
        allMovements->movements = combination2(array,arraySize, gameState);
        if(allMovements->movements->size > 0){
            break;
        }
        delete_pile(allMovements->movements);
    default:
        printf("DEBUG : Used combination of 1\n");
        allMovements->movements = combination1(array,arraySize);
        if(allMovements->movements->size > 0){
            nbMove = 0;
        }
            break;
    }
    assert(allMovements->movements != NULL || nbMove == 0);
    assert(allMovements->movements->size > 0);
        
    printf("DEBUG : Number of plays possible = %d\n", allMovements->movements->size);

    var_globale.onlyBarUsed = 0;
    return allMovements;
}

    /**
     * Recupération du score de chaque combinaison de mouvement possible
     */
IA* getAllScores(const SGameState* const gameState, IA* allMovements){

    printf("DEBUG : Starting to calculate all scores\n");

    assert(allMovements != NULL);

    int globalScore = getGlobalScore(gameState);
    Maillon* tmp = allMovements->movements->first;
    do{
        IAMove* moves = tmp->movement;
        moves->score = getScore(gameState, moves);

        assert(tmp->movement->score != NULL);

        moves->score->score = globalScore - moves->score->score;

        tmp = tmp->suiv;
    }while(tmp != NULL);

    return allMovements;
}

    /**
     * Renvoie le score global pour le tour en cours
     */
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

    /**
     * Renvoie la progression (ou régression) du score pour une combinaison donnée
     */
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

    /**
     * Récupère la combinaison ayant la meilleure progression
     */
IAMove* getBest(IA* allMovements){
    int maxScore = -1;
    IAMove* best = NULL;
    IAMove* move = NULL;
    assert(allMovements != NULL);
    assert(allMovements->movements != NULL);
    assert(allMovements->movements->first != NULL);
    Maillon* tmp = allMovements->movements->first;

    /* NON GESTION DE LA SECURITE
    do{
        move = tmp->movement;
        assert(move != NULL);
        if(move->score->score > maxScore && move->score->notSafe == 0){ // Possibilit� d'utiliser une variable globale pour la tol�rance niveau s�curit�
            best = move;
        }

        tmp = tmp->suiv;
    }while(tmp != NULL);

    */

    if(best == NULL){
        tmp = allMovements->movements->first;
        do{
            move = tmp->movement;
            assert(move != NULL);
            if(move->score->score > maxScore){ // Dans le cas où on a pas de movement safe ou que l'on ne gère pas la sécurité
                best = move;
            }

            tmp = tmp->suiv;
        }while(tmp != NULL);
    }

    if(best == NULL){
        best = allMovements->movements->first->movement;
    }

    return best;
}




    /**
     * Retourne 1 si le mouvement sélectionné est possible par rapport au reste de la combinaison
     * 0 sinon
     */
int isPossible(const SGameState* const gameState, Dictionnary* dico, SMove* move){
    if( (int)gameState->board[move->src_point].nbDames - dico->values[move->src_point] >= 0){
        dico->values[move->src_point]++; // On enlève un pion à l'origine

        dico->values[move->dest_point]--; // On rajoute un pion dans la destination
        return 1;
    }
    return 0;
}

    /**
     * Création d'un dictionnaire qui est en fait un tableau de la taille du terrain dont l'index est la clé
     */
Dictionnary* createDico(){
    Dictionnary* dico = malloc(sizeof(Dictionnary));
    resetDico(dico);

    return dico;
}

    /**
     * Remise à zéro du dictionnaire
     */
void resetDico(Dictionnary* dico){
    int i;
    for(i=0; i<24; i++){
        dico->values[i] = 0;
    }
}


// C(n,k) recherche des combinaisons
    /**
     * C(size,1)
     */
Pile* combination1(SMove* array, int size){
    Pile* moves = createPile();

    IAMove* tmp = NULL;
    int cpt;

    for(cpt=1; cpt<size ; cpt++){
        tmp = calloc(1,sizeof(IAMove));
        tmp->movements = calloc(1,sizeof(SMove));
        tmp->movements[0] = array[cpt];
        tmp->nbMoves = 1;
        push(moves,tmp);
    }

    return moves;
}
        
    /**
     * C(size,2)
     */
Pile* combination2(SMove* array, int size, const SGameState* const gameState){
    Pile* moves = createPile();

    IAMove* tmp = NULL;
    int cpt,a,b = 0;
    Dictionnary* dico = createDico();

    for(cpt=1; cpt<size*size ; cpt++){
        a = cpt % size;
        b = cpt / size;
        if(b>a &&
       (
            array[a].src_point != array[b].src_point ||
            (array[a].dest_point - array[a].src_point) != (array[b].dest_point - array[b].src_point)
        ) &&
       isPossible(gameState, dico, &array[a]) &&
       isPossible(gameState, dico, &array[b])){
            tmp = calloc(1,sizeof(IAMove));
            tmp->movements = calloc(4,sizeof(SMove));
            tmp->movements[0] = array[a];
            tmp->movements[1] = array[b];
            tmp->movements[2] = array[a];
            tmp->movements[3] = array[b];
            tmp->nbMoves = 2;
            push(moves,tmp);
            resetDico(dico);
        }
    }

    free(dico);
    return moves;
}

    /**
     * C(size,2)
     */
Pile* combination4(SMove* array, int size){
    Pile* moves = createPile();
    IAMove* tmp = NULL;
    int cpt,a = 0,b = 0,c = 0,d = 0;
    for(cpt=1; cpt<(size*size*size*size) ; cpt++){
        a = cpt % size; // Incrémentation tout les pas de 1 et retour à zéro en size
        b = cpt % (size*size) / size; // Incrémentation tout les pas de size et retour à zéro en size**2
        c = cpt % (size*size*size) / (size*size); // Incrémentation tout les pas de size**2 et retour à zéro en size**3
        d = cpt / (size*size*size); // Incrémentation tout les pas de size**3
        if(d>c && c>b && b>a){
            tmp = calloc(1,sizeof(IAMove));
            tmp->movements = calloc(4,sizeof(SMove));
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

    /**
     * Renvoie la destination obtenue en fonction du joueur, de la distance et du point d'origine
     */
int getDest(int src, int length){
    int dest = 0;
    if(var_globale.me == WHITE){
        dest = src + length;
    }
    else{
        dest = src - length;
    }
    if(dest > 24){
        dest = 24;
    }
    else if(dest < -1){
        dest = -1;
    }

    return dest;
}

    /**
     * Renvoie 1 si l'aiguille est dans la botte de foin
     * 0 sinon
     */
int inArray(int needle,int* haystack, int length){
    int i;
    for(i=0; i<length; i++) {
        if (haystack[i] == needle) {
            return 1;
        }
    }
    return 0;
}

    /**
     * Initialisation de la variable globale
     */
void init_stock_var(stock_var* var)
{
    var->me = NOBODY;
    var->score = 0;
    var->onlyBarUsed = 0;
}









//// ================== [Gestion de la pile]
Pile* createPile(){
    printf("DEBUG : Creating a new Pile\n");
    Pile* tmp = (Pile*)malloc(sizeof(Pile));
    tmp->first = NULL;
    tmp->last = NULL;
    tmp->size = 0;

    return tmp;
}

//// Constructeur de maillon
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
        delete_maillon(tmp);  //libere la memoire
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
    printf("DEBUG : Destroying a Pile\n");
    int i;
    for(i = 0; i < pile->size; i++)
        pop(pile);
    free(pile);
}

int main(){ // Test des fonctions pour un état basique du jeu
    printf("DEBUG : MODE TESTING !");
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
        if(i == 9){
            fakeGame->board[i].owner = WHITE;
            fakeGame->board[i].nbDames = 1;
        }

        if(i == 7){
            fakeGame->board[i].owner = WHITE;
            fakeGame->board[i].nbDames = 1;
        }

        if(i == 11){
            fakeGame->board[i].owner = WHITE;
            fakeGame->board[i].nbDames = 1;
        }
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

    dices[0]=2;
    dices[1]=3;

    PlayTurn(fakeGame,dices,moves,&nbMoves,0);
    free(fakeGame);

    return 0;
}


// GetAllMoveRec()
    /**
     * Récupération de tout les mouvements possible pour chaque case du plateau qui nous appartient
     */
SMove* getAllMove(const SGameState* const gameState, const unsigned char dices[2], int* arraySize){

    printf("DEBUG : Start Recursion\n");
    unsigned int nbMove;
    if(dices[0] == dices[1]){
        nbMove = 4;
    }
    else{
        nbMove = 2;
    }

    SMove* array = calloc(15*nbMove, sizeof(SMove));

    // Struct + Init struct
    ArrayTmp* arrayTmp = malloc(sizeof(ArrayTmp));
    int i,j;
    arrayTmp->size = 0; // Init struct
    for(i=0; i<4; i++){
        arrayTmp->deepness[i].size = 0;
        for(j=0; j<4; j++){
            arrayTmp->deepness[i].done[j] = 0;
        }
    }


    int ite;
    for(ite=0 ; ite < 24 ; ite++){
        if(gameState->board[ite].owner == var_globale.me){
            //
            printf("DEBUG : rec for piece = %d\n",ite);

            getAllMoveRec(gameState, dices, nbMove, ite, ite, 0, arrayTmp, array, arraySize);
            arrayTmp->deepness[0].size = 0;
        }
    }

    printf("DEBUG : number of possible movements = %d\n",*arraySize);

    return array;
}

    /**
     * Algorithme récursif de validation d'un mouvement puis ajout de ce mouvement au tableau
     */
void getAllMoveRec(const SGameState* const gameState, const unsigned char dices[2], int nbMove, int seed, int actPos, int deepness, ArrayTmp* done, SMove* array, int* arraySize){
    if((nbMove - deepness - done->deepness[deepness].size) <= 0){
        printf("DEBUG : End of leaf (%d %d %d)\n", nbMove, deepness, done->deepness[deepness].size);
        return; // Fin d'une feuille
    }

    int dice;
    for(dice = 0; dice < nbMove%3; dice++){
        int dest = getDest(actPos, dices[dice]);
        printf("DEBUG : De en cours = %d\n",dices[dice]);
        if(!inArray(dice, done->deepness[deepness].done, done->deepness[deepness].size)
           && (deepness == 0 || (deepness>0 && dice != done->deepness[deepness-1].done[done->deepness[deepness-1].size-1]))){
            if (gameState->board[dest].owner != var_globale.me &&
                gameState->board[dest].owner != -1 &&
                gameState->board[dest].nbDames >= 2) {
                //NEXT !
            }
            else {
                //
                SMove* move = malloc(sizeof(SMove));
                move->dest_point = (unsigned int) dest + 1;
                move->src_point = (unsigned int) actPos + 1;

                int mov=0;
                while (mov < gameState->board[seed].nbDames &&
                       (mov < nbMove ||(mov < 2 && actPos != seed))) {
                    array[(*arraySize)++] = *move;
                    printf("DEBUG : Adding a move : %d to %d\n",move->src_point, move->dest_point);
                    mov++;
                }
                done->deepness[deepness].done[done->deepness[deepness].size++] = dice;
                if(done->deepness[deepness].done[done->size++] < nbMove){
                    getAllMoveRec(gameState,dices,nbMove,seed,dest,deepness+1,done,array,arraySize);

                    done->deepness[deepness+1].size = 0; // Clear du tableau de profondeur supérieure

                }
            }
        }
    }
    printf("DEBUG : End of branch\n");
    //return; // Fin d'une branche
}

    /**
     * Fonction de libération de la mémoire
     */
void freeAll(IA* ia, SMove* array, IAMove* res){
    Maillon* tmp = ia->movements->first;
    while(tmp->suiv != NULL){

        // FREE MAILLON
        IAMove* tmp2 = tmp->movement;

        free(tmp2->score);
        free(tmp2->movements);
        free(tmp2);

        tmp = tmp->suiv;
        free(tmp->prec);
    }

    free(tmp);
    free(ia->movements);
    free(ia);

    free(array);

    //free(res->movements);
    free(res->score);
    free(res);
}
