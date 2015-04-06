#include <time.h>
#include <stdio.h>
#include <string.h>
#include "backgammon.h"

int IsMatchOver(SGameState *gameState);
int IsGameOver(SGameState *gameState);
void InitBoard(SGameState* gameState);
SGameState* InitState();
unsigned char* rollDice(unsigned char* dice);
SMove* emptyMoves(SMove* moves, int tailleMoves);
SGameState* move(SMove* moves, SGameState gameState, int tailleMoves);
