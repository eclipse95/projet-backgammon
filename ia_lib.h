
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

typedef struct IA IA;

// custom struct ?
struct IA
{
    SMove* movements;

} ;

void InitLibrary(char name[50]);

void StartMatch(const unsigned int target_score);

void StartGame(Player p);

void EndGame();

void EndMatch();

int DoubleStack(const SGameState * const gameState);

int TakeDouble(const SGameState * const gameState);

void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries);
