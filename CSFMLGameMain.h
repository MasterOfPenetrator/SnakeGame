#ifndef CSFMLGameMain_H

#define CSFMLGameMain_H

// Enums
typedef enum { IT_HEALTH, IT_SPEED, IT_FOOD, IT_GOD, IT_NOCLIP, IT_CUSTOM } ItemType;
typedef enum { LEFT, RIGHT, UP, DOWN, NONE } Direction;

// Includes
#include "CSFMLMenu.h"
#include "CSFMLOtherStuff.h"
#include "CSFMLGameTilemap.h"
#include "CSFMLGameLights.h"
#include "CSFMLGameClock.h"
#include "CSFMLGameSnake.h"
#include "CSFMLGameItems.h"
#include "CSFMLGameEnemies.h"

// Structs
typedef struct {
    // SFML Stuff
    sfText *GM_Health;
    sfText *GM_Score;
    sfText *GM_Pause;
    sfText *GM_Pause_Autokill;

    // General Stuff
    bool GM_Is_Init;
    bool GM_Paused;
    int GM_Level_ID;

    // sfView Stuff
    float GM_View_SumMovement;
    bool GM_View_Movement_ResetFlag;
    bool GM_View_Movement_RoundFlag;
} Game;

// Variables
Game GameMain;

// Funktionen
bool CSFMLGameInit();
void CSFMLGameUpdate();
void CSFMLGameQuit();
void CSFMLGameShowScore();
void CSFMLGameShowHealth();
void CSFMLGamePause();
void CSFMLGamePauseRenderText();
void CSFMLMainMoveView();
void CSFMLMainRenderOther();

#endif /* CSFMLGameMain_H */

