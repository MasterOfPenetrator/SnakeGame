#include "CSFMLGameMain.h"

// Init Game
bool CSFMLGameInit()
{
    // Checkup Level ID
    if(!GameMain.GM_Level_ID)
    {
        printf("Game Subsystem Fehler 'GameMain': Kein Level angegeben!\n");
        return false;
    }

    // Init General Things
    bool Init_Error = false;

    // Init TileMap
    Init_Error |= !CSFMLLoadlevel(GameMain.GM_Level_ID);

    // Init Lights
    Init_Error |= !CSFMLInitLights();
    Init_Error |= !CSFMLSetViewportLight();

    // Init Clock
    Init_Error |= !CSFMLInitClock();

    // Init Snake
    Init_Error |= !CSFMLInitSnake();

    // Init Game Items
    Init_Error |= !CSFMLInitItems();

    // Init Game Highscore
    Init_Error |= !CSFMLInitHighscore();

    // Init Text for Score and Health
    GameMain.GM_Health = sfText_create();
    GameMain.GM_Score = sfText_create();
    GameMain.GM_Pause = sfText_create();
    GameMain.GM_Pause_Autokill = sfText_create();

    if(GameMain.GM_Health == NULL || GameMain.GM_Score == NULL || GameMain.GM_Pause == NULL || GameMain.GM_Pause_Autokill == NULL)
    {
        Init_Error |= true;
    }

    // Setup other things
    GameMain.GM_Is_Init = true;
    GameMain.GM_Paused = false;
    GameMain.GM_View_SumMovement = 0.0f;
    GameMain.GM_View_Movement_ResetFlag = false;
    GameMain.GM_View_Movement_RoundFlag = false;
    mstate.hs_written = false;

    return Init_Error;
}

// Clear Game
void CSFMLGameQuit()
{
    if(GameMain.GM_Is_Init)
    {
        // Clear Game Main Elements
        sfText_destroy(GameMain.GM_Health);
        sfText_destroy(GameMain.GM_Score);
        sfText_destroy(GameMain.GM_Pause);
        sfText_destroy(GameMain.GM_Pause_Autokill);
        GameMain.GM_Pause_Autokill = NULL;
        GameMain.GM_Pause = NULL;
        GameMain.GM_Health = NULL;
        GameMain.GM_Score = NULL;

        // Write the new Highscore Entry!
        CSFMLWriteHighscore();

        // Clear Elements
        CSFMLQuitHighscore();
        CSFMLQuitItems();
        CSFMLQuitSnake();
        CSFMLQuitClock();
        CSFMLQuitLights();
        CSFMLQuitLevel();

        // Other Stuff
        GameMain.GM_View_SumMovement = 0.0f;
        GameMain.GM_View_Movement_ResetFlag = false;
        GameMain.GM_View_Movement_RoundFlag = false;
        GameMain.GM_Paused = false;
        GameMain.GM_Is_Init = false;
    }
}

// Render Game
void CSFMLGameUpdate()
{
    // Game Update centralized
    if(!GameSnake.S_Is_Dead)
    {
        // Update Stuff
        {
            // Update GameClock
            CSFMLUpdateClock();

            // Update Tick Based Stuff
            if(GameClock.GC_SnakeTick)
            {
                // Update Handles
                CSFMLHandleItems();
                CSFMLHandleSnake();

                GameClock.GC_Actual_Snake_Direction = GameSnake.S_Actual_Direction;
            }
        }

        // Render Stuff
        {
            // Move The View
            CSFMLMainMoveView();

            // Render Stuff
            // Viewed Stuff
            sfRenderWindow_setView(screen, Level.BG_View);

            // Render Level
            CSFMLRenderLevel();

            // Render Snake
            CSFMLRenderSnake();

            // Update Snake Light
            CSFMLSetSnakeLight();

            // Render Lights
            CSFMLRenderLights();

            // Render Items
            CSFMLRenderItems();

            // Default Viewed Stuff
            sfRenderWindow_setView(screen, sfRenderWindow_getDefaultView(screen));

            // Other Stuff
            CSFMLMainRenderOther();
        }
    }

}

