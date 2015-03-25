#include <time.h>
#include <stdio.h>
#include <string.h>
#include "backgammon.h"

int IsMatchOver(SGameState *gameState)
{
  if (/*pions blancs sur last case = 15)*/)
  {
	return 0;
  }
  else if (/*pions noirs sur last case = 15)*/)
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
  if (gameState.whiteScore==3)
  {
	return 0;
  }
  else if (gameState.blackScore==3)
  {
	return 1;
  }
  else
  {
	return -1;
  }
}

SGameState InitState()              /*  24 ---- 13
					                	             1 ---- 12	*/
{
  SGameState gameState;
  gameState->

  return gameState;
}
