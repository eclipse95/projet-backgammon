#include <time.h>
#include <stdio.h>
#include <string.h>
#include "backgammon.h"

int IsMatchOver(SGameState *gameState);
int IsGameOver(SGameState *gameState);
SGameState* InitBoard();
SGameState* InitState();
unsigned int* rollDice(unsigned int* dice);
SMoves[2] emptyMoves(SMoves moves); //attention !!!!!!! que se passe-t-il si moves fait 4 cases !!!!!!!!!
