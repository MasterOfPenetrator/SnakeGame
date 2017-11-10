#ifndef CSFMLGameWeapons_H

#define CSFMLGameWeapons_H

// Includes
#include "CSFMLGameMain.h"

// Structs
typedef struct {
    float w;
    float h;
    float x;
    float y;
} wBlock;

typedef struct {
    int Damage;
    int Rate;
    float Speed;
    float Angle;
    float Distance;
} Weapon_Barrel;

typedef struct {
    char Name[50];
    size_t Barrells_Count;
    Weapon_Barrel Barrells[3];
    int Duration;
} Weapon;

typedef struct {
    // SFML Stuff
    sfTexture **GW_WeaponTexture;
    sfSprite *GW_WeaponSprite;
    sfShader **GW_WeaponShader;
    sfRenderStates *GW_WeaponShaderState;
    wBlock *GW_Weapon_Block;

    sfTexture **GW_BarrelTexture;
    sfSprite *GW_BarrelSprite;
    sfShader **GW_BarrelShader;
    sfRenderStates *GW_BarrelShaderState;
    wBlock **GW_Barrel_Block;
    size_t *GW_Barrel_Block_Count;

    // Weapons
    Weapon *GW_Weapons;
    bool *GW_Active;
    size_t GW_Weapon_Count;

    // Other
    bool GW_Fire_Pressed;
    bool GW_Is_Init;

} Weapons;

// Variables
Weapons GameWeapons;

// Functions
bool CSFMLInitWeapons();
void CSFMLQuitWeapons();
void CSFMLRenderWeapons();
void CSFMLHandleWeapons();

// Intern Functions
sfVector2f CSFMLWeaponConvertIndexToVector(wBlock blk);

#endif /* CSFMLGameWeapons_H */
