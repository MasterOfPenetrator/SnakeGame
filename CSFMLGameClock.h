#ifndef CSFMLGameClock_H

#define CSFMLGameClock_H

// Includes
#include "CSFMLGameMain.h"

// Structures
typedef struct {
    sfClock *GC_Clock;
    sfTime GC_Actual_Time;
    sfTime GC_Last_Time;
    bool GC_SnakeTick;
    bool GC_SecondTick;
    float GC_DeltaTime;
    float GC_SnakeTime;
    float GC_SecondTime;
    float GC_Time;
    bool GC_Is_Init;
} Clock;

// Variables
Clock GameClock;

// Functions
bool CSFMLInitClock();
bool CSFMLUpdateClock();
void CSFMLQuitClock();

#endif /* CSFMLGameClock_H */
