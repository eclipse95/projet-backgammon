#include <SDL2/SDL.h>

 typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* tabSprite[9];
} GUI_Block;

SMove* getAllPossibleMoves(SGameState *gameState, int player, unsigned char dice[2], SMove* moves);
int getXSquare(int id);
int getYSquare(int id);
void drawSquare(SGameState *gameState, int id, SDL_Texture* tabSprite[], SDL_Renderer* renderer);
void drawMultiplier(SGameState *gameState,SDL_Texture* tabSprite[], SDL_Renderer* renderer);
void drawDice(SGameState *gameState,unsigned char dice[2],SDL_Texture* tabSprite[], SDL_Renderer* renderer);
void drawAll(SGameState *gameState, SDL_Texture* tabSprite[], SDL_Renderer* renderer);
SDL_Texture* textureLoader(char* filename,SDL_Surface* surfaceLoader,SDL_Renderer* renderer);
void guiInit(GUI_Block* gui);
void guiQuit(GUI_Block* gui);
