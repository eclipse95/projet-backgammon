

#ifndef PROJET_BACKGAMMON_LIBRAIRY_H
#define PROJET_BACKGAMMON_LIBRAIRY_H

#endif //PROJET_BACKGAMMON_LIBRAIRY_H


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
    unsigned int src_point; /*0 = Bar, [1,23] = terrain, 24 = home*/
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
    int score;
    Player me;
    int onlyBarUsed;
} stock_var;

typedef struct{
    int values[24];
} Dictionnary;




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

IAMove* create_IAMove();

void delete_IAMove(IAMove*);

void realloc_IAMove(IAMove*, int);

// Dictionnary
Dictionnary* createDico();
void resetDico(Dictionnary* dico);
int isPossible(const SGameState* const gameState, Dictionnary* dico, int key);


// std function

void InitLibrary(char name[50]);

void StartMatch(const unsigned int target_score);

void StartGame(Player p);

void EndGame();

void EndMatch();

int DoubleStack(const SGameState * const gameState);

int TakeDouble(const SGameState * const gameState);

void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries);



// custom struct and functions



void init_stock_var(stock_var* var);

IA* getAllMovements(const SGameState* const gameState, const unsigned char dices[2]);

IA* getAllScores(const SGameState* const gameState, IA* allMovements);

int getGlobalScore(const SGameState* const gameState); // not finished

IAScore* getScore(const SGameState* const gameState, IAMove* moves); // probleme type

IAMove* getBest(IA* allMovements);

Pile* combination1(SMove* array, int size);

Pile* combination2(SMove* array, int size, const SGameState * const gameState);

Pile* combination4(SMove* array, int size);
