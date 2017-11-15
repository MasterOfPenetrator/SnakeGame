#include "CSFMLGameClock.h"

bool CSFMLInitClock()
{
    // Init Game Timer
    GameClock.GC_Clock = sfClock_create();

    if(GameClock.GC_Clock == NULL)
    {
        printf("Game Subsystem Fehler 'GameClock': Kann Uhr nicht initialisieren!\n");
        return false;
    }

    // Setup General things
    GameClock.GC_Actual_Snake_Direction = NONE;
    GameClock.GC_Prev_Snake_Direction = NONE;
    GameClock.GC_Actual_Time.microseconds = 0;
    GameClock.GC_Last_Time.microseconds = 0;
    GameClock.GC_DeltaTime = 0.0f;
    GameClock.GC_Time = 0.0f;
    GameClock.GC_SnakeTick = false;
    GameClock.GC_SnakeTime = 0.0f;
    GameClock.GC_SecondTick = false;
    GameClock.GC_SecondTime = 0.0f;
    GameClock.GC_Is_Init = true;
    GameClock.GC_ItemEvents = NULL; // Prevent dangling pointer
    GameClock.GC_ItemEvents_Count = 0;
    GameClock.GC_ItemEvents_Init = false;

    return true;
}

bool CSFMLUpdateClock()
{
    if(!GameClock.GC_Is_Init)
        return false;

    // Update GameTime
    GameClock.GC_Actual_Time = sfClock_getElapsedTime(GameClock.GC_Clock);
    GameClock.GC_DeltaTime = sfTime_asSeconds(GameClock.GC_Actual_Time) - sfTime_asSeconds(GameClock.GC_Last_Time);
    GameClock.GC_Time += GameClock.GC_DeltaTime;
    GameClock.GC_SnakeTime += GameClock.GC_DeltaTime;
    GameClock.GC_SecondTime += GameClock.GC_DeltaTime;
    GameClock.GC_Last_Time = GameClock.GC_Actual_Time;

    // Set GameTime to Level Shader, if Level is loaded!
    if(Level.Is_Loaded && shader_enabled)
        sfShader_setFloatUniform(Level.BG_Texture_Shader, "Time", GameClock.GC_Time);

    // Set GameTime to Snake Shader, if Snake is init
    if(GameSnake.S_Is_Init && shader_enabled)
    {
        sfShader_setFloatUniform(GameSnake.SB_Head_Shader, "Time", GameClock.GC_Time);
        sfShader_setFloatUniform(GameSnake.SB_Body_Shader, "Time", GameClock.GC_Time);
    }

    // Set GameTime to Item Shaders
    if(GameItem.GI_Is_Init && shader_enabled)
    {
        size_t it;
        for(it = 0; it<GameItem.GI_Items_Count; it++)
        {
            if(GameItem.GI_Coordinates_Setted[it] && GameItem.GI_Placed[it])
            {
                sfShader_setFloatUniform(GameItem.GI_Shaders[it], "Time", GameClock.GC_Time);
            }
        }
    }

    // Set GameTime to Weapon Shaders
    if(GameWeapons.GW_Is_Init && shader_enabled)
    {
        size_t it;
        for(it = 0; it<GameWeapons.GW_Weapon_Count; it++)
        {
            sfShader_setIntUniform(GameWeapons.GW_BarrelShader[it], "Time", GameClock.GC_Time);
            sfShader_setIntUniform(GameWeapons.GW_WeaponShader[it], "Time", GameClock.GC_Time);
        }
    }

    // Update SecondTick Timer
    if(GameSnake.S_Is_Init)
    {
        if(GameClock.GC_SecondTime > 1.00f)
        {
            GameClock.GC_SecondTick = true;
            GameClock.GC_SecondTime = 0.0f;
        }
        else
        {
            GameClock.GC_SecondTick = false;
        }
    }

    // Update SnakeTick if Snake is Loaded!
    if(GameSnake.S_Is_Init)
    {
        if(GameClock.GC_SnakeTime > (1.00f / GameSnake.S_Speed))
        {
            GameClock.GC_SnakeTick = true;
            GameClock.GC_SnakeTime = 0.0f;
        }
        else
        {
            GameClock.GC_SnakeTick = false;
        }
    }

    // Update Time Events, only when Time Events putted
    if(GameClock.GC_ItemEvents_Count > 0)
    {
        // Iterate over all Items
        size_t i;
        for(i = 0; i<GameClock.GC_ItemEvents_Count; i++)
        {
            // Is Time Over?
            // If Yes, returning to normal and Delete TimeEvent
            if(GameClock.GC_Time > (GameClock.GC_ItemEvents[i].StartTime + (float)GameClock.GC_ItemEvents[i].Duration))
            {
                printf("Event Deleted!\n");
                // Restore Values
                switch(GameClock.GC_ItemEvents[i].Effect_Type)
                {
                    case HEALTH:
                        // Item Effect had a increased Effect
                        if(!GameClock.GC_ItemEvents[i].Direction)
                        {
                            if(GameSnake.S_Health - GameClock.GC_ItemEvents[i].Act_Value <= 0)
                            {
                                GameSnake.S_Health = 0;
                                GameSnake.S_Is_Dead = true;
                            }
                            else
                            {
                                GameSnake.S_Health -= GameClock.GC_ItemEvents[i].Act_Value;
                            }
                        }
                        // Item Effect had a decreased Effect
                        else
                        {
                            if((GameSnake.S_Health + GameClock.GC_ItemEvents[i].Act_Value) >= 100)
                            {
                                GameSnake.S_Health = 100;
                            }
                            else
                            {
                                GameSnake.S_Health += GameClock.GC_ItemEvents[i].Act_Value;
                            }
                        }
                    break;

                    case SPEED:
                        // Item Effect had a increased Effect
                        if(!GameClock.GC_ItemEvents[i].Direction)
                        {
                            if((GameSnake.S_Speed - GameClock.GC_ItemEvents[i].Act_Value) < 0 || (GameSnake.S_Speed - GameClock.GC_ItemEvents[i].Act_Value) < GameSnake.S_DefaultSpeed)
                            {
                                GameSnake.S_Speed = GameSnake.S_DefaultSpeed;
                            }
                            else
                            {
                                GameSnake.S_Speed -= (float)GameClock.GC_ItemEvents[i].Act_Value;
                            }
                        }
                        // Item Effect had a decreased Effect
                        else
                        {
                            // No Upper Limit for Speed
                            GameSnake.S_Speed += (float)GameClock.GC_ItemEvents[i].Act_Value;
                        }
                    break;

                    case SCORE:
                        // Item Effect had a increased Effect
                        if(!GameClock.GC_ItemEvents[i].Direction)
                        {
                            if((GameSnake.S_Score - GameClock.GC_ItemEvents[i].Act_Value) <= 0)
                            {
                                GameSnake.S_Score = 0;
                            }
                            else
                            {
                                GameSnake.S_Score -= GameClock.GC_ItemEvents[i].Act_Value;
                            }
                        }
                        // Item Effect had a decreased Effect
                        else
                        {
                            // No Upper Limit for Score
                            GameSnake.S_Score += GameClock.GC_ItemEvents[i].Act_Value;
                        }
                    break;

                    case GOD:
                        GameSnake.S_GODMODE = (bool)GameClock.GC_ItemEvents[i].Act_Value;
                    break;

                    case NOCLIP:
                        GameSnake.S_NOCLIP = (bool)GameClock.GC_ItemEvents[i].Act_Value;
                    break;

                    default:
                    break;
                }

                // Delete Element by ID
                if(!CSFMLDeleteTimeEventByIndex(i))
                {
                    printf("Game Subsystem Fehler 'GameClock': Kann Element nicht loeschen!\n");
                    return false;
                }
            }
            else
            {
                // Set Time, if it not setted before!
                if(!GameClock.GC_ItemEvents[i].Event_Setted)
                {
                    printf("Event Setted!\n");
                    switch(GameClock.GC_ItemEvents[i].Effect_Type)
                    {
                        case HEALTH:
                            // Item Effect had a increased Effect
                            if(!GameClock.GC_ItemEvents[i].Direction)
                            {
                                if((GameSnake.S_Health + GameClock.GC_ItemEvents[i].Act_Value) >= 100)
                                {
                                    GameSnake.S_Health = 100;
                                }
                                else
                                {
                                    GameSnake.S_Health += GameClock.GC_ItemEvents[i].Act_Value;
                                }
                            }
                            // Item Effect had a decreased Effect
                            else
                            {
                                if(GameSnake.S_Health - GameClock.GC_ItemEvents[i].Act_Value <= 0)
                                {
                                    GameSnake.S_Health = 0;
                                    GameSnake.S_Is_Dead = true;
                                }
                                else
                                {
                                    GameSnake.S_Health -= GameClock.GC_ItemEvents[i].Act_Value;
                                }
                            }
                        break;

                        case SPEED:
                            // Item Effect had a increased Effect
                            if(!GameClock.GC_ItemEvents[i].Direction)
                            {
                                // No Upper Limit for Speed
                                GameSnake.S_Speed += (float)GameClock.GC_ItemEvents[i].Act_Value;
                            }
                            // Item Effect had a decreased Effect
                            else
                            {
                                if((GameSnake.S_Speed - GameClock.GC_ItemEvents[i].Act_Value) < 0 || (GameSnake.S_Speed - GameClock.GC_ItemEvents[i].Act_Value) < GameSnake.S_DefaultSpeed)
                                {
                                    GameSnake.S_Speed = GameSnake.S_DefaultSpeed;
                                }
                                else
                                {
                                    GameSnake.S_Speed -= (float)GameClock.GC_ItemEvents[i].Act_Value;
                                }
                            }
                        break;

                        case SCORE:
                            // Item Effect had a increased Effect
                            if(!GameClock.GC_ItemEvents[i].Direction)
                            {
                                // No Upper Limit for Score
                                GameSnake.S_Score += GameClock.GC_ItemEvents[i].Act_Value;
                            }
                            // Item Effect had a decreased Effect
                            else
                            {
                                if((GameSnake.S_Score - GameClock.GC_ItemEvents[i].Act_Value) <= 0)
                                {
                                    GameSnake.S_Score = 0;
                                }
                                else
                                {
                                    GameSnake.S_Score -= GameClock.GC_ItemEvents[i].Act_Value;
                                }
                            }
                        break;

                        case GOD:
                            GameSnake.S_GODMODE = (bool)GameClock.GC_ItemEvents[i].Act_Value;
                        break;

                        case NOCLIP:
                            GameSnake.S_NOCLIP = (bool)GameClock.GC_ItemEvents[i].Act_Value;
                        break;

                        default:
                        break;
                    }

                    // Set TimeEvent Setted
                    GameClock.GC_ItemEvents[i].Event_Setted = true;
                }
            }
        }
    }

    // Change Direction affecting to Snake Tick
    if(GameClock.GC_SnakeTick)
    {
        GameClock.GC_Actual_Snake_Direction = GameSnake.S_Actual_Direction;
        GameClock.GC_Prev_Snake_Direction = GameSnake.S_Prev_Direction;
    }

    return true;
}

