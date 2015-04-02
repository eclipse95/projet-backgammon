#include <time.h>
#include <stdio.h>
#include <string.h>
#include "backgammon.h"

int IsMatchOver(SGameState *gameState);
int IsGameOver(SGameState *gameState);
void InitBoard(SGameState* gameState);
SGameState* InitState();
unsigned int* rollDice(unsigned int* dice);
SMoves* emptyMoves(SMoves* moves, int tailleMoves);
SGameState* move(SMoves* moves, SGameState gameState, int tailleMoves);
