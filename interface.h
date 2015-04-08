SMove* getAllPossibleMoves(SGameState *gameState, int player, unsigned char dice[2]);
void drawSquare(SGameState *gameState, int id, SDL_Texture* tabSprite[], SDL_Renderer* renderer);
int getXSquare(int id);
int getYSquare(int id);
void drawMultiplier(SGameState *gameState,SDL_Texture* tabSprite[], SDL_Renderer* renderer);
void drawDice(SGameState *gameState,unsigned char dice[2],SDL_Texture* tabSprite[], SDL_Renderer* renderer);
void drawAll(SGameState *gameState, SDL_Texture* tabSprite[], SDL_Renderer* renderer);
SDL_Texture* textureLoader(char* filename,SDL_Surface* surfaceLoader,SDL_Renderer* renderer);
