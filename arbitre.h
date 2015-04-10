#include <time.h>
#include <stdio.h>
#include <string.h>
#include "backgammon.h"

int IsMatchOver(SGameState *gameState);
int IsGameOver(SGameState *gameState);
void InitBoard(SGameState* gameState);
SGameState* InitState();
unsigned char* rollDice(unsigned char* dice);
int emptyMoves(SMove moves[4], int tailleMoves);
int verif_sens(SMove moves[4], int player, int tailleMoves);
int verif_taille_deplacement(SMove moves[4], int tailleMoves, unsigned char dices[2]);
int is_blocked (int roll, SGameState* gameState, Player player);
int move(SMove moves[4], SGameState* gameState, int tailleMoves, int player, unsigned char dices[2]);
