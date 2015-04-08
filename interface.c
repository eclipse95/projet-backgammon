#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backgammon.h"
#include <SDL2/SDL.h>
#include <math.h>

// MODIF POSSIBLE : Passer le tableau de SMove en paramètre parce que le code est hyper gourmand ; Je sais pas si vous voulez utiliser ça donc je m'avance pas
// Possibles causes de bug :
// - cast de unsigned char vers unsigned int pour faire les additions : possible ?
// - tableau de SMove initialisé à ???, je ne sais pas trop comment rajouter un SMove dans le tableau
SMove* getAllPossibleMoves(SGameState *gameState, int player, unsigned char dice[2], SMove* moves) // fonction qui retourne tous les moves que peut faire un joueur / une IA en fonction de son ID et de ses dés
{
	unsigned int i,j,destination;
	unsigned int k=0;
	for(i=0;i<24;i++) // on check chaque case
	{
		if(gameState->board[i].owner==player) // on check seulement pour les pions que le joueur renseigné possède
		{
			for(j=0;j<2;j++) // on regarde pour les 2 dés;
			{
				if(player==0)
				{ 
					destination = i - (unsigned int)dice[j]; // point d'arrivée en partant de i vers le i-le résultat du dé (car dans l'autre sens pour joueur noir
				}
				else
				{
					destination = i + (unsigned int)dice[j]; // point d'arrivée en partant de i vers le i+le résultat du dé
				}
				if(destination <= 25) // on vérifie qu'on ne sort pas du board
				{
					// A AMELIORER : ON NE SAIT PAS ENCORE SI ON PEUT METTRE DES PIONS A L'ARRIVEE
					if(gameState->board[destination].owner==-1 || gameState->board[destination].owner==player)
					{
						moves[k].src_point=i;
						moves[k].dest_point=destination;
						k++;
					}
				}
			}
		}
	}
	return moves;
}

int getXSquare(int id)
{
	return 0;
}
int getYSquare(int id)
{
	return 0;
}
void drawSquare(SGameState *gameState, int id)
{
	int x=getXSquare(id);
	int y=getYSquare(id);
}
void drawMultiplier(SGameState *gameState)
{
	return 0;
}
void drawDice(SGameState *gameState,unsigned char dice[2])
{
	return 0;
}
void drawAll(SGameState *gameState)
{
	int i;
	for(i=0;i<24;i++)
	{
		drawSquare(gameState,i);
	}
	drawMultiplier(gameState);
	drawDice(gameState);
}

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

	/* Création de la fenêtre */
	SDL_Window* pWindow = NULL;
	pWindow = SDL_CreateWindow("Backgammon",SDL_WINDOWPOS_UNDEFINED,
															  SDL_WINDOWPOS_UNDEFINED,
															  800,
															  600,
															  SDL_WINDOW_SHOWN);

	SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow,-1,SDL_RENDERER_ACCELERATED); // Création
	SDL_Surface* pSprite = SDL_LoadBMP("./img/backgammon.bmp");
	if(pSprite==NULL)
	{
		printf("BMP loading failed : %s\n",SDL_GetError());
		return 1;
	}
	
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer,pSprite); 
	SDL_Rect dest = { 0,0, pSprite->w, pSprite->h};
	SDL_RenderCopy(pRenderer,pTexture,NULL,&dest); // Copie du sprite
	SDL_RenderPresent(pRenderer); // Affichage
	SDL_Delay(3000);
	
	SDL_DestroyTexture(pTexture); // Libération de la mémoire
	SDL_FreeSurface(pSprite); // Libération de la ressource
	SDL_DestroyRenderer(pRenderer); // Libération de la mémoire

    SDL_Quit();

    return 0;
}