// Move View by Snake Speed
void CSFMLMainMoveView()
{
    Direction a_dir = GameClock.GC_Actual_Snake_Direction;

    // Get Frame Movement depend on the Snake Direction
    float Frame_Movement = 0.0f;
    sfVector2f View_Movement = {0.0f, 0.0f};

    // Up or Down
    if(a_dir == UP || a_dir == DOWN)
    {
        // Calculate Frame Movement
        Frame_Movement = (GameSnake.SB_Head.h / ((1.0f / GameSnake.S_Speed) * 1000.0f)) * GameClock.GC_DeltaTime * 1000.0f;

        if(Frame_Movement + GameMain.GM_View_SumMovement < GameSnake.SB_Head.h)
        {
            GameMain.GM_View_SumMovement += Frame_Movement;
        }
        else
        {
            GameMain.GM_View_Movement_ResetFlag = true;
        }

        // Now check for Last Step and determine it
        if(GameClock.GC_SnakeTick && !CompareFloats(GameSnake.SB_Head.h, GameMain.GM_View_SumMovement) && GameMain.GM_View_Movement_ResetFlag)
        {
            // Get for this the Actual Movement
            Frame_Movement = GameSnake.SB_Head.w - GameMain.GM_View_SumMovement;
            GameMain.GM_View_Movement_ResetFlag = false;
            GameMain.GM_View_Movement_RoundFlag = true;

            // Reset the Sum
            GameMain.GM_View_SumMovement = 0.0f;
        }

        // Set Vector
        if(a_dir == UP)
        {
            View_Movement.x = 0.0f;
            View_Movement.y = -Frame_Movement;
        }
        else if(a_dir == DOWN)
        {
            View_Movement.x = 0.0f;
            View_Movement.y = Frame_Movement;
        }

        // Move
        sfView_move(Level.BG_View, View_Movement);

        // Round to Absolute Number
        // like 325.002 must be 325.000, otherwise, on longer moving, offsets still adds and will destroy movement
        // Perform a Round to nearest Head Height
        if(GameMain.GM_View_Movement_RoundFlag)
        {
            sfVector2f ActualView = sfView_getCenter(Level.BG_View);
            float Nearest = 100.0f / GameSnake.SB_Head.h;

            ActualView.x /= 100.0f;
            ActualView.y /= 100.0f;

            ActualView.x = roundf(ActualView.x * Nearest) / Nearest;
            ActualView.y = roundf(ActualView.y * Nearest) / Nearest;

            ActualView.x *= 100.0f;
            ActualView.y *= 100.0f;

            sfView_setCenter(Level.BG_View, ActualView);

            GameMain.GM_View_Movement_RoundFlag = false;
        }

    }
    // Left or Right
    else if(a_dir == LEFT || a_dir == RIGHT)
    {
        // Calculate Frame Movement
        Frame_Movement = (GameSnake.SB_Head.w / ((1.0f / GameSnake.S_Speed) * 1000.0f)) * GameClock.GC_DeltaTime * 1000.0f;

        if(Frame_Movement + GameMain.GM_View_SumMovement < GameSnake.SB_Head.w)
        {
            GameMain.GM_View_SumMovement += Frame_Movement;
        }
        else
        {
            GameMain.GM_View_Movement_ResetFlag = true;
        }

        // Now check for Last Step and determine it
        if(GameClock.GC_SnakeTick && !CompareFloats(GameSnake.SB_Head.w, GameMain.GM_View_SumMovement) && GameMain.GM_View_Movement_ResetFlag)
        {
            // Get for this the Actual Movement
            Frame_Movement = GameSnake.SB_Head.w - GameMain.GM_View_SumMovement;
            GameMain.GM_View_Movement_ResetFlag = false;
            GameMain.GM_View_Movement_RoundFlag = true;

            // Reset the Sum
            GameMain.GM_View_SumMovement = 0.0f;
        }

        // Set Vector
        if(a_dir == LEFT)
        {
            View_Movement.x = -Frame_Movement;
            View_Movement.y = 0.0f;
        }
        else if(a_dir == RIGHT)
        {
            View_Movement.x = Frame_Movement;
            View_Movement.y = 0.0f;
        }

        // Move
        sfView_move(Level.BG_View, View_Movement);

        // Round is enable?
        if(GameMain.GM_View_Movement_RoundFlag)
        {
            sfVector2f ActualView = sfView_getCenter(Level.BG_View);
            float Nearest = 100.0f / GameSnake.SB_Head.w;

            ActualView.x /= 100.0f;
            ActualView.y /= 100.0f;

            ActualView.x = roundf(ActualView.x * Nearest) / Nearest;
            ActualView.y = roundf(ActualView.y * Nearest) / Nearest;

            ActualView.x *= 100.0f;
            ActualView.y *= 100.0f;

            sfView_setCenter(Level.BG_View, ActualView);

            GameMain.GM_View_Movement_RoundFlag = false;
        }
    }
}

