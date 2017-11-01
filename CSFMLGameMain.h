#ifndef CSFMLGameMain_H

#define CSFMLGameMain_H

// Includes
#include "CSFMLMenu.h"
#include "CSFMLGameTilemap.h"
#include "CSFMLGameLights.h"
#include "CSFMLGameClock.h"
#include "CSFMLGameSnake.h"
#include "CSFMLGameItems.h"

// Structs
typedef struct {
    sfText *GM_Health;
    sfText *GM_Score;
    sfFont *GM_Font;
} Game;

// Variables
Game GameMain;

// Funktionen
bool CSFMLGameInit();
void CSFMLGameUpdate();
void CSFMLGameQuit();
void CSFMLGameShowScore();
void CSFMLGameShowHealth();
void CSFMLGameUpdateTimeSnakeScore();

#endif /* CSFMLGameMain_H */

