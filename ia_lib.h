
void InitLibrary(char name[50]);

void StartMatch(const unsigned int target_score);

void StartGame(Player p);

void EndGame();

void EndMatch();

int DoubleStack(const SGameState * const gameState);

int TakeDouble(const SGameState * const gameState);

void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries);
