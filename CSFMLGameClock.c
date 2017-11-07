#include "CSFMLGameClock.h"

bool CSFMLInitClock()
{
    // Game Timer initialisieren
    GameClock.GC_Clock = sfClock_create();

    if(GameClock.GC_Clock == NULL)
    {
        printf("Game Subsystem Fehler 'GameClock': Kann Uhr nicht initialisieren!\n");
        return false;
    }

    // Setup General things
    GameClock.GC_Actual_Time.microseconds = 0;
    GameClock.GC_Last_Time.microseconds = 0;
    GameClock.GC_DeltaTime = 0.0f;
    GameClock.GC_Time = 0.0f;
    GameClock.GC_SnakeTick = false;
    GameClock.GC_SnakeTime = 0.0f;
    GameClock.GC_SecondTick = false;
    GameClock.GC_SecondTime = 0.0f;
    GameClock.GC_Is_Init = true;

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

    return true;
}

void CSFMLQuitClock()
{
    // Clear SFML Stuff
    sfClock_destroy(GameClock.GC_Clock);
    GameClock.GC_Clock = NULL;

    // Reset General Things
    GameClock.GC_SnakeTick = false;
    GameClock.GC_Is_Init = false;
}



