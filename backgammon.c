#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "arbitre.h"
#include "backgammon.h"

  srand((unsigned) time(&t)); //permet de créer une nouvelle seed radom (donc de "relancer" le tableau à chaque compilation)

//////////////////////////////////////////////////////////
// Dans la librairie
//

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

int DoubleStack(const SGameState * const gameState)
{
	printf("DoubleStack\n");
	return(0);
}

int TakeDouble(const SGameState * const gameState)
{
	printf("TakeDouble\n");
	return(0);
}

void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries)
{
	printf("PlayTurn\n");
}



//////////////////////////////////////////////////////////
// Dans l'exécutable
//

//void deroulement_du_jeu()	
int main()
{
	//*****// à chaque utilisation de gameState, ne pas oublier de faire une copie de tous les éléments (pas fait ici)
	SGameState gameState;
	SMove moves[4];

	//*****// à faire pour chaque joueur
	
	// Chargement de la librairie (chargement des pointeurs de fonctions des fonctions décrites dans "backgammon.h")
	// -> à ermplacer par votre code de chargement
	pfInitLibrary j1InitLibrary=InitLibrary;
	pfStartMatch j1StartMatch=StartMatch;
	pfStartGame j1StartGame=StartGame;
	pfEndGame j1EndGame=EndGame;
	pfEndMatch j1EndMatch=EndMatch;
	pfDoubleStack j1DoubleStack=DoubleStack;
	pfTakeDouble j1TakeDouble=TakeDouble;
	pfPlayTurn j1PlayTurn=PlayTurn;
	
	pfInitLibrary j2InitLibrary=InitLibrary;
	pfStartMatch j2StartMatch=StartMatch;
	pfStartGame j2StartGame=StartGame;
	pfEndGame j2EndGame=EndGame;
	pfEndMatch j2EndMatch=EndMatch;
	pfDoubleStack j2DoubleStack=DoubleStack;
	pfTakeDouble j2TakeDouble=TakeDouble;
	pfPlayTurn j2PlayTurn=PlayTurn;

	// Initialisation de la librairie
	char name[50];
	j1InitLibrary(name);
	j1StartMatch(5);

	unsigned int *diceResult;
	unsigned int nbMoves;
	unsigned char dices[2];
	int player;

	while (IsMatchOver(&gameState)!=1)
	{
		j1StartGame(BLACK);
		j2StartGame(WHITE);
		diceResult=rollDice(2);
		if(diceResult[0] > diceResult[1]) { player=WHITE; }else{ player=BLACK; } //"arithmétique des pointeurs c pas tres lisible en projet" dixit vincent
		while (IsGameOver(gameState)!=1)
		{
		  if (player==BLACK) 
		  {
			if (j1DoubleStack(&gameState))
				j2TakeDouble(&gameState);
			j1PlayTurn(&gameState,dices,moves,&nbMoves,3);
			//verification pas triche : peut passer que si peut pas jouer, et verif cases envoyées apatiennent à cases possibles
			player=2;
		  }
		  else if (player==WHITE)
		  {
			if (j2DoubleStack(&gameState))
				j1TakeDouble(&gameState);
			j2PlayTurn(&gameState,dices,moves,&nbMoves,3);
			//verification pas triche : peut passer que si peut pas jouer, et verif cases envoyées apatiennent à cases possibles
			player=BLACK;
		  }
		}
		j1EndGame();
		j2EndGame();
	}
	j1EndMatch();
	j2EndMatch();

	return(0);
}
