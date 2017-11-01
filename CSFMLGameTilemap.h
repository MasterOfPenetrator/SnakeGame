#ifndef CSFMLGameTilemap_H

#define CSFMLGameTilemap_H

// Includes
#include "CSFMLGameMain.h"

// Defines
#define MAX_TILES_X 20
#define MAX_TILES_Y 20

typedef struct {
    sfVector2f Position;
    int Action;
} Event;

typedef struct {

    // Generel Things
    bool Is_Init;
    bool Is_Loaded;
    int Level_ID;
    char Level_Path[30];

    // Background
    sfTexture *BG_Texture_Diffuse;
    sfTexture *BG_Texture_Normal;
    sfTexture *BG_Texture_Specular;
    sfShader *BG_Texture_Shader;
    sfRenderStates BG_Texture_State;
    sfSprite *BG_Sprite;

    sfView *BG_View;
    sfView *BG_Minimap;

    unsigned int BG_Width;
    unsigned int BG_Height;
    sfVector2f BG_Screenposition;
    bool BG_Is_Init;

    // Tilemap
    int TL_Map[20][20];
    int TL_X_Size;
    int TL_Y_Size;
    sfVector2f TL_Start_Position;
    sfVector2f TL_End_Position;
    bool TL_Is_Init;

    // Eventmap
    Event *EV_Map;
    size_t EV_Count;
    bool EV_Is_Init;

} GameLevel;

GameLevel Level;

// Funktionen
bool CSFMLLoadlevel(int id);
bool CSFMLRenderLevel();
void CSFMLQuitLevel();
bool CSFMLLoadEventmap();
bool CSFMLLoadTilemap();
bool CSFMLLoadBackground();

#endif /* CSFMLGameTilemap_H */
