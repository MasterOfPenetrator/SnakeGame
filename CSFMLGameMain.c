#include "CSFMLGameMain.h"

// Init Game
bool CSFMLGameInit()
{
    // Init General Things
    bool Init_Error = false;

    // Init TileMap
    Init_Error |= !CSFMLLoadlevel(1);

    // Init Lights
    Init_Error |= !CSFMLInitLights();
    Init_Error |= !CSFMLSetViewportLight();

    // Init Clock
    Init_Error |= !CSFMLInitClock();

    // Init Snake
    Init_Error |= !CSFMLInitSnake();

    // Init Game Items
    Init_Error |= !CSFMLInitItems();

    // Init Text for Score and Health
    GameMain.GM_Font = sfFont_createFromFile("Bilder/Schriftarten/3Dventure.ttf");
    GameMain.GM_Health = sfText_create();
    GameMain.GM_Score = sfText_create();

    if(GameMain.GM_Font == NULL || GameMain.GM_Health == NULL || GameMain.GM_Score == NULL)
    {
        Init_Error |= true;
    }

    return Init_Error;
}

// Clear Game
void CSFMLGameQuit()
{
    // Clear Game Main Elements
    sfFont_destroy(GameMain.GM_Font);
    sfText_destroy(GameMain.GM_Health);
    sfText_destroy(GameMain.GM_Score);
    GameMain.GM_Font = NULL;
    GameMain.GM_Health = NULL;
    GameMain.GM_Score = NULL;

    // Clear Elements
    CSFMLQuitItems();
    CSFMLQuitSnake();
    CSFMLQuitClock();
    CSFMLQuitLights();
    CSFMLQuitLevel();
}

// Render Game
void CSFMLGameUpdate()
{
    // Is Snake Alive
    if(!GameSnake.S_Is_Dead)
    {
        // Activate seperate Game Clock
        CSFMLUpdateClock();

        // Activate Time Score Update
        CSFMLGameUpdateTimeSnakeScore();

        // Activate Lights
        CSFMLRenderLights();

        // Render Level
        CSFMLRenderLevel();

        // Handle Snake Events
        CSFMLHandleSnake();

        // Place Score and Health String
        CSFMLGameShowScore();
        CSFMLGameShowHealth();
    }

    // Place Background Pictures
    MenuPlaceIMG(0, 0, false, 0);
    MenuPlaceIMG(0, 200, true, 0);

}

// Show game score
void CSFMLGameShowScore()
{
    sfVector2f position = {200, 163};
    sfColor color = {233, 77, 0, 255};

    sfText_setPosition(GameMain.GM_Score, position);
    sfText_setFont(GameMain.GM_Score, GameMain.GM_Font);
    sfText_setCharacterSize(GameMain.GM_Score, 20);
    sfText_setColor(GameMain.GM_Score, color);

    char score_string[50] = {0};
    char start[8] = "Score: ";
    char end[8] = " Points";
    char value[12];
    strcat(score_string, start);
    itoa(GameSnake.S_Score, value, 10);
    strcat(score_string, value);
    strcat(score_string, end);

    sfText_setString(GameMain.GM_Score, score_string);

    sfRenderWindow_drawText(screen, GameMain.GM_Score, NULL);
}

// Show game health
void CSFMLGameShowHealth()
{
    sfVector2f position = {440, 163};
    sfColor color = {233, 77, 0, 255};

    sfText_setPosition(GameMain.GM_Health, position);
    sfText_setFont(GameMain.GM_Health, GameMain.GM_Font);
    sfText_setCharacterSize(GameMain.GM_Health, 20);
    sfText_setColor(GameMain.GM_Health, color);

    char health_string[50] = {0};
    char start[9] = "Health: ";
    char end[3] = " %";
    char value[6];
    strcat(health_string, start);
    itoa(GameSnake.S_Health, value, 10);
    strcat(health_string, value);
    strcat(health_string, end);

    sfText_setString(GameMain.GM_Health, health_string);

    sfRenderWindow_drawText(screen, GameMain.GM_Health, NULL);
}

// Update Score for Seconds
void CSFMLGameUpdateTimeSnakeScore()
{
    if(GameClock.GC_SecondTick && GameSnake.S_Is_Init && !GameSnake.S_Is_Dead)
        GameSnake.S_Score += Level.MD_Details.Score_per_Second;
}
