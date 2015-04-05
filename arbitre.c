#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backgammon.h"
#include <math.h>

int IsMatchOver(SGameState *gameState)
{
	if (gameState->bar[BLACK]==15) { return BLACK; } /*pions noirs sur last case = 15*/
	else if (gameState->bar[WHITE]==15) { return WHITE; } /*pions blancs sur last case = 15*/
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
	gameState->bar[WHITE].nbDames=0;	// bar n'a pas d'attribut
	gameState->bar[WHITE].owner=NOBODY;	// bar n'a pas d'attribut
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
unsigned char* rollDice(unsigned char* dice)
{
	int i;
	for(i=0;i<2;i++)
	{
		dice[i]=(rand() % 6)+1;
	}
	return dice;
}


SMoves* emptyMoves(SMoves* moves, int tailleMoves) //on vide les moves après chaque mouvement effectué
{
	if (moves != NULL)
	{
		int i;
	    	for (i=0; i<tailleMoves; i++)
	    	{
	        	moves[i]->src_point = 0;
	        	moves[i]->dest_point = 0;
	    	}
	}
	return moves;
}


int verif_sens(SMoves* moves, int player, int tailleMoves)
{
    int i;
    for (i=0; i<tailleMoves; i++) 
    {
    	if (player==BLACK)
    	{
    		if (moves[i]->dest_point - moves[i]->src_point > 0)
    		{
    		    printf("Erreur de sens");
    	        return -1;  // on vérifie que le pion ne se déplace pas dans le sens inverse
    		}
    	}
    	else if (player==WHITE)
    	{
    		if (moves[i]->dest_point - moves[i]->src_point < 0)
    		{
    		    printf("Erreur de sens !\n");
    	        return -1;  // on vérifie que le pion ne se déplace pas dans le sens inverse
    		}
    	}
    }
    return 0;
}

int verif_taille_deplacement(SMoves * moves, int tailleMoves, unsigned char dices[2])
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


int move(SMoves* moves, SGameState* gameState, int tailleMoves, int player)
{
	int i;
	for (i=0; i<tailleMoves; i++)
	{
	    if (moves[i]->src_point > 0 && moves[i]->src_point < 25) // si le pion est dans le tableau
        {
            gameState->board[moves[i]->src_point-1].nbDames--; // on enlève un pion de la pile source
        }
	    else if (moves[i]->src_point == 0) // retour en jeu d'un pion en sortie
	    {
	        gameState->out[player]--; 
	    }
	    else // mouvement en dehors du tableau donc on le signale
	    {
	        printf("Tentative de mouvement depuis en dehors du tableau !\n");
	        return -1;
	    }
	    
	    if (moves[i]->dest_point > 0 && moves[i]->dest_point < 25) // si le pion est dans le tableau
	    {
    		if (gameState->board[moves[i]->dest_point-1].owner != player && gameState->board[moves[i]->dest_point-1].nbDames==1) // si on peut prendre la case
    		{
    			gameState->out[gameState->board[moves[i].dest_point-1].owner]++; // ajout à la case destination
    			gameState->board[moves[i].dest_point-1].owner=player; // propriétaire de la case change
    		}
    		else if (gameState->board[moves[i]->dest_point-1].owner == NOBODY && gameState->board[moves[i]->dest_point-1].nbDames==0) // 
    		{
    		    gameState->board[moves[i].dest_point-1].nbDames++;
    			gameState->board[moves[i].dest_point-1].owner=player; // prise d'une case libre
    		}
    		else if (gameState->board[moves[i]->dest_point].owner == player) // on ajoute le pion à la case
    		{
    			gameState->board[moves[i]->dest_point].nbDames++;
    		}
    		else // cas où on envoie sur une case où il y a au moins 2 pions adverses
    		{
    		    printf("Problème de vérification : mouvement impossible !\n"); 
    		    return -1;
    		}
	    }
	    else if (moves[i]->dest_point == 25) // on envoie sur la case bar
	    {
	        gameState->bar[player]++;
	    }
	    else
	    {
	        printf("Tentative de mouvement vers en dehors du tableau !\n");
	         return -1;
	    }
	}
	return 0;
}