void CSFMLQuitClock()
{
    // Clear SFML Stuff
    sfClock_destroy(GameClock.GC_Clock);
    GameClock.GC_Clock = NULL;

    // Clearing Time Events
    if(GameClock.GC_ItemEvents_Init)
    {
        free(GameClock.GC_ItemEvents);
        GameClock.GC_ItemEvents = NULL;
    }

    // Reset General Things
    GameClock.GC_Actual_Snake_Direction = NONE;
    GameClock.GC_Prev_Snake_Direction = NONE;
    GameClock.GC_SecondTick = false;
    GameClock.GC_SnakeTick = false;
    GameClock.GC_Is_Init = false;
}

// Adding Time Event
bool CSFMLAddTimeEvent(EffectType Effect, int Duration, int NewValue, bool Dir)
{
    // There 0 Time Events
    if(GameClock.GC_ItemEvents_Count == 0)
    {
        // Init Initial Memory
        GameClock.GC_ItemEvents = malloc(sizeof(TimeEvent));

        if(GameClock.GC_ItemEvents == NULL)
        {
            printf("Game Subsystem Fehler 'GameClock': Kann keinen Speicher fuer TimeEvents reservieren!\n");
            return false;
        }

        // Now Fill Event with Values
        GameClock.GC_ItemEvents[0].Effect_Type = Effect;
        GameClock.GC_ItemEvents[0].Duration = Duration;
        GameClock.GC_ItemEvents[0].StartTime = GameClock.GC_Time;
        GameClock.GC_ItemEvents[0].Act_Value = NewValue;
        GameClock.GC_ItemEvents[0].Direction = Dir;
        GameClock.GC_ItemEvents[0].Event_Setted = false;

        switch(Effect)
        {
            case HEALTH:
                GameClock.GC_ItemEvents[0].Prev_Value = GameSnake.S_Health;
            break;

            case SPEED:
                GameClock.GC_ItemEvents[0].Prev_Value = (int)GameSnake.S_Speed;
            break;

            case SCORE:
                GameClock.GC_ItemEvents[0].Prev_Value = GameSnake.S_Score;
            break;

            case GOD:
                GameClock.GC_ItemEvents[0].Prev_Value = (int)GameSnake.S_GODMODE;
            break;

            case NOCLIP:
                GameClock.GC_ItemEvents[0].Prev_Value = (int)GameSnake.S_NOCLIP;
            break;

            default:
                printf("Game Subsystem Fehler 'GameClock': Unbekannter Effect Type!\n");
                return false;
            break;
        }

        // Count up
        GameClock.GC_ItemEvents_Count++;
        GameClock.GC_ItemEvents_Init = true;
    }
    else
    {
        // Init Memory Only, if No Memory allocated!
        if((GameClock.GC_ItemEvents_Count+1) != (sizeof(GameClock.GC_ItemEvents) / sizeof(GameClock.GC_ItemEvents[0])))
        {
            GameClock.GC_ItemEvents = realloc(GameClock.GC_ItemEvents, (GameClock.GC_ItemEvents_Count+1)*sizeof(TimeEvent));

            if(GameClock.GC_ItemEvents == NULL)
            {
                printf("Game Subsystem Fehler 'GameClock': Kann keinen zusaetzlichen Speicher fuer ItemEvents anlegen!\n");
                return false;
            }
        }

        // Now Fill Event with Values
        GameClock.GC_ItemEvents[GameClock.GC_ItemEvents_Count].Effect_Type = Effect;
        GameClock.GC_ItemEvents[GameClock.GC_ItemEvents_Count].Duration = Duration;
        GameClock.GC_ItemEvents[GameClock.GC_ItemEvents_Count].StartTime = GameClock.GC_Time;
        GameClock.GC_ItemEvents[GameClock.GC_ItemEvents_Count].Act_Value = NewValue;
        GameClock.GC_ItemEvents[GameClock.GC_ItemEvents_Count].Direction = Dir;
        GameClock.GC_ItemEvents[GameClock.GC_ItemEvents_Count].Event_Setted = false;

        switch(Effect)
        {
            case HEALTH:
                GameClock.GC_ItemEvents[GameClock.GC_ItemEvents_Count].Prev_Value = GameSnake.S_Health;
            break;

            case SPEED:
                GameClock.GC_ItemEvents[GameClock.GC_ItemEvents_Count].Prev_Value = (int)GameSnake.S_Speed;
            break;

            case SCORE:
                GameClock.GC_ItemEvents[GameClock.GC_ItemEvents_Count].Prev_Value = GameSnake.S_Score;
            break;

            case GOD:
                GameClock.GC_ItemEvents[GameClock.GC_ItemEvents_Count].Prev_Value = (int)GameSnake.S_GODMODE;
            break;

            case NOCLIP:
                GameClock.GC_ItemEvents[GameClock.GC_ItemEvents_Count].Prev_Value = (int)GameSnake.S_NOCLIP;
            break;

            default:
                printf("Game Subsystem Fehler 'GameClock': Unbekannter Effect Type!\n");
                return false;
            break;
        }

        // Count up
        GameClock.GC_ItemEvents_Count++;
    }

    return true;
}

// Deleting the latest Time Event
bool CSFMLDeleteTimeEventByIndex(size_t index)
{
    // Checkup
    if(index > GameClock.GC_ItemEvents_Count)
        return false;

    // Shift Elements
    size_t i;
    for(i = index; i<GameClock.GC_ItemEvents_Count-1; i++)
        GameClock.GC_ItemEvents[i] = GameClock.GC_ItemEvents[i+1];

    // Count down
    GameClock.GC_ItemEvents_Count--;

    return true;
}



