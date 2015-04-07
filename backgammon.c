#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "arbitre.h"
#include "backgammon.h"
#include "interface.h"

//void deroulement_du_jeu()  --->>> mis par le prof
int main()
{
	srand(time((time_t) NULL));    //permet de créer une nouvelle seed random (donc de "relancer" le tableau à chaque compilation)
	void* libj1,* libj2;
	// à chaque utilisation de gameState, ne pas oublier de faire une copie de tous les éléments (pas fait ici)

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

	SGameState* gameState=InitState();
	SMove moves[4];
	unsigned int nbMoves;
	unsigned char dices[2];
	int tailleMoves;
	Player player, startingPlayer;
	unsigned int has_asked_double[2];
	has_asked_double[BLACK]=0;
	has_asked_double[WHITE]=0;
	int tries[2];
	tries[BLACK]=3;
	tries[WHITE]=3;
	//SGameState* copy = (SGameState*) malloc(sizeof(SGameState)); //allocation mémoire de la copie de gameState //copie à utiliser a chaque modif de gamestate pour verif que le gars a pas modif directement dessus ?
	//copyState(gameState, copy);

	while (IsMatchOver(gameState)!=-1)
	{
		j1StartGame(BLACK);
		j2StartGame(WHITE);
		player = NOBODY;
		InitBoard(gameState);
		while (player==NOBODY) //qui commence toujours déterminé par les dés
		{
			rollDice(dices);
			if(dices[BLACK] > dices[WHITE]) { player=BLACK; }else{ player=WHITE; }
		}
		
		startingPlayer = player; //savoir qui a commencé permet de compter les turns
		
		while (IsGameOver(gameState)!=-1)
		{
			if (player == startingPlayer){ gameState->turn++;	}
			
			rollDice(dices); 
			if (dices[0]==dices[1]) { tailleMoves=4; }
			else {tailleMoves=2;}
			
			if(player==BLACK)
			{
			  	if (has_asked_double[player]==0)
			  	{//un joueur dont le DoubleStack a été accepté ne peut pas le redemander ensuite
					if (j1DoubleStack(gameState)!=0)
					{
						if (j2TakeDouble(gameState)!=0) //si accepté
						{
							gameState->stake*=2;
							has_asked_double[player]=1;
						}
						else //si refusé
						{
							gameState->out[player]=15; //fin game // a voir si pb pour l'interface
						}
					}
			  	}

				if (IsGameOver(gameState)==-1)
				{
					j1PlayTurn(gameState,dices,moves,&nbMoves,tries[player]);
					
					if(move(moves, gameState, tailleMoves, player, dices)==-1) //verification triche
					{
						tries[player]--;
					}
					if (tries[player]==0) //victoire adverse si 0 tries
					{
						gameState->out[1-player]=15;
					}
					emptyMoves(moves, tailleMoves);
					player=1-player;
				}
			}
			else if(player == WHITE)
			{
				if (has_asked_double[player]==0)
				{//un joueur dont le DoubleStack a été accepté ne peut pas le redemander ensuite
					if (j2DoubleStack(gameState)!=0)
					{
						if (j1TakeDouble(gameState)!=0) //si accepté
						{
							gameState->stake*=2;
							has_asked_double[player]=1;
						}
						else //si refusé
						{
							gameState->out[player]=15; //fin game //a voir si pb pour l'interface
						}
					}
				}
				
				if (IsGameOver(gameState)==-1)
				{
					j2PlayTurn(gameState,dices,moves,&nbMoves,tries[player]);
					
					if(move(moves, gameState, tailleMoves, player, dices)==-1) //verification triche
					{
						tries[player]--;
					}
					if (tries[player]==0) //victoire adverse si 0 tries
					{
						gameState->out[1-player]=15;
					}
					
					emptyMoves(moves, tailleMoves);
					player=1-player;
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
			else { return -1; } //gestion d'erreur plus précise ?
			
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
	free(gameState);
	//free(copy);
	return(0);
}
