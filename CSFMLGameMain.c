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

    // Init Game Weapons
    Init_Error |= !CSFMLInitWeapons();

    // Init Text for Score and Health
    GameMain.GM_Font = sfFont_createFromFile("Bilder/Schriftarten/3Dventure.ttf");
    GameMain.GM_Health = sfText_create();
    GameMain.GM_Score = sfText_create();

    if(GameMain.GM_Font == NULL || GameMain.GM_Health == NULL || GameMain.GM_Score == NULL)
    {
        Init_Error |= true;
    }

    // Setup other things
    GameMain.GM_Is_Init = true;
    GameMain.GM_View_SumMovement = 0.0f;
    GameMain.GM_View_Movement_ResetFlag = false;
    GameMain.GM_View_StartFlag = false;

    return Init_Error;
}

// Clear Game
void CSFMLGameQuit()
{
    // Clear Game Main Elements
    sfFont_destroy(GameMain.GM_Font);
    sfText_destroy(GameMain.GM_Health);
    sfText_destroy(GameMain.GM_Score);
    GameMain.GM_Font = NULL;
    GameMain.GM_Health = NULL;
    GameMain.GM_Score = NULL;

    // Clear Elements
    CSFMLQuitWeapons();
    CSFMLQuitItems();
    CSFMLQuitSnake();
    CSFMLQuitClock();
    CSFMLQuitLights();
    CSFMLQuitLevel();

    // Other Stuff
    GameMain.GM_View_SumMovement = 0.0f;
    GameMain.GM_View_Movement_ResetFlag = false;
    GameMain.GM_View_StartFlag = false;
    GameMain.GM_Is_Init = false;
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
                CSFMLHandleWeapons();
                CSFMLHandleItems();
                CSFMLHandleSnake();
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

            // Render Weapons
            CSFMLRenderWeapons();

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
    Direction p_dir = GameClock.GC_Prev_Snake_Direction;

    // Detect Start of the Movement!
    if(a_dir != NONE && p_dir == NONE)
        GameMain.GM_View_StartFlag = true;

    // Get Frame Movement depend on the Snake Direction
    if(GameMain.GM_View_StartFlag)
    {
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
                Frame_Movement = 0.0f;
                GameMain.GM_View_Movement_ResetFlag = true;
            }

            // Now check for Last Step and determine it
            if(GameClock.GC_SnakeTick && !CompareFloats(GameSnake.SB_Head.h, GameMain.GM_View_SumMovement) && GameMain.GM_View_Movement_ResetFlag)
            {
                // Get for this the Actual Movement
                Frame_Movement = GameSnake.SB_Head.w - GameMain.GM_View_SumMovement;
                GameMain.GM_View_Movement_ResetFlag = false;

                printf("Tick Ende\n");

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
                Frame_Movement = 0.0f;
                GameMain.GM_View_Movement_ResetFlag = true;
            }

            // Now check for Last Step and determine it
            if(GameClock.GC_SnakeTick && !CompareFloats(GameSnake.SB_Head.w, GameMain.GM_View_SumMovement) && GameMain.GM_View_Movement_ResetFlag)
            {
                // Get for this the Actual Movement
                Frame_Movement = GameSnake.SB_Head.w - GameMain.GM_View_SumMovement;
                GameMain.GM_View_Movement_ResetFlag = false;

                printf("Tick Ende\n");

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
        }

        // Test Ausgabe
        sfVector2f MOV = sfView_getCenter(Level.BG_View);
        printf("View X: %f - View Y: %f\n", MOV.x, MOV.y);
        printf("Move X: %f - Move Y: --\n", GameMain.GM_View_SumMovement);
    }
}

// Render Other Stuff
void CSFMLMainRenderOther()
{
    // Render Item Hint
    CSFMLRenderItemText();

    // Activate Time Score Update
    CSFMLGameUpdateTimeSnakeScore();

    // Place Score and Health String
    CSFMLGameShowScore();
    CSFMLGameShowHealth();

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
    sfText_setFont(GameMain.GM_Score, GameMain.GM_Font);
    sfText_setCharacterSize(GameMain.GM_Score, 20);
    sfText_setColor(GameMain.GM_Score, color);

    char score_string[50] = {0};
    char start[8] = "Score: ";
    char end[8] = " Points";
    char value[12];
    strcat(score_string, start);
    itoa(GameSnake.S_Score, value, 10);
    strcat(score_string, value);
    strcat(score_string, end);

    sfText_setString(GameMain.GM_Score, score_string);

    sfRenderWindow_drawText(screen, GameMain.GM_Score, NULL);
}

// Show game health
void CSFMLGameShowHealth()
{
    sfVector2f position = {440, 163};
    sfColor color = {233, 77, 0, 255};

    sfText_setPosition(GameMain.GM_Health, position);
    sfText_setFont(GameMain.GM_Health, GameMain.GM_Font);
    sfText_setCharacterSize(GameMain.GM_Health, 20);
    sfText_setColor(GameMain.GM_Health, color);

    char health_string[50] = {0};
    char start[9] = "Health: ";
    char end[3] = " %";
    char value[6];
    strcat(health_string, start);
    itoa(GameSnake.S_Health, value, 10);
    strcat(health_string, value);
    strcat(health_string, end);

    sfText_setString(GameMain.GM_Health, health_string);

    sfRenderWindow_drawText(screen, GameMain.GM_Health, NULL);
}

// Update Score for Seconds
void CSFMLGameUpdateTimeSnakeScore()
{
    if(GameClock.GC_SecondTick && GameSnake.S_Is_Init && !GameSnake.S_Is_Dead)
        GameSnake.S_Score += Level.MD_Details.Score_per_Second;
}
