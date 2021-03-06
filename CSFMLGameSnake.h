#ifndef CSFMLGameSnake_H

#define CSFMLGameSnake_H

// Includes
#include "CSFMLGameMain.h"

// Defines
#define SNAKE_SELFHIT_DMG 45
#define SNAKE_BITE_DMG 10

// Structs
typedef struct {

    // Blocks
    Block SB_Head;
    Block *SB_Body;

    // CSFML Stuff
    sfTexture *SB_Head_Texture;
    sfShader *SB_Head_Shader;
    sfSprite *SB_Head_Sprite;
    sfRenderStates SB_Head_State;

    sfTexture *SB_Body_Texture;
    sfShader *SB_Body_Shader;
    sfSprite *SB_Body_Sprite;
    sfRenderStates SB_Body_State;

    // Properties
    size_t SB_Body_Elements;
    char S_Name[21];

    int S_Health;
    int S_Rotate;
    int S_Score;

    float S_Speed;
    float S_DefaultSpeed;
    float S_LightDistance;
    bool S_Light_FBM;
    bool S_Light_OnOff;
    bool S_Autokill;
    float S_Autokill_Speed_Amount;

    bool S_Is_Init;
    bool S_Is_Dead;

    bool S_GODMODE;
    bool S_NOCLIP;

    Direction S_Actual_Direction;
    Direction S_Prev_Direction;

    // TODO Snake Weapons
    int S_Actual_Bite_DMG;


} Snake;

// Variables
Snake GameSnake;

// Functions
bool CSFMLInitSnake();
bool CSFMLRenderSnake();
void CSFMLQuitSnake();
sfVector2f CSFMLSnakeConvertIndexToVector(Block blk);
bool CSFMLPushSnakeBlock(Direction dir);
bool CSFMLPopSnakeBlock();
bool CSFMLSetSnakeLight();
bool CSFMLGrowSnake();
bool CSFMLCheckTileMapCollision();
bool CSFMLCheckSelfCollision();
bool CSFMLHandleSnake();

#endif /* CSFMLGameSnake_H */

