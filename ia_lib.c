#include "ia_lib.h"
#include <stdlib.h>
#include <stdio.h>

void InitLibrary(char name[50])
{
	printf("InitLibrary\n");
	strcpy(name,"Yay");
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
	
	//generate All Moves Possible

    	//get Score

    	//return first
}

// Unofficial Functions
SMove* getAllMovements(const SGameState* const gameState, const unsigned char dices[2]){
	SMove* allMovements;
	
	
	
	return allMovements;
}

int* getScore(SMove* allMovements){
	int* scores;
	
	return scores;
}

int getBest(int* scores){
	int max = -1;
	int id = -1;
	int length = sizeof(scores)/sizeof(int);
	
	
	
	
	return id;
}
