#include "CSFMLGameSnake.h"

// Init Snake
bool CSFMLInitSnake()
{
    if(!GameClock.GC_Is_Init || !GameLight_Is_Init || !Level.Is_Loaded)
        return false;

    // Init Textures, Shaders
    GameSnake.SB_Head_Texture = sfTexture_createFromFile("Bilder/Game/Tiles/Snake_Head.png", NULL);
    GameSnake.SB_Body_Texture = sfTexture_createFromFile("Bilder/Game/Tiles/Snake_Body.png", NULL);
    GameSnake.SB_Head_Shader = sfShader_createFromFile(NULL, NULL, "Bilder/Shader/Snake_Head_Shader.frag");
    GameSnake.SB_Body_Shader = sfShader_createFromFile(NULL, NULL, "Bilder/Shader/Snake_Body_Shader.frag");

    if(GameSnake.SB_Head_Texture == NULL || GameSnake.SB_Head_Shader == NULL || GameSnake.SB_Body_Texture == NULL || GameSnake.SB_Body_Shader == NULL)
    {
        printf("Game Subsystem Fehler 'GameSnake': Kann Snake Texturen oder Shader nicht laden!\n");
        return false;
    }

    // Setup Snake Shaders
    sfVector2f res = {sfTexture_getSize(GameSnake.SB_Head_Texture).x, sfTexture_getSize(GameSnake.SB_Head_Texture).y};
    sfShader_setTextureUniform(GameSnake.SB_Head_Shader, "Head_Texture", GameSnake.SB_Head_Texture);
    sfShader_setBoolUniform(GameSnake.SB_Head_Shader, "Head_Death", false);
    sfShader_setVec2Uniform(GameSnake.SB_Head_Shader, "Head_Resolution", res);
    sfShader_setTextureUniform(GameSnake.SB_Body_Shader, "Body_Texture", GameSnake.SB_Body_Texture);
    sfShader_setBoolUniform(GameSnake.SB_Body_Shader, "Body_Death", false);
    GameSnake.SB_Head_State.blendMode = sfBlendAlpha;
    GameSnake.SB_Head_State.shader = GameSnake.SB_Head_Shader;
    GameSnake.SB_Head_State.texture = GameSnake.SB_Head_Texture;
    GameSnake.SB_Head_State.transform = sfTransform_Identity;
    GameSnake.SB_Body_State.blendMode = sfBlendAlpha;
    GameSnake.SB_Body_State.shader = GameSnake.SB_Body_Shader;
    GameSnake.SB_Body_State.texture = GameSnake.SB_Body_Texture;
    GameSnake.SB_Body_State.transform = sfTransform_Identity;

    // Init Snake Blocks
    GameSnake.SB_Head.x = Level.TL_Start_Position.x;
    GameSnake.SB_Head.y = Level.TL_Start_Position.y;
    GameSnake.SB_Head.w = SNAKE_PICTURE_SIZE;
    GameSnake.SB_Head.h = SNAKE_PICTURE_SIZE;

    GameSnake.SB_Body = malloc(SNAKE_START_BLOCKS * sizeof(Block));

    if(GameSnake.SB_Body == NULL)
    {
        printf("Game Subsystem Fehler 'GameSnake': Kann Speicher fuer Blocks nicht laden!\n");
        return false;
    }

    size_t i;
    for(i = 0; i<SNAKE_START_BLOCKS; i++)
    {
        GameSnake.SB_Body[i].w = SNAKE_PICTURE_SIZE;
        GameSnake.SB_Body[i].h = SNAKE_PICTURE_SIZE;
        GameSnake.SB_Body[i].x = GameSnake.SB_Head.x - (((float)i/2)+0.5f);
        GameSnake.SB_Body[i].y = GameSnake.SB_Head.y;
    }

    // Setup general things
    strcpy(GameSnake.S_Name, "Youre Cool!");
    GameSnake.S_Health = 100;
    GameSnake.S_Score = 0;
    GameSnake.S_Speed = 2.0f;
    GameSnake.S_DefaultSpeed = 2.0f;
    GameSnake.S_EndItemTime = 0.0f;
    GameSnake.SB_Body_Elements = SNAKE_START_BLOCKS;
    GameSnake.S_Actual_Direction = NONE;
    GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
    GameSnake.S_Rotate = 0;
    GameSnake.S_LightDistance = 200.0f;
    GameSnake.S_Light_FBM = false;
    GameSnake.S_Light_OnOff = true;
    GameSnake.S_Is_Init = true;
    GameSnake.S_GODMODE = false;
    GameSnake.S_NOCLIP = false;

    return true;
}

