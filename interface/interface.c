#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "backgammon.h"
//#include "interface.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <assert.h>

#define TAILLE_PION 40 //*
#define W_DICE 39 //*
#define H_DICE 41 //*
#define X_WINDOW 800 //*
#define Y_WINDOW 600 //*
#define FONT "Xenotron.ttf"
#define FONT_SIZE 24
#define PION_STACK_DECAL 10

/* A DECOMMENTER POUR TEST SUR INTERFACE.C SEUL */
/*
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
*/

/* FONCTION A PEUT ETRE UTILISER POUR L'INPUT MANUEL SI ON A LE TEMPS, A ENLEVER SINON */

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
// 0 -> 23 : board
// 24, 25 : out black, out white
// 26, 27 : bar black, bar white
int getXSquare(int id)
{
	if(id==26 || id==27)
	{
		return 752;
	}
	if(id==25)
	{
		return 392;
	}
	if(id==24)
	{
		return 321;
	}
	if(id>=18 && id<=23)
	{
		return (392 + 60*(id-18));
	}
	if(id>=12 && id<=17)
	{
		return (18 + 60*(id-12));
	}
	if(id>=6 && id<=11)
	{
		return (321 - 60*(id-6));
	}
	if(id>=0 && id<=5)
	{
		return (695 - 60*id);
	}
	return 0;
}
int getYSquare(int id)
{
	if(id==27)
	{
		return 17;
	}
	if(id==26)
	{
		return 372;
	}
	if(id==24 || id==25)  
	{
		return 275;
	}
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
	// multiplier sert juste à entasser les pions vers le haut ou vers le bas pour l'affichage
	if(id>=0 && id<=11){multiplier=-1;}
	if(id>=12 && id<=23){multiplier=1;}
	SDL_Texture* texture=NULL;
	Square square=gameState->board[id];
	if(x==0 || y==0){printf("coordonnées introuvables");return;}
	switch(square.owner) // Charge la texture associé à la couleur du pion
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
		SDL_Rect dest={x,y+i*TAILLE_PION*multiplier,TAILLE_PION,TAILLE_PION};	
		SDL_RenderCopy(renderer,texture,NULL,&dest);
	}
}

// dessine les pions hors du jeu
void drawOut(SGameState *gameState,SDL_Texture* tabSprite[], SDL_Renderer* renderer)
{
	int x,y;
	char output[2];
	// out black
	x=getXSquare(24);y=getYSquare(24);
	SDL_Rect dest={x,y,TAILLE_PION,TAILLE_PION};
	SDL_RenderCopy(renderer,tabSprite[1],NULL,&dest);
	sprintf(output,"%i",gameState->out[0]);
	drawText(x,y+10,output,renderer);
	// out white
	x=getXSquare(25);y=getYSquare(25);
	SDL_Rect dest2={x,y,TAILLE_PION,TAILLE_PION};
	SDL_RenderCopy(renderer,tabSprite[2],NULL,&dest2);
	sprintf(output,"%i",gameState->out[1]);
	drawText(x,y+10,output,renderer);
}

// dessine les pions gagnés
void drawBar(SGameState *gameState,SDL_Texture* tabSprite[], SDL_Renderer* renderer)
{
	int x,y,i,j,jmax;
	SDL_Rect dest;
	dest.w=TAILLE_PION;
	dest.h=TAILLE_PION;
	for(i=0;i<2;i++)
	{
		x=getXSquare(26+i);y=getYSquare(26+i);
		jmax=gameState->bar[i];
		for(j=0;j<jmax;j++)
		{
			dest.x=x;
			dest.y=y;
			SDL_RenderCopy(renderer,tabSprite[i+1],NULL,&dest);
			y=y+PION_STACK_DECAL;
		}
	}
}

// dessine le texte text à l'endroit x;y
void drawText(int x,int y, char* text,SDL_Renderer* renderer)
{
	TTF_Font* font = TTF_OpenFont(FONT, FONT_SIZE);
	SDL_Color color = {255, 0, 0}; // texte en rouge
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	int w,h;
	SDL_QueryTexture(Message,NULL,NULL,&w,&h);
	SDL_Rect dest = {x+w/2,y,w,h}; // centrage (pas optimal mais bon)
	SDL_RenderCopy(renderer,Message,NULL,&dest);
	// libérage immédiat de la mémoire (pas nécessaire de conserver les images générées)
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(surfaceMessage);
}

