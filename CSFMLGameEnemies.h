#ifndef CSFMLGameEnemies_H

#define CSFMLGameEnemies_H

// Includes
#include "CSFMLGameMain.h"

// Structs
typedef struct {
    int GE_Health;
    int GE_Speed;
} Enemy;

// Functions
bool CSFMLEnemiesInit();
bool CSFMLHandleEnemies();
bool CSFMLRenderEnemies();
void CSFMLEnemiesQuit();

#endif /* CSFMLGameEnemies_H */

