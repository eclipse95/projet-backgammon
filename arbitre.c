#include <time.h>
#include <stdio.h>
#include <string.h>
#include "backgammon.h"

srand((unsigned) time(&t)); //permet de créer une nouvelle seed random (donc de "relancer" le tableau à chaque compilation)

int IsMatchOver(SGameState *gameState)
{
  if (gameState->bar[BLACK]==15)/*pions noirs sur last case = 15*/
  { return BLACK; }
  else if (gameState->bar[WHITE]==15)/*pions blancs sur last case = 15*/
  { return WHITE; }
  else
  { return NOBODY; }
}

int IsGameOver(SGameState *gameState)
{
  if (gameState->blackScore==5)
  { return BLACK; }
  else if (gameState->whiteScore==5)
  { return WHITE; }
  else
  { return NOBODY; }
}


void InitBoard(SGameState* gameState)
{
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
	 gameState->stake=1;
	 gameState->turn=0;
}


SGameState* InitState()              /*  12 ---- 23
					 11 ---- 0	  VOIR IMAGE WIKIPEDIA */
{
  SGameState* gameState = (SGameState*) malloc(sizeof(SGameState));	//allocation mémoire
  InitBoard(gameState);
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
  return gameState;
}

// rollDice prend en paramètre le nombre de dés à lancer et renvoie un pointeur vers un array de résultats en unsigned int
unsigned int* rollDice(unsigned int* dice)
{
	int i;
	for(i=0;i<2;i++)
	{
		dice[i]=(rand() % 6)+1;
	}
	return dice;
}

SMoves[2] emptyMoves(SMoves moves) //on vide les moves après chaque mouvement effectué //attention si tableau de 4
{
	moves[0].src_point=0;
	moves[0].dest_point=0;
	moves[1].src_point=0;
	moves[1].dest_point=0;
	return moves;
}
