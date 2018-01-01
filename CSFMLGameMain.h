#ifndef CSFMLGameMain_H

#define CSFMLGameMain_H

// Includes
#include "CSFMLMain.h"
#include "CSFMLOtherStuff.h"
#include "CSFMLGameTilemap.h"
#include "CSFMLGameLights.h"
#include "CSFMLGameClock.h"
#include "CSFMLGameSnake.h"
#include "CSFMLGameItems.h"
#include "CSFMLGameEnemies.h"
#include "CSFMLGameHighScore.h"

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