// Render Snake
bool CSFMLRenderSnake()
{
    if(!GameSnake.S_Is_Init)
        return false;

    // Render Head
    sfSprite *head = sfSprite_create();
    sfSprite_setPosition(head, CSFMLSnakeConvertIndexToVector(GameSnake.SB_Head));
    sfSprite_setTexture(head, GameSnake.SB_Head_Texture, sfTrue);
    sfShader_setIntUniform(GameSnake.SB_Head_Shader, "Head_Rotate_Angle", GameSnake.S_Rotate);
    sfShader_setIntUniform(GameSnake.SB_Head_Shader, "Head_Health", GameSnake.S_Health);
    sfShader_setBoolUniform(GameSnake.SB_Head_Shader, "Head_Death", GameSnake.S_Is_Dead);
    sfRenderWindow_drawSprite(screen, head, &GameSnake.SB_Head_State);
    sfSprite_destroy(head);
    head = NULL;

    // Render Body
    size_t i;
    for(i = 0; i<GameSnake.SB_Body_Elements; i++)
    {
        sfSprite *body= sfSprite_create();
        sfSprite_setPosition(body, CSFMLSnakeConvertIndexToVector(GameSnake.SB_Body[i]));
        sfSprite_setTexture(body, GameSnake.SB_Body_Texture, sfTrue);
        sfShader_setIntUniform(GameSnake.SB_Body_Shader, "Body_Health", GameSnake.S_Health);
        sfShader_setBoolUniform(GameSnake.SB_Body_Shader, "Body_Death", GameSnake.S_Is_Dead);
        sfRenderWindow_drawSprite(screen, body, &GameSnake.SB_Body_State);
        sfSprite_destroy(body);
        body = NULL;
    }

    return true;
}

// Quit Snake
void CSFMLQuitSnake()
{
    // Clear SFML Stuff
    sfTexture_destroy(GameSnake.SB_Head_Texture);
    sfTexture_destroy(GameSnake.SB_Body_Texture);
    sfShader_destroy(GameSnake.SB_Head_Shader);
    sfShader_destroy(GameSnake.SB_Body_Shader);
    GameSnake.SB_Head_Texture = NULL;
    GameSnake.SB_Body_Texture = NULL;
    GameSnake.SB_Head_Shader = NULL;
    GameSnake.SB_Body_Shader = NULL;

    // Clear Block Stuff
    free(GameSnake.SB_Body);
    GameSnake.SB_Body = NULL;

    // Reset general things
    GameSnake.S_Actual_Direction = NONE;
    GameSnake.S_Prev_Direction = NONE;
    GameSnake.S_Health = 0;
    GameSnake.S_Speed = 0.0f;
    GameSnake.S_Is_Init = false;
    GameSnake.S_Is_Dead = false;
}

// Converting Head Array Indices to Screen Space Coordinates
sfVector2f CSFMLSnakeConvertIndexToVector(Block blk)
{
    sfVector2f Ret_Value;

    Ret_Value.x = (blk.x * Level.TL_X_Size) + Level.BG_Screenposition.x;
    Ret_Value.y = (blk.y * Level.TL_Y_Size) + Level.BG_Screenposition.y;

    return Ret_Value;
}


