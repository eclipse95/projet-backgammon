#include <time.h>
#include <stdio.h>
#include <string.h>
#include "backgammon.h"

int IsMatchOver(SGameState *gameState)
{
  if (gameState->bar[0]==15)/*pions noirs sur last case = 15*/
  {
	return 0;
  }
  else if (gameState->bar[1]==15)/*pions blancs sur last case = 15*/
  {
	return 1;
  }
  else
  {
	return -1;
  }
}

int IsGameOver(SGameState *gameState)
{
  if (gameState->whiteScore==3)
  {
	return 0;
  }
  else if (gameState->blackScore==3)
  {
	return 1;
  }
  else
  {
	return -1;
  }
}

SGameState* InitState()              /*  12 ---- 23
					 11 ---- 0	  VOIR IMAGE WIKIPEDIA */
{
  SGameState* gameState = (SGameState*) malloc(sizeof(SGameState));	//allocation mémoire	//penser au free()
  gameState->board[0].owner=1;
  gameState->board[0].nbDames=2;
  gameState->board[11].owner=1;
  gameState->board[11].nbDames=5;
  gameState->board[16].owner=1;
  gameState->board[16].nbDames=3;
  gameState->board[18].owner=1;
  gameState->board[18].nbDames=5;
  gameState->board[5].owner=0;
  gameState->board[5].nbDames=5;
  gameState->board[7].owner=0;
  gameState->board[7].nbDames=3;
  gameState->board[12].owner=0;
  gameState->board[12].nbDames=5;
  gameState->board[23].owner=0;
  gameState->board[23].nbDames=2;
  gameState->whiteScore=0;
  gameState->blackScore=0;
  gameState->turn=0;
  gameState->stake=1;

  return gameState;
}
