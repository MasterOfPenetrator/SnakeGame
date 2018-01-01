#include "CSFMLMainAPI.h"

// Init the CSFML API
bool CSFMLInitAPI(void)
{
    // Init Graphics
    sfVideoMode mode = {600, 700, 32};
    MainAPI.AI_Screen = sfRenderWindow_create(mode, "Snake Game", sfClose, NULL);

    if(MainAPI.AI_Screen == NULL)
    {
        printf("Main Subsystem Fehler 'API': Kann Screen Context nicht erstellen!\n");
        return false;
    }

    // Init FPS Clock
    MainAPI.AI_FPS.Clock = sfClock_create();

    if(MainAPI.AI_FPS.Clock == NULL)
    {
        printf("Main Subsystem Fehler 'API' Kann FPS Clock nicht initialisieren!\n");
        return false;
    }

    // General Stuff
    MainAPI.AI_Is_Init = true;
    MainAPI.AI_FPS.Value = 0.0f;
    MainAPI.AI_FPS.Actual.microseconds = 0.0f;
    MainAPI.AI_FPS.Last.microseconds = 0.0f;

    return true;
}

// Quit the API
void CSFMLQuitAPI(void)
{
    // Clear Stuff
    if(MainAPI.AI_Is_Init)
    {
        sfRenderWindow_destroy(MainAPI.AI_Screen);
        MainAPI.AI_Screen = NULL;
        sfClock_destroy(MainAPI.AI_FPS.Clock);
        MainAPI.AI_FPS.Clock = NULL;
    }

    // Clear general stuff
    MainAPI.AI_FPS.Value = 0.0f;
    MainAPI.AI_FPS.Actual.microseconds = 0.0f;
    MainAPI.AI_FPS.Last.microseconds = 0.0f;
    MainAPI.AI_Is_Init = false;
}

// Update the API
bool CSFMLUpdateAPI(void)
{
    // Update FPS
    MainAPI.AI_FPS.Actual = sfClock_getElapsedTime(clck);
    FPS = 1000000.0f / (MainAPI.AI_FPS.Actual.microseconds - MainAPI.AI_FPS.Last.microseconds);
    MainAPI.AI_FPS.Last = MainAPI.AI_FPS.Actual;
}
