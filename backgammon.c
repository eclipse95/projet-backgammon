#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "arbitre.h"
#include "backgammon.h"

srand((unsigned) time(&t)); //permet de créer une nouvelle seed radom (donc de "relancer" le tableau à chaque compilation)


// Dans la librairie /////////////////////////////////////////////////////////////////////////////// ///////////////////////////////////////////////////////////////////////////////

void InitLibrary(char name[50])
{
	printf("InitLibrary\n");
	strcpy(name,"Fabien Picarougne");
}

void StartMatch(const unsigned int target_score)
{
	printf("StartMatch\n");
}

void StartGame(Player p)
{
	printf("StartGame\n");
}

void EndGame()
{
	printf("EndGame\n");
}

void EndMatch()
{
	printf("EndMatch\n");
}

int DoubleStack(const SGameState * const gameState) //demande videau // pb : stocker qui l'a demandé pour pas lui redemander
{
	printf("DoubleStack\n");
	return(0);
}

int TakeDouble(const SGameState * const gameState) //réponse
{
	printf("TakeDouble\n");
	return(0);
}

void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries)
{
	printf("PlayTurn\n");
}


// Dans l'exécutable  /////////////////////////////////////////////////////////////////////////////// ///////////////////////////////////////////////////////////////////////////////

