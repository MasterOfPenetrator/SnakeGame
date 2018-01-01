#ifndef CSFMLMainAPI_H

#define CSFMLMainAPI_H

// Includes
#include "CSFMLMain.h"

// Structs
typedef struct {
    sfClock *Clock;
    float Value;
    sfTime Actual;
    sfTime Last;
} API_FPS;

typedef struct {
    sfRenderWindow *AI_Screen;
    API_FPS AI_FPS;

    bool AI_Is_Init;
} API;

// Variables
API MainAPI;

// Functions
bool CSFMLInitAPI();
void CSFMLQuitAPI();
bool CSFMLUpdateAPI();


#endif /* CSFMLMainAPI_H */
