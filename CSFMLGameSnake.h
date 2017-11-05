#ifndef CSFMLGameSnake_H

#define CSFMLGameSnake_H

// Includes
#include "CSFMLGameMain.h"

// Defines
#define SNAKE_START_BLOCKS 5
#define SNAKE_PICTURE_SIZE 25
#define SNAKE_SELFHIT_DMG 45

// Enums
typedef enum { LEFT, RIGHT, UP, DOWN, NONE } Direction;

// Structs
typedef struct {
    float x;
    float y;
    float w;
    float h;
} Block;

typedef struct __attribute__((__packed__)) {
    Block SB_Head;
    Block *SB_Body;

    sfTexture *SB_Head_Texture;
    sfShader *SB_Head_Shader;
    sfRenderStates SB_Head_State;
    sfTexture *SB_Body_Texture;
    sfShader *SB_Body_Shader;
    sfRenderStates SB_Body_State;

    size_t SB_Body_Elements;
    char S_Name[20];

    int S_Health;
    int S_Rotate;
    int S_Score;

    float S_Speed;
    float S_DefaultSpeed;
    float S_LightDistance;
    bool S_Light_FBM;
    bool S_Light_OnOff;

    Direction S_Actual_Direction;
    Direction S_Prev_Direction;

    bool S_Is_Init;
    bool S_Is_Dead;

    bool S_GODMODE;
    bool S_NOCLIP;

    float S_EndItemTime;

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

