#ifndef CSFMLGameLights_H

#define CSFMLGameLights_H

// Includes
#include "CSFMLGameMain.h"

// Defines
#define MAX_LIGHTS 10

// Light variables
sfGlslVec3 *GameLight_Lightpos;
sfGlslVec3 *GameLight_Conepos;
float *GameLight_Coneactive;
float *GameLight_FBMactive;
float *GameLight_Ambientpower;
float *GameLight_Coneangle;
int *GameLight_ID;
float *GameLight_Active;
size_t GameLight_Count;
bool GameLight_Is_Init;
bool GameLight_Viewport_Setted;

// Functions
bool CSFMLInitLights();
bool CSFMLRenderLights();
void CSFMLQuitLights();
bool CSFMLSetViewportLight();
bool CSFMLSetLight(int light_x, int light_y, int cone_x, int cone_y, float ambientpower, bool coneactive, bool fbmactive, float coneangle, int id, bool active);

#endif /* CSFMLGameLights_H */
