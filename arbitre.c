#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backgammon.h"
#include <math.h>
//besoin de math dans le makefile pour abs

int IsMatchOver(SGameState *gameState)
{
	if (gameState->out[BLACK]==15) { return BLACK; } /*pions noirs sur last case = 15*/
	else if (gameState->out[WHITE]==15) { return WHITE; } /*pions blancs sur last case = 15*/
	else { return NOBODY; }
}

int IsGameOver(SGameState *gameState)
{
	if (gameState->blackScore==5) { return BLACK; }
	else if (gameState->whiteScore==5) { return WHITE; }
	else { return NOBODY; }
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
	gameState->bar[WHITE] = 0;	
	gameState->bar[BLACK] = 0;
	gameState->out[WHITE] = 0;
	gameState->out[BLACK] = 0;
	gameState->whiteScore = 0;
	gameState->blackScore = 0;
	return gameState;
}

unsigned char* rollDice(unsigned char* dice)
{
	int i;
	for(i=0;i<2;i++)
	{
		dice[i]=(rand() % 6)+1;		// peut-être cast nécessaire
	}
	return dice;
}


SMove* emptyMoves(SMove moves[4], int tailleMoves) //on vide les moves après chaque mouvement effectué
{
	if (moves != NULL)
	{
		int i;
		for (i=0; i<tailleMoves; i++)
		{
			moves[i].src_point = 0;
			moves[i].dest_point = 0;
		}
	}
	return moves;
}


int verif_sens(SMove moves[4], int player, int tailleMoves)
{
	int i;
	for (i=0; i<tailleMoves; i++) 
	{
		if (player==BLACK)
		{
			if (moves[i].dest_point - moves[i].src_point > 0)
			{
				printf("Erreur de sens");
				return -1;  // on vérifie que le pion ne se déplace pas dans le sens inverse
			}
		}
		else if (player==WHITE)
		{
			if (moves[i].dest_point - moves[i].src_point < 0)
			{
				printf("Erreur de sens !\n");
				return -1;  // on vérifie que le pion ne se déplace pas dans le sens inverse
			}
		}
	}
	return 0;
}

int verif_taille_deplacement(SMove moves[4], int tailleMoves, unsigned char dices[2])
{
	int i, j;
	int nb_des;
	if (tailleMoves==4) //cas où les dès sont identiques
	{
		for (i=0; i<tailleMoves; i++) // pour chacun des dès, on regarde si on a bien fait un déplacement correspondant au dé
		{
			if (abs(moves[i].src_point - moves[i].dest_point) != dices[0])
			{
				printf("Erreur de taille de déplacement !\n");
				return -1;
			}
		}
	}
	else // cas où 2 dès différents
	{
		for (i=0; i<tailleMoves; i++)
		{
			nb_des=0;
			for (j=0; j<tailleMoves; j++)
			{
				if (abs(moves[i].src_point - moves[i].dest_point) != dices[j]) // si on a bien fait un déplacement correspondant au dé
				{
					nb_des++;
				}
			}
			if ( nb_des>=2)
			{
				printf("Erreur de taille de déplacement !\n");
				return -1;
			}
		}
		if (abs(moves[0].src_point - moves[0].dest_point) == abs(moves[1].src_point - moves[1].dest_point))
		{
			printf("Erreur de taille de déplacement !\n");
			return -1;
		}
	}
	return 0;
}

