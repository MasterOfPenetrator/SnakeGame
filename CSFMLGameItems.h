#ifndef CSFMLGameItems_H

#define CSFMLGameItems_H

// Includes
#include "CSFMLGameMain.h"

// Enums
typedef enum { I_HEALTH, I_SCORE, I_SPEED, I_GOD, I_CLIP } ItemEffect;
typedef enum { I_INCREASE, I_DECREASE, I_NOTHING } ItemDirection;


// Structs
typedef struct
{
    float x;
    float y;
    float w;
    float h;
} iBlock;

typedef struct {
    ItemEffect I_Effect;
    ItemDirection I_Direction;
    int I_Amount;
    int I_Duration;
} ItemProperty;

typedef struct {
    char I_Name[50];
    char I_TilePath[50];
    char I_ShaderPath[50];
    int I_Chance;

    size_t I_PropertyCount;
    ItemProperty I_Propertys[3];

} Item;

typedef struct {
    sfTexture **GI_Textures;
    sfShader **GI_Shaders;
    sfRenderStates *GI_States;

    Item *GI_Items;
    size_t GI_Items_Count;

    iBlock *GI_Blocks;
    bool *GI_Placed;
    bool *GI_Coordinates_Setted;

    bool GI_Is_Init;

} Items;

// Variables
Items GameItem;

// Functions
bool CSFMLInitItems();
void CSFMLQuitItems();
void CSFMLPlaceItems();
void CSFMLSetCoordinatesItems();
sfVector2f CSFMLItemConvertIndexToVector(iBlock blk);
void CSFMLHandleItems();


#endif /* CSFMLGameSnake_H */
