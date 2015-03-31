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
  
  int i;
  for (i=0; i<12; i++)
  {
  	switch(i)
  	{
  	case 0 :
  		gameState->board[i].nbDames=2;
  		gameState->board[i].owner=WHITE;
  		gameState->board[23-i].nbDames=2;
  		gameState->board[23-i].owner=BLACK;
  		break;
  	case 5 :
  		gameState->board[i].nbDames=5;
  		gameState->board[i].owner=BLACK;
  		gameState->board[23-i].nbDames=5;
  		gameState->board[23-i].owner=WHITE;
  		break;
   	case 7 :
    		gameState->board[i].nbDames=3;
    		gameState->board[i].owner=BLACK;
  		gameState->board[23-i].nbDames=3;
  		gameState->board[23-i].owner=WHITE;
  		break;
  	case 11 :
   		gameState->board[i].nbDames=5;
   		gameState->board[i].owner=WHITE;
  		gameState->board[23-i].nbDames=5;
  		gameState->board[23-i].owner=BLACK;
  		break;
  	default:
  		gameState->board[i].nbDames=0;
  		gameState->board[i].owner=NOBODY;
  		gameState->board[23-i].nbDames=0;
    		gameState->board[23-i].owner=NOBODY;
  		break;
  	}
  }
  gameState->bar[WHITE].nbDames=0;
  gameState->bar[WHITE].owner=NOBODY;
  gameState->bar[BLACK].nbDames=0;
  gameState->bar[BLACK].owner=NOBODY;
  gameState->out[WHITE].nbDames=0;
  gameState->out[WHITE].owner=NOBODY;
  gameState->out[BLACK].nbDames=0;
  gameState->out[BLACK].owner=NOBODY;
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

SMoves[2] emptyMoves(SMoves moves, int player)
{
	moves[player].src_point=0;
	moves[player].dest_point=0;
	return moves;
}