//void deroulement_du_jeu()  --->>> mis par le prof	
int main()
{
	void* libj1, libj2;
	// à chaque utilisation de gameState, ne pas oublier de faire une copie de tous les éléments (pas fait ici) ---->>> mis par le prof
	SGameState gameState=InitState();
	SMove moves[4];

	//// Chargement des bibliothèques et de leurs fonctions
	//Joueur 1
	if ((libj1=dlopen("libj1.so",RTLD_LAZY))==NULL) { return(-1); }
	pfInitLibrary j1InitLibrary=InitLibrary;
	if ((j1InitLibrary=(pfInitLibrary)dlsym(libj1,"InitLibrary"))==NULL) { return(-1); }
	pfStartMatch j1StartMatch=StartMatch;
	if ((j1StartMatch=(pfStartMatch)dlsym(libj1,"StartMatch"))==NULL) { return(-1); }
	pfStartGame j1StartGame=StartGame;
	if ((j1StartGame=(pfStartGame)dlsym(libj1,"StartGame"))==NULL) { return(-1); }
	pfEndGame j1EndGame=EndGame;
	if ((j1EndGame=(pfEndGame)dlsym(libj1,"EndGame"))==NULL) { return(-1); }
	pfEndMatch j1EndMatch=EndMatch;
	if ((j1EndMatch=(pfEndMatch)dlsym(libj1,"EndMatch"))==NULL) { return(-1); }
	pfDoubleStack j1DoubleStack=DoubleStack;
	if ((j1DoubleStack=(pfDoubleStack)dlsym(libj1,"DoubleStack"))==NULL) { return(-1); }
	pfTakeDouble j1TakeDouble=TakeDouble;
	if ((j1TakeDouble=(pfTakeDouble)dlsym(libj1,"TakeDouble"))==NULL) { return(-1); }
	pfPlayTurn j1PlayTurn=PlayTurn;
	if ((j1PlayTurn=(pfPlayTurn)dlsym(libj1,"PlayTurn"))==NULL) { return(-1); }
	
	//Joueur 2
	if ((libj2=dlopen("libj2.so",RTLD_LAZY))==NULL) { return(-1); }
	pfInitLibrary j2InitLibrary=InitLibrary;
	if ((j2InitLibrary=(pfInitLibrary)dlsym(libj2,"InitLibrary"))==NULL) { return(-1); }
	pfStartMatch j2StartMatch=StartMatch;
	if ((j2StartMatch=(pfStartMatch)dlsym(libj2,"StartMatch"))==NULL) { return(-1); }
	pfStartGame j2StartGame=StartGame;
	if ((j2StartGame=(pfStartGame)dlsym(libj2,"StartGame"))==NULL) { return(-1); }
	pfEndGame j2EndGame=EndGame;
	if ((j2EndGame=(pfEndGame)dlsym(libj2,"EndGame"))==NULL) { return(-1); }
	pfEndMatch j2EndMatch=EndMatch;
	if ((j2EndMatch=(pfEndMatch)dlsym(libj2,"EndMatch"))==NULL) { return(-1); }
	pfDoubleStack j2DoubleStack=DoubleStack;
	if ((j2DoubleStack=(pfDoubleStack)dlsym(libj2,"DoubleStack"))==NULL) { return(-1); }
	pfTakeDouble j2TakeDouble=TakeDouble;
	if ((j2TakeDouble=(pfTakeDouble)dlsym(libj2,"TakeDouble"))==NULL) { return(-1); }
	pfPlayTurn j2PlayTurn=PlayTurn;
	if ((j2PlayTurn=(pfPlayTurn)dlsym(libj2,"PlayTurn"))==NULL) { return(-1); }

	//// Initialisation de la bibliothèque de chaque joueur du match
	char name1[50];
	char name2[50];
	j1InitLibrary(name1);
	j2InitLibrary(name2);
	j1StartMatch(5);
	j2StartMatch(5);

	unsigned int *roll=malloc(sizeof(unsigned int)*2);
	unsigned int nbMoves;
	unsigned char dices[2];
	int player;

	while (IsMatchOver(&gameState)!=-1)
	{
		j1StartGame(BLACK);
		j2StartGame(WHITE);
		player=NOBODY;
		while (player==NOBODY)
		{
			dices=rollDice(roll);
			if(dices[BLACK] > dices[WHITE]) { player=BLACK; }else{ player=WHITE; } 
		}
		while (IsGameOver(gameState)!=-1)
		{
		InitBoard(gameState);
		if (player==BLACK)
		{
		  	//if le gars a déja lancé un dble stack qui a été accepté, on rappelle pas la fonction
			if (j1DoubleStack(&gameState)!=0)
			{
				if (j2TakeDouble(&gameState)!=0)
				{
					gameState->stake*=2;
				}
				else
				{
					//modifier IsGameOver??
					gameState->bar[BLACK]=15;
				}
			}
			if (IsGameOver(gameState)==-1)
			{
				j1PlayTurn(&gameState,dices,moves,&nbMoves,3);
				//verification pas triche : peut passer que si peut pas jouer, et verif cases envoyées apatiennent à cases possibles
				//faire le mouvement
				emptyMoves(moves);
				player=WHITE;
			}
			j1PlayTurn(&gameState,dices,moves,&nbMoves,3);
			//verification pas triche : peut passer que si peut pas jouer, et verif cases envoyées apatiennent à cases possibles
			//faire le mouvement
			emptyMoves(moves);
			player=WHITE;
		}
		else if (player==WHITE)
		{
		//if le gars a déja lancé un dble stack qui a été accepté, on rappelle pas la fonction
			if (j2DoubleStack(&gameState)!=0)
			{
				if (j1TakeDouble(&gameState)!=0)
				{
					gameState->stake*=2;
				}
				else
				{
					//modifier IsGameOver??
					gameState->bar[WHITE]=15;
				}
			}
			if (IsGameOver(gameState)==-1)
			{
				j2PlayTurn(&gameState,dices,moves,&nbMoves,3);
				//verification pas triche : peut passer que si peut pas jouer, et verif cases envoyées apatiennent à cases possibles
				//faire le mouvement
				emptyMoves(moves);
				player=BLACK;
			}
		  }
		}
		if (IsGameOver(gameState)==BLACK) 
		{ 
			gameState->blackScore+=gameState->stake;
			printf("Les Noirs gagnent cette manche et %i points !", gameState->stake);
		}
		else if (IsGameOver(gameState)==WHITE) 
		{ 
			gameState->whiteScore+=gameState->stake; 
			printf("Les Blancs gagnent cette manche et %i points !\n", gameState->stake);
		}
		else { return -1; } //gestion d'erreur plus précise
		j1EndGame();
		j2EndGame();
	}
	if (IsMatchOver(gameState)==BLACK) { printf("Les Noirs gagnent la partie !"); }
	else if (IsMatchOver(gameState)==WHITE) { printf("Les Blancs gagnent la partie !"); }
	else { return -1; }
	j1EndMatch();
	j2EndMatch();
	
	dlclose(libj1); //libération des bibliothèques
	dlclose(libj2);
	free(roll);
	free(gameState);
	return(0);
}
