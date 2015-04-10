

#ifndef PROJET_BACKGAMMON_LIBRAIRY_H
#define PROJET_BACKGAMMON_LIBRAIRY_H

#endif //PROJET_BACKGAMMON_LIBRAIRY_H

// Version finale

typedef enum {
    BLACK = 0,
    NOBODY = -1,
    WHITE = 1
} Player;

typedef struct {
    Player owner;           /*!<  Joueur contrôlant la case */
    unsigned int nbDames;   /*!<  Nombre de dames sur la case (non utilisé si `owner == NOBODY.`) */
} Square;

typedef struct {
    Square board[24];           /*!< Les 24 cases du tableau, en partant du bord blanc. */
    unsigned int bar[2];
    unsigned int out[2];
    unsigned int whiteScore;    /*!< Score du joueur blanc */
    unsigned int blackScore;    /*!< Score du joueur noir */
    unsigned int turn;          /*!< Numéro du tour en cours (initialisé à 0) */
    unsigned int stake;			/*!< Mise courante de la partie */
} SGameState;

typedef struct
{
    unsigned int src_point;     /*0 = Bar, [1,23] = terrain, 24 = home*/
    unsigned int dest_point;
} SMove;


typedef struct Maillon Maillon;
typedef struct
{
    int score;
    int notSafe;
} IAScore ;

// IAMove
typedef struct
{
    SMove* movements;
    int nbMoves;
    IAScore* score;
} IAMove;

// Pile
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

typedef struct{
    Pile* movements;
    int size;
}IA;

typedef struct
{
    int score;          /*<! Score à attendre */
    Player me;          /*<! Couleur de l'ia */
    int onlyBarUsed;    /*<! Témoin si jeton sur la bar */
    int readyToFinish;  /*<! Vrai si les pions sont en disposition de rentrer à la maison */
} stock_var;

typedef struct{
    int values[24];
} Dictionnary;

// Structure pour la récursion
/* Structure pour stocker les feuilles de la récursion */
typedef struct{
    int done[4];
    int size;
} ArrayTmpDeep;

/* Structure pour stocker les branches de la récursion */
typedef struct{
    ArrayTmpDeep deepness[4];
    int size;
} ArrayTmp;

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

// Dictionnary
Dictionnary* createDico();
void resetDico(Dictionnary* dico);
int isPossible(const SGameState* const gameState, Dictionnary* dico, SMove* key);


// std function

void InitLibrary(char name[50]);

void StartMatch(const unsigned int target_score);

void StartGame(Player p);

void EndGame();

void EndMatch();

int DoubleStack(const SGameState * const gameState);

int TakeDouble(const SGameState * const gameState);

void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries);



// Fonction pour les structures non API
/* Fonction d'initialisation de la structure contenant les vars globales*/

void init_stock_var(stock_var* var);

IA* getAllMovements(const SGameState* const gameState, const unsigned char dices[2], SMove* array);

IA* getAllScores(const SGameState* const gameState, IA* allMovements);

int getGlobalScore(const SGameState* const gameState);

IAScore* getScore(const SGameState* const gameState, IAMove* moves);

IAMove* getBest(IA* allMovements);

/* Renvoi un mouvement possible */
Pile* combination1(SMove* array, int size);

/* renvoi combinaison de 2 mouvements*/
Pile* combination2(SMove* array, int size, const SGameState * const gameState);

/* renvoi combinaison de 4 mouvements */
Pile* combination4(SMove* array, int size);

/* Renvoi la case à la distance spécifiée de la source */
int getDest(int src, int length);

/* Vérifie la présence d'un entier dans un tableau */
int inArray(int needle,int* haystack, int length);

/* Fonction d'initialisation la génération des mouvements */
SMove* getAllMove(const SGameState* const gameState, const unsigned char dices[2], int* arraySize);

/* Fonction permetant génération des mouvements possibles */
void getAllMoveRec(const SGameState* const gameState, const unsigned char dices[2], int nbMove, int seed, int actPos, int deepness, ArrayTmp* done, SMove* array, int* arraySize);

/* Fonction pour libérer toutes les structures de PlayTurn() */
void freeAll(IA* ia, SMove* array, IAMove* res);
