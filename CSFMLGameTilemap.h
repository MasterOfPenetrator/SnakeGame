#ifndef CSFMLGameTilemap_H

#define CSFMLGameTilemap_H

// Includes
#include "CSFMLGameMain.h"

// Defines
#define MAX_TILES_X 20
#define MAX_TILES_Y 20

// Structs
typedef struct {
    sfVector2f Position;
    int Action;
} Event;

typedef struct {
    ItemType Type;
    size_t Count;
} MapAllowedItems;

typedef struct {
    int Type;
    int Damage;
} MapDMGTypes;

typedef struct {
    float Amount;
    ItemType Effect;
    int Duration; // How often it will appear in 1 Minute
} KillEffect;

typedef struct {
    const char MapName[20];
    int Score_per_Second;
    float Start_Speed;
    float Default_Speed;
    size_t Snake_Start_Blocks;
    size_t Snake_Grow_Amount;
    float Ambient_Power;
    KillEffect Autokill[3];
    size_t Autokill_Count;
    MapDMGTypes DMG[3];
    size_t DMG_Count;
    MapAllowedItems ItemsAllowed[6];
} MapDescriptor;

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

    // Views
    sfView *BG_View;
    sfView *BG_Minimap;

    // Music
    sfMusic *BG_Music;
    bool BG_Music_Is_Init;

    // Other Related Stuff
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

    // Map Descriptor
    MapDescriptor MD_Details;
    bool MD_Is_Init;

} GameLevel;

GameLevel Level;

// Funktionen
bool CSFMLLoadlevel(int id);
bool CSFMLRenderLevel();
void CSFMLQuitLevel();
bool CSFMLLoadMusic();
bool CSFMLLoadMapDescriptor();
bool CSFMLPreloadMapName(const char *Path, char *LevelName, size_t LevelName_Size); // For Menu Level Names
bool CSFMLLoadEventmap();
bool CSFMLLoadTilemap();
bool CSFMLLoadBackground();

#endif /* CSFMLGameTilemap_H */
