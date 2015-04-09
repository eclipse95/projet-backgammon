#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backgammon.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <assert.h>

#define TAILLE_PION 40
#define W_DICE 39
#define H_DICE 41
#define X_WINDOW 800
#define Y_WINDOW 600
#define FONT "Sans.ttf"
#define FONT_SIZE 24

/* TODO
 * transparence ? sans tout changer si possible. A essayer : setcolorkey sur les surfaces dans le loader
 * display dice
 * display stake
 * display out
 * display bar
 */

/* A VIRER QUAND L'ARBITRE MARCHE */
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

// getXSquare et getYSquare retournent la position du premier pion à afficher en fonction de la case renseignée
int getXSquare(int id)
{
	if(id>=18 && id<=23)
	{
		return (392 + 60*(23-id));
	}
	if(id>=12 && id<=17)
	{
		return (18 + 60*(17-id));
	}
	if(id>=6 && id<=11)
	{
		return (321 - 60*(12-id));
	}
	if(id>=0 && id<=5)
	{
		return (695 - 60*(5-id));
	}
	return 0;
}
int getYSquare(int id)
{
	if(id>=12 && id<=23)
	{
		return 16;
	}
	if(id>=0 && id<=11)
	{
		return 535;
	}
	return 0;
}

// dessine tous les pions de la case id
void drawSquare(SGameState *gameState, int id, SDL_Texture* tabSprite[], SDL_Renderer* renderer)
{
	int x=getXSquare(id);
	int y=getYSquare(id);
	printf("drawsquare x:%i y:%i\n",x,y);
	int i,multiplier;
	if(id>=0 && id<=11){multiplier=-1;}
	if(id>=12 && id<=23){multiplier=1;}
	SDL_Texture* texture=NULL;
	Square square=gameState->board[id];
	if(x==0 || y==0){printf("coordonnées introuvables");return;}
	switch(square.owner)
	{
		case BLACK:
			printf("case black\n");
			texture=tabSprite[1];
			break;
		
		case WHITE:
			printf("case white\n");
			texture=tabSprite[2];
			break;
			
		case NOBODY:
			printf("case nobody\n");
			return;
			break;
	}
	for(i=0;i<square.nbDames;i++)
	{
		printf("Tour de boucle affiche dames %i\n",i);
		SDL_Rect dest={x,y+i*TAILLE_PION*multiplier,TAILLE_PION,TAILLE_PION}; 752 326
		SDL_RenderCopy(renderer,texture,NULL,&dest);
	}
}
// dessine le texte text à l'endroit x et y
void drawText(int x,int y, char* text)
{
	TTF_Font* Sans = TTF_OpenFont(FONT, FONT_SIZE); //this opens a font style and sets a size
	SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "put your text here", White);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); 
}
// dessine le videau
void drawMultiplier(SGameState *gameState,SDL_Texture* tabSprite[], SDL_Renderer* renderer)
{
	printf("drawmultiplier\n");
}

// dessine les dés
void drawDice(SGameState *gameState,unsigned char dice[2],SDL_Texture* tabSprite[], SDL_Renderer* renderer)
{
	SDL_Rect dest = {753,223,W_DICE,H_DICE};
	SDL_RenderCopy(renderer,tabSprite[(int)dice[0]+2],NULL,&dest);
	SDL_Rect dest2 = {752,326,W_DICE,H_DICE};
	SDL_RenderCopy(renderer,tabSprite[(int)dice[1]+2],NULL,&dest2);
	printf("drawdice\n");
}

// dessine tout
void drawAll(SGameState *gameState, SDL_Texture* tabSprite[], SDL_Renderer* renderer)
{
	printf("drawall\n");
	int i;
	unsigned char dice[2]; // à enlever plus tard
	for(i=0;i<24;i++)
	{
		printf("tour boucle drawsquare %i\n",i);
		drawSquare(gameState,i,tabSprite,renderer);
	}
	drawMultiplier(gameState,tabSprite,renderer);
	drawDice(gameState,dice,tabSprite,renderer);
}
// fonction custom pour automatiser le process de création de textures
SDL_Texture* textureLoader(char* filename,SDL_Surface* surfaceLoader, SDL_Renderer* renderer)
{
	printf("load texture %s\n",filename);
	SDL_Texture* returnValue;
	surfaceLoader=SDL_LoadBMP(filename);
	returnValue=SDL_CreateTextureFromSurface(renderer,surfaceLoader);
	SDL_FreeSurface(surfaceLoader);
	SDL_SetTextureBlendMode(returnValue, SDL_BLENDMODE_BLEND);
	return returnValue;
}

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }
	int i;
	SGameState* gameState=malloc(sizeof(SGameState));
	InitBoard(gameState);
	/* Création de la fenêtre */
	SDL_Window* pWindow = NULL;
	pWindow = SDL_CreateWindow("Backgammon",SDL_WINDOWPOS_UNDEFINED,
															  SDL_WINDOWPOS_UNDEFINED,
															  X_WINDOW,
															  Y_WINDOW,
															  SDL_WINDOW_SHOWN);

	SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow,-1,SDL_RENDERER_ACCELERATED); // Init renderer
	// Bloc de génération initiale des textures à utiliser
	SDL_Texture* ptabSprite[9];
	SDL_Surface* surfaceLoader;
	ptabSprite[0]=textureLoader("./img/backgammon.bmp",surfaceLoader,pRenderer);
	ptabSprite[1]=textureLoader("./img/black.bmp",surfaceLoader,pRenderer);
	ptabSprite[2]=textureLoader("./img/white.bmp",surfaceLoader,pRenderer);
	ptabSprite[3]=textureLoader("./img/1.bmp",surfaceLoader,pRenderer);
	ptabSprite[4]=textureLoader("./img/2.bmp",surfaceLoader,pRenderer);
	ptabSprite[5]=textureLoader("./img/3.bmp",surfaceLoader,pRenderer);
	ptabSprite[6]=textureLoader("./img/4.bmp",surfaceLoader,pRenderer);
	ptabSprite[7]=textureLoader("./img/5.bmp",surfaceLoader,pRenderer);
	ptabSprite[8]=textureLoader("./img/6.bmp",surfaceLoader,pRenderer);
	SDL_Rect dest = { 0,0, X_WINDOW,Y_WINDOW};
	SDL_RenderCopy(pRenderer,ptabSprite[0],NULL,NULL); // Copie du sprite
	drawAll(gameState,ptabSprite,pRenderer);
	SDL_RenderPresent(pRenderer); // Affichage
	SDL_Delay(3000);
	for(i=0;i<9;i++)
		SDL_DestroyTexture(ptabSprite[i]); // Libération de la mémoire
	SDL_DestroyRenderer(pRenderer); // Libération de la mémoire

    SDL_Quit();

    return 0;
}