// Push a Snake Block
bool CSFMLPushSnakeBlock(Direction dir)
{
    if(!GameSnake.S_Is_Init)
        return false;

    if(dir == UP)
    {
        GameSnake.SB_Head.y -= (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }
    else if(dir == DOWN)
    {
        GameSnake.SB_Head.y += (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }
    else if(dir == LEFT)
    {
        GameSnake.SB_Head.x -= (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }
    else if(dir == RIGHT)
    {
        GameSnake.SB_Head.x += (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }

    return true;
}

// Pop a Snake Block
bool CSFMLPopSnakeBlock()
{
    if(!GameSnake.S_Is_Init)
        return false;

    // Move Blocks down
    size_t i;
    for(i = GameSnake.SB_Body_Elements; i>0; i--)
        GameSnake.SB_Body[i] = GameSnake.SB_Body[i-1];

    // Set First Element to last Head Block Element
    GameSnake.SB_Body[0].x = GameSnake.SB_Head.x;
    GameSnake.SB_Body[0].y = GameSnake.SB_Head.y;

    return true;
}

// Lets grow the fucking Snake..
bool CSFMLGrowSnake()
{

}

// Set Snake Light
bool CSFMLSetSnakeLight()
{
    if(!GameSnake.S_Is_Init)
        return false;

    // Licht setzen
    float light_x, light_y, cone_x, cone_y;
    if(GameSnake.S_Rotate == 0)
    {
        cone_x = (Level.TL_Start_Position.x * Level.TL_X_Size) + Level.BG_Screenposition.x + GameSnake.S_LightDistance;
        cone_y = (Level.TL_Start_Position.y * Level.TL_Y_Size) + Level.BG_Screenposition.y + SNAKE_PICTURE_SIZE/2;
        light_x = (Level.TL_Start_Position.x * Level.TL_X_Size) + Level.BG_Screenposition.x;
        light_y = (Level.TL_Start_Position.y * Level.TL_Y_Size) + Level.BG_Screenposition.y + SNAKE_PICTURE_SIZE/2;
    }
    else if(GameSnake.S_Rotate == 180)
    {
        cone_x = ((Level.TL_Start_Position.x+1) * Level.TL_X_Size) - (Level.BG_Screenposition.x + GameSnake.S_LightDistance);
        cone_y = (Level.TL_Start_Position.y * Level.TL_Y_Size) + Level.BG_Screenposition.y + SNAKE_PICTURE_SIZE/2;
        light_x = ((Level.TL_Start_Position.x+1) * Level.TL_X_Size) - Level.BG_Screenposition.x;
        light_y = (Level.TL_Start_Position.y * Level.TL_Y_Size) + Level.BG_Screenposition.y + SNAKE_PICTURE_SIZE/2;
    }
    else if(GameSnake.S_Rotate == 90)
    {
        cone_x = (Level.TL_Start_Position.x * Level.TL_X_Size) + Level.BG_Screenposition.x + SNAKE_PICTURE_SIZE/2;
        cone_y = ((Level.TL_Start_Position.y+1) * Level.TL_Y_Size) + Level.BG_Screenposition.y - GameSnake.S_LightDistance;
        light_x = (Level.TL_Start_Position.x * Level.TL_X_Size) + Level.BG_Screenposition.x + SNAKE_PICTURE_SIZE/2;
        light_y = ((Level.TL_Start_Position.y+1) * Level.TL_Y_Size) + Level.BG_Screenposition.y;
    }
    else if(GameSnake.S_Rotate == 270)
    {
        cone_x = (Level.TL_Start_Position.x * Level.TL_X_Size) + Level.BG_Screenposition.x + SNAKE_PICTURE_SIZE/2;
        cone_y = ((Level.TL_Start_Position.y) * Level.TL_Y_Size) + (Level.BG_Screenposition.y + GameSnake.S_LightDistance);
        light_x = (Level.TL_Start_Position.x * Level.TL_X_Size) + Level.BG_Screenposition.x + SNAKE_PICTURE_SIZE/2;;
        light_y = ((Level.TL_Start_Position.y) * Level.TL_Y_Size) + Level.BG_Screenposition.y;
    }
    else
    {
        cone_x = 0.0f;
        cone_y = 0.0f;
        light_x = 0.0f;
        light_y = 0.0f;
    }

    CSFMLSetLight(light_x, light_y, cone_x, cone_y, 0, true, GameSnake.S_Light_FBM, 20, 1, GameSnake.S_Light_OnOff);

    return true;
}

bool CSFMLCheckTileMapCollision()
{
    if(!GameSnake.S_Is_Init)
        return false;

    // Define new Position
    Direction dir = GameSnake.S_Actual_Direction;
    float fNew_X = GameSnake.SB_Head.x;
    float fNew_Y = GameSnake.SB_Head.y;

    if(dir == UP)
    {
        fNew_Y -= (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }
    else if(dir == DOWN)
    {
        fNew_Y += (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }
    else if(dir == LEFT)
    {
        fNew_X -= (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }
    else if(dir == RIGHT)
    {
        fNew_X += (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }

    size_t New_X = floor(fNew_X);
    size_t New_Y = floor(fNew_Y);

    if(New_X > MAX_TILES_X || New_Y > MAX_TILES_Y)
    {
        printf("Game Subsystem Fehler 'GameSnake': Berechnete Tiles ueber Max?!\n");
        return false;
    }

    // Degenerate Health and Change Direction
    bool Snake_Hit = false;
    if(Level.TL_Map[New_Y][New_X] == 1)
    {
        // Do DMG
        if(!GameSnake.S_GODMODE)
            GameSnake.S_Health -= SNAKE_BORDERHIT_DMG_1;

        // Change Direction
        Snake_Hit = true;
    }
    else if(Level.TL_Map[New_Y][New_X] == 2)
    {
        // Do DMG
        if(!GameSnake.S_GODMODE)
            GameSnake.S_Health -= SNAKE_BORDERHIT_DMG_2;

        // Change Direction
        Snake_Hit = true;
    }
    else if(Level.TL_Map[New_Y][New_X] == 3)
    {
        // Do DMG
        if(!GameSnake.S_GODMODE)
            GameSnake.S_Health -= SNAKE_BORDERHIT_DMG_3;

        // Change Direction
        Snake_Hit = true;
    }

    // Process Snake Health
    if(GameSnake.S_Health > 40)
    {
        GameSnake.S_Light_FBM = false;
    }
    else if(GameSnake.S_Health <= 40 && GameSnake.S_Health > 0)
    {
        GameSnake.S_Light_FBM = true;
    }
    else if(GameSnake.S_Health <= 0)
    {
        GameSnake.S_Light_FBM = true;
        GameSnake.S_Light_OnOff = false;
        GameSnake.S_Health = 0;
        GameSnake.S_Is_Dead = true;
        return false;
    }

    // Process Automatic Change Direction to Avoid Snake moves into a non-moveable Tile!
    if(Snake_Hit)
    {
        printf("Tile Map Collision Detected!\n");
        // Actual Direction Demand Process
        // Up or Down - Possible New Direction Left or Right, check for Another Hit
        if(dir == UP || dir == DOWN)
        {
            // Set Both First possible
            bool Left = true, Right = true;

            // Restore Old Values
            if(dir == UP)
                New_Y += 1;
            if(dir == DOWN)
                New_Y -= 1;

            // Map Maxiums reached for Left
            if(!(New_X-1 <= 0))
            {
                // Now Check Tile Map for Left
                if(Level.TL_Map[New_Y][New_X-1] == 1 || Level.TL_Map[New_Y][New_X-1] == 2 || Level.TL_Map[New_Y][New_X-1] == 3)
                    Left = false;
            }
            else
            {
                Left = false;
            }

            //Map Maximums reached for Right
            if(!(New_X+1 >= MAX_TILES_X))
            {
                // Now Check Tile Map for Right
                if(Level.TL_Map[New_Y][New_X+1] == 1 || Level.TL_Map[New_Y][New_X+1] == 2 || Level.TL_Map[New_Y][New_X+1] == 3)
                    Right = false;
            }
            else
            {
                Right = false;
            }

            // Now Change Direction or set Snake Dead
            if(!Right && !Left)
            {
                GameSnake.S_Is_Dead = true;
                return false;
            }
            else if(Right && !Left)
            {
                GameSnake.S_Actual_Direction = RIGHT;
            }
            else if(!Right && Left)
            {
                GameSnake.S_Actual_Direction = LEFT;
            }
            else
            {
                GameSnake.S_Actual_Direction = RIGHT;
            }
        }

        // Actual Direction Demand Process
        // Left or Right - Possible New Direction Up or Down, check for Another Hit
        if(dir == LEFT || dir == RIGHT)
        {
            // Set Both First possible
            bool Down = true, Up = true;

            // Restore Old Values
            if(dir == LEFT)
                New_X += 1;
            if(dir == RIGHT)
                New_X -= 1;

            // Map Maxiums reached for Down
            if(!(New_Y+1 >= MAX_TILES_Y))
            {
                // Now Check Tile Map for Down
                if(Level.TL_Map[New_Y+1][New_X] == 1 || Level.TL_Map[New_Y+1][New_X] == 2 || Level.TL_Map[New_Y+1][New_X] == 3)
                    Down = false;
            }
            else
            {
                Down = false;
            }

            //Map Maximums reached for Up
            if(!(New_Y-1 <= 0))
            {
                // Now Check Tile Map for Up
                if(Level.TL_Map[New_Y-1][New_X] == 1 || Level.TL_Map[New_Y-1][New_X] == 2 || Level.TL_Map[New_Y-1][New_X] == 3)
                    Up = false;
            }
            else
            {
                Up = false;
            }

            // Now Change Direction or set Snake Dead
            if(!Down && !Up)
            {
                if(!GameSnake.S_GODMODE)
                    GameSnake.S_Is_Dead = true;
            }
            else if(Down && !Up)
            {
                GameSnake.S_Actual_Direction = DOWN;
            }
            else if(!Down && Up)
            {
                GameSnake.S_Actual_Direction = UP;
            }
            else
            {
                GameSnake.S_Actual_Direction = DOWN;
            }
        }
    }

    // Is Snake Dead
    if(GameSnake.S_Is_Dead)
    {
        GameSnake.S_Light_FBM = true;
        GameSnake.S_Light_OnOff = false;
        GameSnake.S_Health = 0;
        return false;
    }

    return true;
}

// Check for self collision
bool CSFMLCheckSelfCollision()
{
    if(!GameSnake.S_Is_Init)
        return false;

    // Generate new Head X und Y Coordinates
    Direction dir = GameSnake.S_Actual_Direction;
    float fNew_X = GameSnake.SB_Head.x;
    float fNew_Y = GameSnake.SB_Head.y;

    if(dir == UP)
    {
        fNew_Y -= (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }
    else if(dir == DOWN)
    {
        fNew_Y += (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }
    else if(dir == LEFT)
    {
        fNew_X -= (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }
    else if(dir == RIGHT)
    {
        fNew_X += (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }

    // Iterate over all Body Elements and Check for Collision
    size_t i;
    bool Hit = false;
    for(i = 0; i<GameSnake.SB_Body_Elements; i++)
    {
        // Hit on Body
        if(GameSnake.SB_Body[i].x == fNew_X && GameSnake.SB_Body[i].y == fNew_Y)
        {
            if(!GameSnake.S_GODMODE)
                GameSnake.S_Health -= SNAKE_SELFHIT_DMG;
            Hit = true;
        }
    }

    // Process Snake Health
    if(GameSnake.S_Health > 40)
    {
        GameSnake.S_Light_FBM = false;
    }
    else if(GameSnake.S_Health <= 40 && GameSnake.S_Health > 0)
    {
        GameSnake.S_Light_FBM = true;
    }
    else if(GameSnake.S_Health <= 0)
    {
        GameSnake.S_Light_FBM = true;
        GameSnake.S_Light_OnOff = false;
        GameSnake.S_Health = 0;
        GameSnake.S_Is_Dead = true;
        return false;
    }

    // Self Hit Detected, calculate new direction, for beeing alived
    if(Hit)
    {
        printf("Selfhit Detected !\n");
        // Generate normalized Indices
        size_t New_X = floor(fNew_X);
        size_t New_Y = floor(fNew_Y);

        // If Self Hit Detected on Move Up or Down Direction
        if(dir == UP || dir == DOWN)
        {
            // Set Both First possible
            bool Left = true, Right = true;

            // Restore Old Values
            if(dir == UP)
                New_Y += 1;
            if(dir == DOWN)
                New_Y -= 1;

            // Check for TileMap Collision
            // For Possible Next Left Movement
            if(!(New_X-1 <= 0))
            {
                // Now Check Tile Map for Left
                if(Level.TL_Map[New_Y][New_X-1] == 1 || Level.TL_Map[New_Y][New_X-1] == 2 || Level.TL_Map[New_Y][New_X-1] == 3)
                    Left = false;
            }
            else
            {
                Left = false;
            }

            // For Possible Next Right Movement
            if(!(New_X+1 >= MAX_TILES_X))
            {
                // Now Check Tile Map for Right
                if(Level.TL_Map[New_Y][New_X+1] == 1 || Level.TL_Map[New_Y][New_X+1] == 2 || Level.TL_Map[New_Y][New_X+1] == 3)
                    Right = false;
            }
            else
            {
                Right = false;
            }

            // Check for Another Self-Hit
            float Left_X = fNew_X - ((float)SNAKE_PICTURE_SIZE/Level.TL_X_Size); // Minus Movement for Left Movement
            float Right_X = fNew_X + ((float)SNAKE_PICTURE_SIZE/Level.TL_X_Size); // Plus Movement for Right Movement

            // Restore Old Values
            float Move_Y = fNew_Y;
            if(dir == UP)
                Move_Y -= (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
            if(dir == DOWN)
                Move_Y += (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;

            for(i = 0; i<GameSnake.SB_Body_Elements; i++)
            {
                // Another Self Hit on Left Direction
                if(GameSnake.SB_Body[i].x == Left_X && GameSnake.SB_Body[i].y == Move_Y)
                    Left = false;

                // Another Self Hit on Right Direction
                if(GameSnake.SB_Body[i].x == Right_X && GameSnake.SB_Body[i].y == Move_Y)
                    Right = false;
            }

            // Check Up Right & Left Possibility
            // Now Change Direction or set Snake Dead
            if(!Right && !Left)
            {
                if(!GameSnake.S_GODMODE)
                    GameSnake.S_Is_Dead = true;
            }
            else if(Right && !Left)
            {
                GameSnake.S_Actual_Direction = RIGHT;
            }
            else if(!Right && Left)
            {
                GameSnake.S_Actual_Direction = LEFT;
            }
            else
            {
                GameSnake.S_Actual_Direction = LEFT;
            }
        }
        else if(dir == LEFT || dir == RIGHT)
        {
           // Set Both First possible
            bool Down = true, Up = true;

            // Restore Old Values
            if(dir == LEFT)
                New_X += 1;
            if(dir == RIGHT)
                New_X -= 1;

            // Map Maxiums reached for Down
            if(!(New_Y+1 >= MAX_TILES_Y))
            {
                // Now Check Tile Map for Down
                if(Level.TL_Map[New_Y+1][New_X] == 1 || Level.TL_Map[New_Y+1][New_X] == 2 || Level.TL_Map[New_Y+1][New_X] == 3)
                    Down = false;
            }
            else
            {
                Down = false;
            }


            //Map Maximums reached for Up
            if(!(New_Y-1 <= 0))
            {
                // Now Check Tile Map for Up
                if(Level.TL_Map[New_Y-1][New_X] == 1 || Level.TL_Map[New_Y-1][New_X] == 2 || Level.TL_Map[New_Y-1][New_X] == 3)
                    Up = false;
            }
            else
            {
                Up = false;
            }

            // Check for Another Self-Hit
            float Up_Y = fNew_Y - ((float)SNAKE_PICTURE_SIZE/Level.TL_X_Size); // Minus Movement for Left Movement
            float Down_Y = fNew_Y + ((float)SNAKE_PICTURE_SIZE/Level.TL_X_Size); // Plus Movement for Right Movement

            // Restore Old Values
            float Move_X = fNew_X;
            if(dir == LEFT)
                Move_X -= (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
            if(dir == RIGHT)
                Move_X += (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;

            for(i = 0; i<GameSnake.SB_Body_Elements; i++)
            {
                // Another Self Hit on Up Direction
                if(GameSnake.SB_Body[i].x == Move_X && GameSnake.SB_Body[i].y ==  Up_Y)
                    Up = false;

                // Another Self Hit on Down Direction
                if(GameSnake.SB_Body[i].x == Move_X && GameSnake.SB_Body[i].y == Down_Y)
                    Down = false;
            }

            // Check Up Up & Down Possibility
            // Now Change Direction or set Snake Dead
            if(!Down && !Up)
            {
                if(!GameSnake.S_GODMODE)
                    GameSnake.S_Is_Dead = true;
            }
            else if(Down && !Up)
            {
                GameSnake.S_Actual_Direction = DOWN;
            }
            else if(!Down && Up)
            {
                GameSnake.S_Actual_Direction = UP;
            }
            else
            {
                GameSnake.S_Actual_Direction = UP;
            }
        }
    }

     // Is Snake Dead
    if(GameSnake.S_Is_Dead)
    {
        GameSnake.S_Light_FBM = true;
        GameSnake.S_Light_OnOff = false;
        GameSnake.S_Health = 0;
        return false;
    }

    return true;
}

// Handle Snake Movement
bool CSFMLHandleSnake()
{
    if(!GameSnake.S_Is_Init)
        return false;

    // Move Snake with SnakeTick
    if(GameClock.GC_SnakeTick)
    {
        // Check TileMap Collide, Self Collide, if Noclip On, Ignore both
        // Its a bit Tricky, if first equation true, the if skips next elements, so they will not execute :)
        if(GameSnake.S_NOCLIP || (CSFMLCheckTileMapCollision() && CSFMLCheckSelfCollision()))
        {
            // Handle Items here!
            CSFMLHandleItems();

            // Right Movement
            if(GameSnake.S_Actual_Direction == RIGHT)
            {
                // Delete Last Block
                CSFMLPopSnakeBlock();

                // Just Move if Prev Move was not LEFT
                if(!(GameSnake.S_Prev_Direction == LEFT))
                {
                    // Update Rotation
                    if(GameSnake.S_Rotate != 0)
                    {
                        GameSnake.S_Rotate = 0;
                    }

                    // Move
                    CSFMLPushSnakeBlock(RIGHT);
                    sfVector2f Offset = {SNAKE_PICTURE_SIZE, 0};
                    sfView_move(Level.BG_View, Offset);
                    GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
                }
                else
                {
                    // Move
                    CSFMLPushSnakeBlock(LEFT);
                    sfVector2f Offset = {-SNAKE_PICTURE_SIZE, 0};
                    sfView_move(Level.BG_View, Offset);
                }
            }
            // Move Left
            else if(GameSnake.S_Actual_Direction == LEFT)
            {
                // Delete Last Block
                CSFMLPopSnakeBlock();

                // Just Move if Prev Move was not RIGHT
                if(!(GameSnake.S_Prev_Direction == RIGHT))
                {
                    // Update Rotation
                    if(GameSnake.S_Rotate != 180)
                    {
                        GameSnake.S_Rotate = 180;
                    }

                    // Move
                    CSFMLPushSnakeBlock(LEFT);
                    sfVector2f Offset = {-SNAKE_PICTURE_SIZE, 0};
                    sfView_move(Level.BG_View, Offset);
                    GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
                }
                else
                {
                    // Move
                    CSFMLPushSnakeBlock(RIGHT);
                    sfVector2f Offset = {SNAKE_PICTURE_SIZE, 0};
                    sfView_move(Level.BG_View, Offset);
                }
            }
            // Move Up
            else if(GameSnake.S_Actual_Direction == UP)
            {
                // Delete Last Block
                CSFMLPopSnakeBlock();

                // Just Move if Prev Move was not DOWN
                if(!(GameSnake.S_Prev_Direction == DOWN))
                {
                    // Update Rotation
                    if(GameSnake.S_Rotate != 90)
                    {
                        GameSnake.S_Rotate = 90;
                    }

                    // Move
                    CSFMLPushSnakeBlock(UP);
                    sfVector2f Offset = {0, -SNAKE_PICTURE_SIZE};
                    sfView_move(Level.BG_View, Offset);
                    GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
                }
                else
                {
                    // Move
                    CSFMLPushSnakeBlock(DOWN);
                    sfVector2f Offset = {0, SNAKE_PICTURE_SIZE};
                    sfView_move(Level.BG_View, Offset);
                }
            }
            // Move Down
            else if(GameSnake.S_Actual_Direction == DOWN)
            {
                // Delete Last Block
                CSFMLPopSnakeBlock();

                // Just Move if Prev Move was not UP
                if(!(GameSnake.S_Prev_Direction == UP))
                {
                    // Update Rotation
                    if(GameSnake.S_Rotate != 270)
                    {
                        GameSnake.S_Rotate = 270;
                    }

                    // Move
                    CSFMLPushSnakeBlock(DOWN);
                    sfVector2f Offset = {0, SNAKE_PICTURE_SIZE};
                    sfView_move(Level.BG_View, Offset);
                    GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
                }
                else
                {
                    // Move
                    CSFMLPushSnakeBlock(UP);
                    sfVector2f Offset = {0, -SNAKE_PICTURE_SIZE};
                    sfView_move(Level.BG_View, Offset);
                }
            }
        }
    }

    // Set Viewport
    sfRenderWindow_setView(screen, Level.BG_View);

    // Handle Items
    CSFMLRenderItems();

    // Place Light
    CSFMLSetSnakeLight();

    // Render Snake
    CSFMLRenderSnake();

    // Set Default Viewport
    sfRenderWindow_setView(screen, sfRenderWindow_getDefaultView(screen));

    return true;
}
