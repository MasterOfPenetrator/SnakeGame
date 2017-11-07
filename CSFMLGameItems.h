#ifndef CSFMLGameItems_H

#define CSFMLGameItems_H

// Includes
#include "CSFMLGameMain.h"

// Enums
typedef enum { I_HEALTH, I_SCORE, I_SPEED, I_GOD, I_CLIP } ItemEffect;
typedef enum { I_INCREASE, I_DECREASE, I_NOTHING } ItemDirection;

// Structs
typedef struct {
    float x;
    float y;
    float w;
    float h;
} iBlock;

typedef struct {
    ItemType Type;
    size_t Actual_Count;
    size_t Max_Count;
} AllowedItems;

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
    sfSprite *GI_Sprite;
    sfRenderStates *GI_States;

    Item *GI_Items;
    size_t GI_Items_Count;

    iBlock *GI_Blocks;
    bool *GI_Placed;
    bool *GI_Coordinates_Setted;

    AllowedItems *GI_AllowedItems;
    size_t GI_AllowedItems_Count;

    sfText *GI_Text;
    sfFont *GI_Font;
    bool GI_TextRenderIt;
    float GI_TextAlpha;

    bool GI_Is_Init;

} Items;

// Variables
Items GameItem;

// Functions
bool CSFMLInitItems();
void CSFMLQuitItems();
void CSFMLPlaceItems();
void CSFMLSetCoordinatesItems();
void CSFMLRenderItems();
void CSFMLHandleItems();
void CSFMLItemSpawnText(const char *text);
void CSFMLRenderItemText();

// Intern Functions
sfVector2f CSFMLItemConvertIndexToVector(iBlock blk);
ItemType CSFMLGetItemType(Item Current);
bool CSFMLIncreaseItemCount(size_t ItemIndex);
void CSFMLDecreaseItemCount(size_t ItemIndex);

#endif /* CSFMLGameItems_H */
