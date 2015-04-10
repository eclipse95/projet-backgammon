-- COMMENT UTILISER L'INTERFACE --

-----------
Infos de base
-----------

La GUI se base sur un struct GUI_Block

Selon interface.h :

 typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* tabSprite[9];
} GUI_Block;

A moins de faire une utilisation très spécifique, n'utilisez que guiInit, guiQuit et drawAll

A peut être enlever dans les includes : backgammon.h (déjà include dans l'arbitre), assert.h (je ne m'en sers plus, mais au cas où vous voulez test)

Pour les constants avec #define, ne modifiez que celles qui n'ont pas de //* sauf si vous voulez vraiment vous "amusez" à changer le code en dur

Pour la compilation, utilisez tout à la fin de la ligne -lSDL2main -lSDL2 -lSDL2_ttf ***DANS L'ORDRE***
cf. mon Makefile pour l'exemple (INT-Makefile)

-----------
Demo de code
-----------

GUI_Block gui;
=> Crée un struct GUI_Block, pas besoin (et même ne le faites pas) d'initialiser à la main

guiInit(&gui);
=> Initialise tout ce qu'il y a à initialiser pour la SDL tout en modifiant le GUI_Block passé en paramètre

drawAll(gameState,gui.tabSprite,gui.renderer,dice);
=> Clean l'écran et appelle toutes les fonctions de dessin d'un coup avec des paramètres optimisés. Pas besoin de l'appeler plusieurs fois si le gameState et le dice sont identiques.
(gameState est un SGameState, et dice est un unsigned char[2]. Non initialisé par guiInit, donc à importer)

guiQuit(&gui);
=> Execute toutes les fonctions de libération mémoire de la SDL et ferme la fenêtre



Pour une démo de code plus parlante, voire la fonction main mise en commentaire. (Pour des tests sur interface.c seul, dé-commentez aussi InitBoard tout en haut, sinon supprimez le)
