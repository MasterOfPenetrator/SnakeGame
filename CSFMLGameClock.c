#include "CSFMLGameClock.h"

bool CSFMLInitClock()
{
    // Init Game Timer
    GameClock.GC_Clock = sfClock_create();
    GameClock.GC_Autokill_Time = malloc(sizeof(float));

    if(GameClock.GC_Clock == NULL || GameClock.GC_Autokill_Time == NULL)
    {
        printf("Game Subsystem Fehler 'GameClock': Kann Uhr nicht initialisieren!\n");
        return false;
    }

    // Setup General things
    GameClock.GC_Actual_Snake_Direction = NONE;
    GameClock.GC_Actual_Time.microseconds = 0;
    GameClock.GC_Last_Time.microseconds = 0;
    GameClock.GC_DeltaTime = 0.0f;
    GameClock.GC_Time = 0.0f;
    GameClock.GC_SnakeTick = false;
    GameClock.GC_SnakeTime = 0.0f;
    GameClock.GC_SecondTick = false;
    GameClock.GC_SecondTime = 0.0f;
    GameClock.GC_MinuteTick = false;
    GameClock.GC_MinuteTime = 0.0f;
    GameClock.GC_Is_Init = true;
    GameClock.GC_ItemEvents = NULL; // Prevent dangling pointer
    GameClock.GC_ItemEvents_Count = 0;
    GameClock.GC_ItemEvents_Init = false;
    GameClock.GC_Autokill_Init = false;
    GameClock.GC_Autokill_Time[0] = 0.0f;

    return true;
}

bool CSFMLUpdateClock()
{
    bool Update_Error = false;

    if(!GameClock.GC_Is_Init)
    {
        Update_Error |= true;
        return Update_Error;
    }

    // Update GameTime
    GameClock.GC_Actual_Time = sfClock_getElapsedTime(GameClock.GC_Clock);
    GameClock.GC_DeltaTime = sfTime_asSeconds(GameClock.GC_Actual_Time) - sfTime_asSeconds(GameClock.GC_Last_Time);
    GameClock.GC_Time += GameClock.GC_DeltaTime;
    GameClock.GC_SnakeTime += GameClock.GC_DeltaTime;
    GameClock.GC_SecondTime += GameClock.GC_DeltaTime;
    GameClock.GC_MinuteTime += GameClock.GC_DeltaTime;
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
            sfShader_setFloatUniform(GameItem.GI_Shaders[it], "Time", GameClock.GC_Time);
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

    // Update MinuteTick Timer
    if(GameSnake.S_Is_Init)
    {
        if(GameClock.GC_MinuteTime > 60.0f)
        {
            GameClock.GC_MinuteTick = true;
            GameClock.GC_MinuteTime = 0.0f;
        }
        else
        {
            GameClock.GC_MinuteTick = false;
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

    // Update Snake Score by Time
    if(GameSnake.S_Is_Init && !GameSnake.S_Is_Dead && !GameMain.GM_Paused && GameClock.GC_SecondTick && Level.Is_Loaded)
    {
        GameSnake.S_Score += Level.MD_Details.Score_per_Second;
    }

    // Update Other Stuff
    Update_Error |= !CSFMLUpdateItemTimeEvents();
    Update_Error |= !CSFMLUpdateAutokill();

    return Update_Error;
}

// Update Item Time Events
bool CSFMLUpdateItemTimeEvents()
{
    if(GameClock.GC_ItemEvents_Count > 0 && !GameMain.GM_Paused)
    {
        // Iterate over all Items
        size_t i;
        for(i = 0; i<GameClock.GC_ItemEvents_Count; i++)
        {
            // Is Time Over?
            // If Yes, returning to normal and Delete TimeEvent
            if(GameClock.GC_Time > (GameClock.GC_ItemEvents[i].StartTime + (float)GameClock.GC_ItemEvents[i].Duration))
            {
                printf("Game Time: Item Event Deleted!\n");
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
                    printf("Game Time: Item Event Setted!\n");
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

    return true;
}

// Update Autokill
bool CSFMLUpdateAutokill()
{
    size_t i = 0;

    // Init Autokill
    if(!GameClock.GC_Autokill_Init && Level.Is_Loaded)
    {
        if(Level.MD_Details.Autokill_Count > 1)
        {
            // Update Mem
            GameClock.GC_Autokill_Time = realloc(GameClock.GC_Autokill_Time, Level.MD_Details.Autokill_Count * sizeof(float));

            if(GameClock.GC_Autokill_Time == NULL)
            {
                printf("Game Subsystem Fehler 'GameClock': Kann Speicher fuer Autokill Timer nicht reservieren!\n");
                return false;
            }

            // Fill Time with zeroes
            for(i = 0; i<Level.MD_Details.Autokill_Count; i++)
                GameClock.GC_Autokill_Time[i] = 0.0f;

        }

        GameClock.GC_Autokill_Init = true;
    }

    // Proceed Autokill
    if(Level.Is_Loaded && GameSnake.S_Is_Init && Level.MD_Details.Autokill_Count > 0)
    {
        for(i = 0; i<Level.MD_Details.Autokill_Count; i++)
        {
            GameClock.GC_Autokill_Time[i] += GameClock.GC_DeltaTime;

            if(GameClock.GC_Autokill_Time[i] > (60.0f / Level.MD_Details.Autokill[i].Duration))
            {
                switch(Level.MD_Details.Autokill[i].Effect)
                {
                    case IT_SPEED:
                        GameSnake.S_Speed += Level.MD_Details.Autokill[i].Amount;
                        GameSnake.S_DefaultSpeed += Level.MD_Details.Autokill[i].Amount;
                        printf("Autokill: Speed increased, to: %f !\n", GameSnake.S_DefaultSpeed);
                    break;

                    case IT_HEALTH:
                        if((1 + Level.MD_Details.Autokill[i].Amount) <= (GameSnake.S_Health - Level.MD_Details.Autokill[i].Amount))
                        {
                            GameSnake.S_Health -= Level.MD_Details.Autokill[i].Amount;
                            printf("Autokill: Health decreased!\n");
                        }
                        else
                        {
                            GameSnake.S_Health = 0;
                            GameSnake.S_Is_Dead = true;
                            printf("Autokill: Health decreased to Zero -> You're Dead Bitch!\n");
                        }
                    break;

                    default:
                    break;
                }

                GameClock.GC_Autokill_Time[i] = 0.0f;
            }
        }
    }

    return true;
}

// Clear Clock Elements
void CSFMLQuitClock()
{
    // Clear SFML Stuff
    sfClock_destroy(GameClock.GC_Clock);
    GameClock.GC_Clock = NULL;

    free(GameClock.GC_Autokill_Time);
    GameClock.GC_Autokill_Time = NULL;

    // Clearing Time Events
    if(GameClock.GC_ItemEvents_Init)
    {
        free(GameClock.GC_ItemEvents);
        GameClock.GC_ItemEvents = NULL;
    }

    // Reset General Things
    GameClock.GC_Actual_Snake_Direction = NONE;
    GameClock.GC_SecondTick = false;
    GameClock.GC_SnakeTick = false;
    GameClock.GC_MinuteTick = false;
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



