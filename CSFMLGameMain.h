#ifndef CSFMLGameMain_H

#define CSFMLGameMain_H

// Enums
typedef enum { IT_HEALTH, IT_SPEED, IT_FOOD, IT_GOD, IT_NOCLIP, IT_CUSTOM } ItemType;

// Includes
#include "CSFMLMenu.h"
#include "CSFMLOtherStuff.h"
#include "CSFMLGameTilemap.h"
#include "CSFMLGameLights.h"
#include "CSFMLGameClock.h"
#include "CSFMLGameSnake.h"
#include "CSFMLGameItems.h"
#include "CSFMLGameWeapons.h"
#include "CSFMLGameEnemies.h"

// Structs
typedef struct {
    sfText *GM_Health;
    sfText *GM_Score;
    sfFont *GM_Font;
    bool Game_Is_Init;
    int Game_Level_ID;
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
void CSFMLMainRenderOther();

#endif /* CSFMLGameMain_H */