int is_blocked (int roll, SGameState* gameState, Player player)
{
	if (player == BLACK)
	{
		for (i=roll; i<24 ; i++)
		{
			if (gameState->board[i].owner == player) //pour chaque case du joueur
			{
				if (gameState->board[i-roll].owner == player || (gamestate->board[i-roll].nbDames==0 && gamestate->board[i-roll].owner==NOBODY) || (gamestate->board[i-roll].nbDames==1 && gamestate->board[i-roll].owner==WHITE) ) //on verifie qu'il peut faire un coup a partir de cette case
				{
					return 0;
				}
			}
		}
		if (gameState->bar[player]!=0) //de meme s'il a des pions out
		{
			if (gameState->board[24-roll].owner==player || (gamestate->board[24-roll].nbDames==0 && gamestate->board[24-roll].owner==NOBODY) || (gamestate->board[24-roll].nbDames==1 && gamestate->board[24-roll].owner==WHITE) )
				{
					return 0;
				}
		}
		for (i=0; i<roll ; i++)
		{
			if (gameState->board[i].owner==player) //pour chaque case du joueur
			{
				return 0;
			}
		}
	}
	else if(player == WHITE)
	{
		for (i=0; i<24-roll ; i++)
		{
			if (gameState->board[i].owner==player) //pour chaque case du joueur
			{
				if (gameState->board[i+roll].owner==player || (gamestate->board[i+roll].nbDames==0 && gamestate->board[i+roll].owner==NOBODY) || (gamestate->board[i+roll].nbDames==1 && gamestate->board[i+roll].owner==BLACK) ) //on verifie qu'il peut faire un coup a partir de cette case
				{
					return 0;
				}
			}
		}
		if (gameState->bar[player]!=0) //de meme s'il a des pions dans bar
		{
			if (gameState->board[roll-1].owner==player || (gamestate->board[roll-1].nbDames==0 && gamestate->board[roll-1].owner==NOBODY) || (gamestate->board[roll-1].nbDames==1 && gamestate->board[roll-1].owner==BLACK) )
				{
					return 0;
				}
		}
		for (i=24-roll; i<25 ; i++)
		{
			if (gameState->board[i].owner==player) //pour chaque case du joueur
			{
				return 0;
			}
		}
	}
	return 1;
}

int move(SMove moves[4], SGameState* gameState, int tailleMoves, int player, unsigned char dices[2])
{
	int i;
	if (verif_sens(moves, player, tailleMoves)==0 && verif_taille_deplacement(moves, tailleMoves, dices)==0)
	{
		for (i=0; i<tailleMoves; i++)
		{
			if (is_blocked(abs(moves[i].src_point-moves[i].dest_point), gameState, player)!=1) //si le joueur n'est pas bloqué, on effectue 1 mouvement, puis on revérifie pour le suivant
			{
				if (moves[i].src_point > 0 && moves[i].src_point < 25) // si le pion est dans le tableau
				{
					gameState->board[moves[i].src_point-1].nbDames--; // on enlève un pion de la pile source
				}
				else if (moves[i].src_point == 0) // retour en jeu d'un pion en sortie
				{
					gameState->bar[player]--; 
				}
				else // mouvement en dehors du tableau donc on le signale
				{
					printf("Tentative de mouvement depuis en dehors du tableau !\n");
					return -1;
				}
        
				if (moves[i].dest_point > 0 && moves[i].dest_point < 25) // si le pion est dans le tableau
				{
					if (gameState->board[moves[i].dest_point-1].owner != player && gameState->board[moves[i].dest_point-1].nbDames==1) // si on peut prendre la case
					{
						gameState->bar[gameState->board[moves[i].dest_point-1].owner]++; // ajout à la case destination
						gameState->board[moves[i].dest_point-1].owner = player; // propriétaire de la case change
					}
					else if (gameState->board[moves[i].dest_point-1].owner == NOBODY && gameState->board[moves[i].dest_point-1].nbDames==0) //
					{
						gameState->board[moves[i].dest_point-1].nbDames++;
						gameState->board[moves[i].dest_point-1].owner = player; // prise d'une case libre
					}
					else if (gameState->board[moves[i].dest_point].owner == player) // on ajoute le pion à la case
					{
						gameState->board[moves[i].dest_point].nbDames++;
					}
					else // cas où on envoie sur une case où il y a au moins 2 pions adverses
					{
						printf("Problème de vérification : mouvement impossible !\n"); 
						return -1;
					}
				}
				else if (moves[i].dest_point == 25) // on envoie sur la case bar
				{
					gameState->out[player]++;
				}
				else
				{
					printf("Tentative de mouvement vers en dehors du tableau !\n");
					return -1;
				}
			}
			else { return 0; }
		}
	}
	return 0;
}
