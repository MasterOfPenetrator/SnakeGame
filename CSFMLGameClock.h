#ifndef CSFMLGameClock_H

#define CSFMLGameClock_H

// Includes
#include "CSFMLGameMain.h"

// Enums
typedef enum {HEALTH, SPEED, SCORE, GOD, NOCLIP } EffectType;

// Structures
typedef struct {
    int Duration;
    int Prev_Value;
    int Act_Value;
    float StartTime;
    bool Direction; // 0 = Increase, 1 = Decrease
    bool Event_Setted;
    EffectType Effect_Type;
} TimeEvent;

typedef struct {
    sfClock *GC_Clock;
    sfTime GC_Actual_Time;
    sfTime GC_Last_Time;
    Direction GC_Actual_Snake_Direction;
    volatile bool GC_SnakeTick;
    volatile bool GC_SecondTick;
    volatile bool GC_MinuteTick;
    float GC_DeltaTime;
    float GC_SnakeTime;
    float GC_SecondTime;
    float GC_MinuteTime;
    float GC_Time;
    TimeEvent *GC_ItemEvents;
    size_t GC_ItemEvents_Count;

    float *GC_Autokill_Time;
    bool GC_Autokill_Init;

    bool GC_ItemEvents_Init;
    bool GC_Is_Init;
} Clock;

// Variables
Clock GameClock;

// Functions
bool CSFMLInitClock();
bool CSFMLUpdateClock();
bool CSFMLUpdateItemTimeEvents();
bool CSFMLUpdateAutokill();
void CSFMLQuitClock();
bool CSFMLAddTimeEvent(EffectType Effect, int Duration, int NewValue, bool Dir);
bool CSFMLDeleteTimeEventByIndex(size_t index);

#endif /* CSFMLGameClock_H */