// Render Other Stuff
void CSFMLMainRenderOther()
{
    // Render Item Hint
    CSFMLRenderItemText();

    // Place Score and Health String
    CSFMLGameShowScore();
    CSFMLGameShowHealth();
    CSFMLGamePauseRenderText();

    // Place Background Pictures
    MenuPlaceIMG(0, 0, false, 0);
    MenuPlaceIMG(0, 200, true, 0);
}

// Show game score
void CSFMLGameShowScore()
{
    sfVector2f position = {200, 163};
    sfColor color = {233, 77, 0, 255};

    sfText_setPosition(GameMain.GM_Score, position);
    sfText_setFont(GameMain.GM_Score, media.spezial_font);
    sfText_setCharacterSize(GameMain.GM_Score, 20);
    sfText_setColor(GameMain.GM_Score, color);

    char score_string[50] = {0};
    char start[8] = "Score: ";
    char end[8] = " Points";
    char value[12];
    strncat(score_string, start, 8);
    itoa(GameSnake.S_Score, value, 10);
    strncat(score_string, value, 12);
    strncat(score_string, end, 8);

    sfText_setString(GameMain.GM_Score, score_string);

    sfRenderWindow_drawText(screen, GameMain.GM_Score, NULL);
}

// Show game health
void CSFMLGameShowHealth()
{
    sfVector2f position = {440, 163};
    sfColor color = {233, 77, 0, 255};

    sfText_setPosition(GameMain.GM_Health, position);
    sfText_setFont(GameMain.GM_Health, media.spezial_font);
    sfText_setCharacterSize(GameMain.GM_Health, 20);
    sfText_setColor(GameMain.GM_Health, color);

    char health_string[50] = {0};
    char start[9] = "Health: ";
    char end[3] = " %";
    char value[6];
    strncat(health_string, start, 9);
    itoa(GameSnake.S_Health, value, 10);
    strncat(health_string, value, 6);
    strncat(health_string, end, 3);

    sfText_setString(GameMain.GM_Health, health_string);

    sfRenderWindow_drawText(screen, GameMain.GM_Health, NULL);
}

// Set Game Paused
void CSFMLGamePause()
{
    // Toggle State
    GameMain.GM_Paused = !GameMain.GM_Paused;

    if(shader_enabled)
    {
        sfShader_setBoolUniform(Level.BG_Texture_Shader, "Desaturate", GameMain.GM_Paused);
        sfShader_setBoolUniform(GameSnake.SB_Head_Shader, "Desaturate", GameMain.GM_Paused);
        sfShader_setBoolUniform(GameSnake.SB_Body_Shader, "Desaturate", GameMain.GM_Paused);

        size_t i;
        for(i = 0; i<GameItem.GI_Items_Count; i++)
        {
            sfShader_setBoolUniform(GameItem.GI_Shaders[i], "Desaturate", GameMain.GM_Paused);
        }
    }
}

// Render Paused Text
void CSFMLGamePauseRenderText()
{
    // Show Text
    if(GameMain.GM_Paused)
    {
        sfText_setFont(GameMain.GM_Pause, media.normal_font);
        sfText_setCharacterSize(GameMain.GM_Pause, 30);
        sfText_setString(GameMain.GM_Pause, "Game Paused");

        sfColor color = {233, 77, 0, 255};
        sfVector2f Position = {SCREEN_WIDTH/2 - sfText_getLocalBounds(GameMain.GM_Pause).width/2, Level.BG_Screenposition.y + 472.0f/2 - sfText_getLocalBounds(GameMain.GM_Pause).height/2};
        sfText_setPosition(GameMain.GM_Pause, Position);
        sfText_setColor(GameMain.GM_Pause, color);

        sfRenderWindow_drawText(screen, GameMain.GM_Pause, NULL);

        if(Level.MD_Details.Autokill_Count > 0)
        {
            sfText_setFont(GameMain.GM_Pause_Autokill, media.normal_font);
            sfText_setCharacterSize(GameMain.GM_Pause_Autokill, 20);
            sfText_setString(GameMain.GM_Pause_Autokill, "Remember... Autokill still active...");
            sfText_setColor(GameMain.GM_Pause_Autokill, color);
            Position.x /= 1.75f;
            Position.y += 50;
            sfText_setPosition(GameMain.GM_Pause_Autokill, Position);
            sfRenderWindow_drawText(screen, GameMain.GM_Pause_Autokill, NULL);
        }
    }
}

