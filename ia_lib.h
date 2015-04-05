#include "pile.h"

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
	unsigned int src_point;
	unsigned int dest_point;
} SMove;


void InitLibrary(char name[50]);

void StartMatch(const unsigned int target_score);

void StartGame(Player p);

void EndGame();

void EndMatch();

int DoubleStack(const SGameState * const gameState);

int TakeDouble(const SGameState * const gameState);

void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries);



// custom struct and functions
typedef struct				// <!> A modifier
{
	Pile* movements;		// Changer le type
	int nbMoves;
} IAMove;


typedef struct{
    Pile* movements;
    int size;
}IA;

typedef struct
{
	int score;
	Player me;
} stock_var;


void init_stock_var(stock_var* var);

IA* getAllMovements(const SGameState* const gameState, const unsigned char dices[2]);

IA* getAllScores(const SGameState* const gameState, IA* allMovements);

int getGlobalScore(const SGameState* const gameState); // not finished

IAScore* getScore(const SGameState* const gameState, IAMove* moves);

IAMove* getBest(IA* allMovements);

Pile* combination1(SMove* array, int size);

Pile* combination2(SMove* array, int size);

Pile* combination4(SMove* array, int size);



// TODO Nouvelles fonctions à ajouter
// TODO Implémenter struct IAScore et IAMove