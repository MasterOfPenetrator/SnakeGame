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
    sfText *Text;
    bool RenderIt;
    float Alpha;
} ItemSpawnText;

typedef struct {
    iBlock Block;
    Item Current;
    size_t ItemIndex;
} SpawnedItem;

typedef struct {
    sfTexture **GI_Textures;
    sfShader **GI_Shaders;
    sfSprite *GI_Sprite;
    sfRenderStates *GI_States;

    Item *GI_Items;
    size_t GI_Items_Count;

    SpawnedItem *GI_Spawned;
    size_t GI_Spawned_Count;
    size_t GI_Spawned_Used;

    AllowedItems *GI_AllowedItems;
    size_t GI_AllowedItems_Count;

    ItemSpawnText *GI_ItemTexts;
    size_t GI_ItemTexts_Count;
    size_t GI_ItemTexts_Used;

    bool GI_Is_Init;

} Items;

// Variables
Items GameItem;

// Functions
bool CSFMLInitItems();
void CSFMLQuitItems();
void CSFMLRenderItems();
void CSFMLHandleItems();
bool CSFMLItemSpawnText(const char *text);
void CSFMLRenderItemText();

// Intern Functions
sfVector2f CSFMLItemConvertIndexToVector(iBlock blk);
ItemType CSFMLGetItemType(Item Current);
bool CSFMLIncreaseItemCount(size_t ItemIndex);
void CSFMLDecreaseItemCount(size_t ItemIndex);

#endif /* CSFMLGameItems_H */
