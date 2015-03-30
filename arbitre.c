#include <time.h>
#include <stdio.h>
#include <string.h>
#include "backgammon.h"

srand((unsigned) time(&t)); //permet de créer une nouvelle seed radom (donc de "relancer" le tableau à chaque compilation)

int IsMatchOver(SGameState *gameState)
{
  if (gameState->bar[BLACK]==15)/*pions noirs sur last case = 15*/
  { return 0; }
  else if (gameState->bar[WHITE]==15)/*pions blancs sur last case = 15*/
  { return 1; }
  else
  { return -1; }
}

int IsGameOver(SGameState *gameState)
{
  if (gameState->whiteScore==5)
  { return WHITE; }
  else if (gameState->blackScore==5)
  { return BLACK; }
  else
  { return NOBODY; }
}

SGameState* InitState()              /*  12 ---- 23
					 11 ---- 0	  VOIR IMAGE WIKIPEDIA */
{
  SGameState* gameState = (SGameState*) malloc(sizeof(SGameState));	//allocation mémoire	//penser au free()
  gameState->board[0].owner=WHITE;
  gameState->board[0].nbDames=2;
  gameState->board[11].owner=WHITE;
  gameState->board[11].nbDames=5;
  gameState->board[16].owner=WHITE;
  gameState->board[16].nbDames=3;
  gameState->board[18].owner=WHITE;
  gameState->board[18].nbDames=5;
  gameState->board[5].owner=BLACK;
  gameState->board[5].nbDames=5;
  gameState->board[7].owner=BLACK;
  gameState->board[7].nbDames=3;
  gameState->board[12].owner=BLACK;
  gameState->board[12].nbDames=5;
  gameState->board[23].owner=BLACK;
  gameState->board[23].nbDames=2;
  gameState->whiteScore=0;
  gameState->blackScore=0;
  gameState->turn=0;
  gameState->stake=1;
  return gameState;
}

// rollDice prend en paramètre le nombre de dés à lancer et renvoie un pointeur vers un array de résultats en unsigned int
unsigned int* rollDice(int nbDice)
{
	unsigned int *roll=malloc(sizeof(unsigned int)*nbDice); //penser a free à la fin du main ptet
	int i;
	for(i=0;i<nbDice;i++)
	{
		roll[i]=(rand() % 5)+1;
	}
	return roll;
}