// dessine le videau (je l'ai appelé Multiplier sans savoir le mot au début)
void drawMultiplier(SGameState *gameState,SDL_Texture* tabSprite[], SDL_Renderer* renderer)
{
	char output[2];
	sprintf(output,"%i",gameState->stake);
	drawText(767,286,output,renderer);
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
void drawAll(SGameState *gameState, SDL_Texture* tabSprite[], SDL_Renderer* renderer,unsigned char dice[2])
{
	printf("drawall\n");
	SDL_RenderClear(renderer); // On clean l'écran pour tout redessiner
	SDL_RenderCopy(renderer,tabSprite[0],NULL,NULL); // Fond
	int i;
	for(i=0;i<24;i++) // Dessine toutes les cases du Board
	{
		printf("tour boucle drawsquare %i\n",i);
		drawSquare(gameState,i,tabSprite,renderer);
	}
	drawMultiplier(gameState,tabSprite,renderer); 
	drawDice(gameState,dice,tabSprite,renderer);
	drawOut(gameState,tabSprite,renderer);
	drawBar(gameState,tabSprite,renderer);
	SDL_RenderPresent(renderer); // On met à jour l'écran une fois que tout est collé
}

// fonction custom pour automatiser le process de création de textures
SDL_Texture* textureLoader(char* filename,SDL_Surface* surfaceLoader, SDL_Renderer* renderer)
{
	printf("load texture %s\n",filename);
	SDL_Texture* returnValue;
	surfaceLoader=SDL_LoadBMP(filename);
	SDL_SetColorKey(surfaceLoader,SDL_TRUE,SDL_MapRGB(surfaceLoader->format,0,0,255)); // Transparence à la place de la couleur Bleue pour les pions
	returnValue=SDL_CreateTextureFromSurface(renderer,surfaceLoader);
	SDL_FreeSurface(surfaceLoader);
	SDL_SetTextureBlendMode(returnValue, SDL_BLENDMODE_BLEND); // Ligne normalement inutile mais j'ai peur de l'enlever
	return returnValue;
}

// à lancer pour initialiser la fenêtre
void guiInit(GUI_Block* gui)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }
    TTF_Init(); // Chargement de la lib pour les polices d'écriture
	/* Création de la fenêtre */
	gui->window = SDL_CreateWindow("Backgammon",SDL_WINDOWPOS_UNDEFINED,
															  SDL_WINDOWPOS_UNDEFINED,
															  X_WINDOW,
															  Y_WINDOW,
															  SDL_WINDOW_SHOWN);

	gui->renderer = SDL_CreateRenderer(gui->window,-1,SDL_RENDERER_ACCELERATED); // Init renderer
	
	// Médiateur de génération initiale des textures à utiliser, n'est jamais utilisé en tant que tel
	SDL_Surface* surfaceLoader;
	// pré-chargement de toutes les images de l'interface en texture
	gui->tabSprite[0]=textureLoader("./img/backgammon.bmp",surfaceLoader,gui->renderer);
	gui->tabSprite[1]=textureLoader("./img/black.bmp",surfaceLoader,gui->renderer);
	gui->tabSprite[2]=textureLoader("./img/white.bmp",surfaceLoader,gui->renderer);
	gui->tabSprite[3]=textureLoader("./img/1.bmp",surfaceLoader,gui->renderer);
	gui->tabSprite[4]=textureLoader("./img/2.bmp",surfaceLoader,gui->renderer);
	gui->tabSprite[5]=textureLoader("./img/3.bmp",surfaceLoader,gui->renderer);
	gui->tabSprite[6]=textureLoader("./img/4.bmp",surfaceLoader,gui->renderer);
	gui->tabSprite[7]=textureLoader("./img/5.bmp",surfaceLoader,gui->renderer);
	gui->tabSprite[8]=textureLoader("./img/6.bmp",surfaceLoader,gui->renderer);
}

// à lancer pour fermer la fenêtre et libérer la mémoire
void guiQuit(GUI_Block* gui)
{
	int i;
	for(i=0;i<9;i++)
		SDL_DestroyTexture(gui->tabSprite[i]); // Destruction de toutes les textures chargées au début
	SDL_DestroyRenderer(gui->renderer); // Destruction du rendu
	SDL_DestroyWindow(gui->window); // Détruit la fenêtre (et donc la ferme, naturellement)
	TTF_Quit();
	SDL_Quit();
}

/*
int main(int argc, char** argv)
{
	// Génération d'un gameState fictif
	SGameState* gameState=malloc(sizeof(SGameState));
	InitBoard(gameState);
	gameState->bar[0]=3;
	gameState->bar[1]=5;
	gameState->out[0]=2;
	unsigned char dice[2]; // à enlever plus tard
	dice[0]=3;
	dice[1]=2;
	
	// Partie SDL
	GUI_Block gui;
	guiInit(&gui); // On initialise SDL et la fenêtre
	drawAll(gameState,gui.tabSprite,gui.renderer,dice);// Affichage état du jeu 1
	SDL_Delay(3000); // Attendre 3s
	
	// La partie avance
	dice[0]=5;
	dice[1]=1;
	gameState->bar[1]=8;
	gameState->out[1]=1;
	drawAll(gameState,gui.tabSprite,gui.renderer,dice);// Affichage état du jeu 2
	SDL_Delay(3000); // Attendre 3s
	
	// On termine
	guiQuit(&gui); // On ferme tout
    return 0;
}
*/
