#include "CSFMLGameSnake.h"

// Init Snake
bool CSFMLInitSnake()
{
    // Check Up Init Stuff
    if(!GameClock.GC_Is_Init || !GameLight_Is_Init || !Level.Is_Loaded || !GameSnake.S_Name)
        return false;

    // Init Textures, Sprites
    GameSnake.SB_Head_Texture = sfTexture_createFromFile("Bilder/Game/Tiles/Snake_Head.png", NULL);
    GameSnake.SB_Body_Texture = sfTexture_createFromFile("Bilder/Game/Tiles/Snake_Body.png", NULL);
    GameSnake.SB_Head_Sprite = sfSprite_create();
    GameSnake.SB_Body_Sprite = sfSprite_create();

    if(GameSnake.SB_Head_Texture == NULL ||GameSnake.SB_Body_Texture == NULL || GameSnake.SB_Head_Sprite == NULL || GameSnake.SB_Body_Sprite == NULL)
    {
        printf("Game Subsystem Fehler 'GameSnake': Kann Snake Texturen oder Sprite nicht laden!\n");
        return false;
    }

    // Init Shader, if enabled
    if(shader_enabled)
    {
        GameSnake.SB_Head_Shader = sfShader_createFromFile(NULL, NULL, "Bilder/Shader/Snake_Head_Shader.frag");
        GameSnake.SB_Body_Shader = sfShader_createFromFile(NULL, NULL, "Bilder/Shader/Snake_Body_Shader.frag");

        if(GameSnake.SB_Head_Shader == NULL || GameSnake.SB_Body_Shader == NULL)
        {
            printf("Game Subsystem Fehler 'GameSnake': Kann Snake Shader nicht laden!\n");
            return false;
        }

        sfVector2f res = {sfTexture_getSize(GameSnake.SB_Head_Texture).x, sfTexture_getSize(GameSnake.SB_Head_Texture).y};
        sfShader_setTextureUniform(GameSnake.SB_Head_Shader, "Head_Texture", GameSnake.SB_Head_Texture);
        sfShader_setBoolUniform(GameSnake.SB_Head_Shader, "Head_Death", false);
        sfShader_setVec2Uniform(GameSnake.SB_Head_Shader, "Head_Resolution", res);
        sfShader_setBoolUniform(GameSnake.SB_Head_Shader, "Desaturate", false);
        sfShader_setTextureUniform(GameSnake.SB_Body_Shader, "Body_Texture", GameSnake.SB_Body_Texture);
        sfShader_setBoolUniform(GameSnake.SB_Body_Shader, "Body_Death", false);
        sfShader_setBoolUniform(GameSnake.SB_Body_Shader, "Desaturate", false);
        GameSnake.SB_Head_State.blendMode = sfBlendAlpha;
        GameSnake.SB_Head_State.shader = GameSnake.SB_Head_Shader;
        GameSnake.SB_Head_State.texture = GameSnake.SB_Head_Texture;
        GameSnake.SB_Head_State.transform = sfTransform_Identity;
        GameSnake.SB_Body_State.blendMode = sfBlendAlpha;
        GameSnake.SB_Body_State.shader = GameSnake.SB_Body_Shader;
        GameSnake.SB_Body_State.texture = GameSnake.SB_Body_Texture;
        GameSnake.SB_Body_State.transform = sfTransform_Identity;
    }
    else
    {
        GameSnake.SB_Head_Shader = NULL; // prevent dangling pointer
        GameSnake.SB_Body_Shader = NULL; // prevent dangling pointer
    }

    // Init Snake Blocks
    GameSnake.SB_Head.x = Level.TL_Start_Position.x;
    GameSnake.SB_Head.y = Level.TL_Start_Position.y;
    GameSnake.SB_Head.w = sfTexture_getSize(GameSnake.SB_Head_Texture).x;
    GameSnake.SB_Head.h = sfTexture_getSize(GameSnake.SB_Head_Texture).y;

    GameSnake.SB_Body = malloc(SNAKE_START_BLOCKS * sizeof(Block));

    if(GameSnake.SB_Body == NULL)
    {
        printf("Game Subsystem Fehler 'GameSnake': Kann Speicher fuer Blocks nicht laden!\n");
        return false;
    }

    size_t i;
    for(i = 0; i<SNAKE_START_BLOCKS; i++)
    {
        GameSnake.SB_Body[i].w = sfTexture_getSize(GameSnake.SB_Body_Texture).x;
        GameSnake.SB_Body[i].h = sfTexture_getSize(GameSnake.SB_Body_Texture).y;
        GameSnake.SB_Body[i].x = GameSnake.SB_Head.x - (((float)i*(float)GameSnake.SB_Body[i].w/Level.TL_X_Size)+(float)GameSnake.SB_Body[i].w/Level.TL_X_Size);
        GameSnake.SB_Body[i].y = GameSnake.SB_Head.y;
    }

    GameSnake.SB_Body_Elements = SNAKE_START_BLOCKS;

    // Setup general things
    GameSnake.S_Health = 100;
    GameSnake.S_Score = 0;
    GameSnake.S_Speed = Level.MD_Details.Start_Speed;
    GameSnake.S_DefaultSpeed = Level.MD_Details.Default_Speed;
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

    // Set Texture and Position
    sfSprite_setPosition(GameSnake.SB_Head_Sprite, CSFMLSnakeConvertIndexToVector(GameSnake.SB_Head));
    sfSprite_setTexture(GameSnake.SB_Head_Sprite, GameSnake.SB_Head_Texture, sfTrue);

    // Render it
    if(shader_enabled)
    {
        sfShader_setIntUniform(GameSnake.SB_Head_Shader, "Head_Rotate_Angle", GameSnake.S_Rotate);
        sfShader_setIntUniform(GameSnake.SB_Head_Shader, "Head_Health", GameSnake.S_Health);
        sfShader_setBoolUniform(GameSnake.SB_Head_Shader, "Head_Death", GameSnake.S_Is_Dead);
        sfRenderWindow_drawSprite(screen, GameSnake.SB_Head_Sprite, &GameSnake.SB_Head_State);
    }
    else
    {
        sfRenderWindow_drawSprite(screen, GameSnake.SB_Head_Sprite, NULL);
    }


    // Render Body
    size_t i;
    for(i = 0; i<GameSnake.SB_Body_Elements; i++)
    {
        // Set Texture and Position
        sfSprite_setPosition(GameSnake.SB_Body_Sprite, CSFMLSnakeConvertIndexToVector(GameSnake.SB_Body[i]));
        sfSprite_setTexture(GameSnake.SB_Body_Sprite, GameSnake.SB_Body_Texture, sfTrue);

        // Render it
        if(shader_enabled)
        {
            sfShader_setIntUniform(GameSnake.SB_Body_Shader, "Body_Health", GameSnake.S_Health);
            sfShader_setBoolUniform(GameSnake.SB_Body_Shader, "Body_Death", GameSnake.S_Is_Dead);
            sfRenderWindow_drawSprite(screen, GameSnake.SB_Body_Sprite, &GameSnake.SB_Body_State);
        }
        else
        {
            sfRenderWindow_drawSprite(screen, GameSnake.SB_Body_Sprite, NULL);
        }
    }

    return true;
}

// Quit Snake
void CSFMLQuitSnake()
{
    // Clear SFML Stuff
    sfTexture_destroy(GameSnake.SB_Head_Texture);
    sfTexture_destroy(GameSnake.SB_Body_Texture);
    sfSprite_destroy(GameSnake.SB_Head_Sprite);
    sfSprite_destroy(GameSnake.SB_Body_Sprite);
    GameSnake.SB_Body_Sprite = NULL;
    GameSnake.SB_Head_Sprite = NULL;
    GameSnake.SB_Head_Texture = NULL;
    GameSnake.SB_Body_Texture = NULL;

    // Clear Shader Stuff
    if(shader_enabled)
    {
        sfShader_destroy(GameSnake.SB_Head_Shader);
        sfShader_destroy(GameSnake.SB_Body_Shader);
        GameSnake.SB_Head_Shader = NULL;
        GameSnake.SB_Body_Shader = NULL;
    }

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
        GameSnake.SB_Head.y -= (float)GameSnake.SB_Head.h/Level.TL_Y_Size;
    }
    else if(dir == DOWN)
    {
        GameSnake.SB_Head.y += (float)GameSnake.SB_Head.h/Level.TL_Y_Size;
    }
    else if(dir == LEFT)
    {
        GameSnake.SB_Head.x -= (float)GameSnake.SB_Head.w/Level.TL_X_Size;
    }
    else if(dir == RIGHT)
    {
        GameSnake.SB_Head.x += (float)GameSnake.SB_Head.w/Level.TL_X_Size;
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
    for(i = GameSnake.SB_Body_Elements-1; i>0; i--)
    {
        GameSnake.SB_Body[i].x = GameSnake.SB_Body[i-1].x;
        GameSnake.SB_Body[i].y = GameSnake.SB_Body[i-1].y;
        GameSnake.SB_Body[i].w = GameSnake.SB_Body[i-1].w;
        GameSnake.SB_Body[i].h = GameSnake.SB_Body[i-1].h;
    }

    // Set First Element to last Head Block Element
    GameSnake.SB_Body[0].x = GameSnake.SB_Head.x;
    GameSnake.SB_Body[0].y = GameSnake.SB_Head.y;

    return true;
}

// Lets grow the fucking Snake..
bool CSFMLGrowSnake()
{
    GameSnake.SB_Body = realloc(GameSnake.SB_Body, (GameSnake.SB_Body_Elements+1) * sizeof(Block));

    if(GameSnake.SB_Body == NULL)
    {
        printf("Game Subsystem Fehler 'GameSnake': Kann keinen zusaetzlichen Speicher fuer die Snake anfordern!\n");
        return false;
    }
    else
    {
        float fBody_X = 0.0f, fBody_Y = 0.0f;
        bool Hit = false;
        bool Ignore_Hit = false;
        int p;
        int loop_counter = 0;
        GameSnake.SB_Body[GameSnake.SB_Body_Elements].w = GameSnake.SB_Body[0].w;
        GameSnake.SB_Body[GameSnake.SB_Body_Elements].h = GameSnake.SB_Body[0].h;

        // Check Direction and Setup the new Block
        // And Check if the New Blocks maybe in Tilemap Border or Item
        if(GameSnake.S_Actual_Direction == UP)
        {
            fBody_X = GameSnake.SB_Body[GameSnake.SB_Body_Elements-1].x;
            fBody_Y = GameSnake.SB_Body[GameSnake.SB_Body_Elements-1].y + (float)GameSnake.SB_Body[GameSnake.SB_Body_Elements-1].h/Level.TL_Y_Size;
        }
        else if(GameSnake.S_Actual_Direction == DOWN)
        {
            fBody_X = GameSnake.SB_Body[GameSnake.SB_Body_Elements-1].x;
            fBody_Y = GameSnake.SB_Body[GameSnake.SB_Body_Elements-1].y - (float)GameSnake.SB_Body[GameSnake.SB_Body_Elements-1].h/Level.TL_Y_Size;
        }
        else if(GameSnake.S_Actual_Direction == RIGHT)
        {
            fBody_X = GameSnake.SB_Body[GameSnake.SB_Body_Elements-1].x - (float)GameSnake.SB_Body[GameSnake.SB_Body_Elements-1].w/Level.TL_X_Size;
            fBody_Y = GameSnake.SB_Body[GameSnake.SB_Body_Elements-1].y;
        }
        else if(GameSnake.S_Actual_Direction == LEFT)
        {
            fBody_X = GameSnake.SB_Body[GameSnake.SB_Body_Elements-1].x + (float)GameSnake.SB_Body[GameSnake.SB_Body_Elements-1].w/Level.TL_X_Size;
            fBody_Y = GameSnake.SB_Body[GameSnake.SB_Body_Elements-1].y;
        }

        // Check for Spawning Block into Tilemap borders
        do {
            // Get Indices
            size_t Body_X = floor(fBody_X);
            size_t Body_Y = floor(fBody_Y);

            // Reset Hit
            Hit = false;

            // Check for Tilemap Collide
            for(p = 0; p<3; p++)
                if(Level.TL_Map[Body_Y][Body_X] == Level.MD_Details.DMG[p].Type)
                    Hit = true;

            // When Hit is True, move Coordinates and Check again
            if(Hit)
            {
                // Replace Block Left or Right -> Depend on Loops Counter
                if(GameSnake.S_Actual_Direction == UP)
                {
                    // Set new Position Right instead of Down
                    if(loop_counter == 0)
                    {
                        fBody_X += (float)GameSnake.SB_Body[0].w/Level.TL_X_Size; // Right
                        fBody_Y -= (float)GameSnake.SB_Body[0].h/Level.TL_Y_Size; // Reset Y
                    }
                    // Second Try Left instead of Down, no need for Reset Y
                    else if(loop_counter == 1)
                    {
                        fBody_X -= 2 * ((float)GameSnake.SB_Body[0].w/Level.TL_X_Size); // Left
                    }
                    // Third Try?, so the Block must Spawn in the Border! No reason to set Snake dead or something
                    else
                    {
                        printf("Game Subsystem Warning 'GameSnake': Snake Block wird in Border platziert! Keine andere Moeglichkeit!\n");

                        // Set Origin!
                        fBody_X += (float)GameSnake.SB_Body[0].w/Level.TL_X_Size;
                        fBody_Y += (float)GameSnake.SB_Body[0].h/Level.TL_Y_Size;

                        // Activate Ignore Hit
                        Ignore_Hit = true;
                    }
                }
                // Replace Block Left or Right -> Depend on Loops Counter
                else if(GameSnake.S_Actual_Direction == DOWN)
                {
                    // Set new Position Right instead of UP
                    if(loop_counter == 0)
                    {
                        fBody_X += (float)GameSnake.SB_Body[0].w/Level.TL_X_Size; // Right
                        fBody_Y += (float)GameSnake.SB_Body[0].h/Level.TL_Y_Size; // Reset Y
                    }
                    // Second Try Left instead of Down, no need for Reset Y
                    else if(loop_counter == 1)
                    {
                        fBody_X -= 2 * ((float)GameSnake.SB_Body[0].w/Level.TL_X_Size); // Left
                    }
                    // Third Try?, so the Block must Spawn in the Border! No reason to set Snake dead or something
                    else
                    {
                        printf("Game Subsystem Warning 'GameSnake': Snake Block wird in Border platziert! Keine andere Moeglichkeit!\n");

                        // Set Origin!
                        fBody_X += (float)GameSnake.SB_Body[0].w/Level.TL_X_Size;
                        fBody_Y -= (float)GameSnake.SB_Body[0].h/Level.TL_Y_Size;

                        // Activate Ignore Hit
                        Ignore_Hit = true;
                    }
                }
                // Replace Block UP or DOWN -> Depend on Loops Counter
                else if(GameSnake.S_Actual_Direction == LEFT)
                {
                    // Set new Position UP instead of LEFT
                    if(loop_counter == 0)
                    {
                        fBody_X -= (float)GameSnake.SB_Body[0].w/Level.TL_X_Size; // Reset X
                        fBody_Y += (float)GameSnake.SB_Body[0].h/Level.TL_Y_Size; // New Y
                    }
                    // Second Try Left instead of Down, no need for Reset Y
                    else if(loop_counter == 1)
                    {
                        fBody_Y -= 2 * ((float)GameSnake.SB_Body[0].h/Level.TL_Y_Size); // New Y
                    }
                    // Third Try?, so the Block must Spawn in the Border! No reason to set Snake dead or something
                    else
                    {
                        printf("Game Subsystem Warning 'GameSnake': Snake Block wird in Border platziert! Keine andere Moeglichkeit!\n");

                        // Set Origin!
                        fBody_X += (float)GameSnake.SB_Body[0].w/Level.TL_X_Size;
                        fBody_Y += (float)GameSnake.SB_Body[0].h/Level.TL_Y_Size;

                        // Activate Ignore Hit
                        Ignore_Hit = true;
                    }
                }
                // Replace Block UP or DOWN -> Depend on Loops Counter
                else if(GameSnake.S_Actual_Direction == RIGHT)
                {
                    // Set new Position UP instead of LEFT
                    if(loop_counter == 0)
                    {
                        fBody_X += (float)GameSnake.SB_Body[0].w/Level.TL_X_Size; // Reset X
                        fBody_Y += (float)GameSnake.SB_Body[0].h/Level.TL_Y_Size; // New Y
                    }
                    // Second Try Down instead of UP, no need for Reset Y
                    else if(loop_counter == 1)
                    {
                        fBody_Y -= 2 * ((float)GameSnake.SB_Body[0].h/Level.TL_Y_Size); // New Y
                    }
                    // Third Try?, so the Block must Spawn in the Border! No reason to set Snake dead or something
                    else
                    {
                        printf("Game Subsystem Warning 'GameSnake': Snake Block wird in Border platziert! Keine andere Moeglichkeit!\n");

                        // Set Origin!
                        fBody_X -= (float)GameSnake.SB_Body[0].w/Level.TL_X_Size;
                        fBody_Y += (float)GameSnake.SB_Body[0].h/Level.TL_Y_Size;

                        // Activate Ignore Hit
                        Ignore_Hit = true;
                    }
                }

                loop_counter++;

            }

        } while(Hit && !Ignore_Hit); // If no Hit is detected move

        // Move;
        GameSnake.SB_Body[GameSnake.SB_Body_Elements].x = fBody_X;
        GameSnake.SB_Body[GameSnake.SB_Body_Elements].y = fBody_Y;

    }

    GameSnake.SB_Body_Elements++;

    return true;
}

// Set Snake Light
// Todo Fix the Light, because Smoother Movement, Light moves bs
bool CSFMLSetSnakeLight()
{
    if(!GameSnake.S_Is_Init)
        return false;

    // Licht setzen

    float Light_X = 0.0f, Light_Y = 0.0f, Cone_X = 0.0f, Cone_Y = 0.0f;
    if(GameSnake.S_Rotate == 0)
    {
        Light_X = (Level.TL_Start_Position.x * Level.TL_X_Size) + (GameSnake.SB_Head.w/2) + Level.BG_Screenposition.x;
        Light_Y = (Level.TL_Start_Position.y * Level.TL_Y_Size) + (GameSnake.SB_Head.h/2) + Level.BG_Screenposition.y;
        Cone_Y = Light_Y;
        Cone_X = Light_X + GameSnake.S_LightDistance;
    }
    else if(GameSnake.S_Rotate == 180)
    {
        Light_X = (Level.TL_Start_Position.x * Level.TL_X_Size) + (GameSnake.SB_Head.w/2) + Level.BG_Screenposition.x;
        Light_Y = (Level.TL_Start_Position.y * Level.TL_Y_Size) + (GameSnake.SB_Head.h/2) + Level.BG_Screenposition.y;
        Cone_Y = Light_Y;
        Cone_X = Light_X - GameSnake.S_LightDistance;
    }
    else if(GameSnake.S_Rotate == 90)
    {
        Light_X = (Level.TL_Start_Position.x * Level.TL_X_Size) + (GameSnake.SB_Head.w/2) + Level.BG_Screenposition.x;
        Light_Y = (Level.TL_Start_Position.y * Level.TL_Y_Size) + (GameSnake.SB_Head.h/2) + Level.BG_Screenposition.y;
        Cone_Y = Light_Y - GameSnake.S_LightDistance;
        Cone_X = Light_X;
    }
    else if(GameSnake.S_Rotate == 270)
    {
        Light_X = (Level.TL_Start_Position.x * Level.TL_X_Size) + (GameSnake.SB_Head.w/2) + Level.BG_Screenposition.x;
        Light_Y = (Level.TL_Start_Position.y * Level.TL_Y_Size) + (GameSnake.SB_Head.h/2) + Level.BG_Screenposition.y;
        Cone_Y = Light_Y + GameSnake.S_LightDistance;
        Cone_X = Light_X;
    }

    CSFMLSetLight(Light_X, Light_Y, Cone_X, Cone_Y, 0, true, GameSnake.S_Light_FBM, 20, 1, GameSnake.S_Light_OnOff);


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
    int p;

    if(dir == UP)
    {
        fNew_Y -= (float)GameSnake.SB_Head.h/Level.TL_Y_Size;
    }
    else if(dir == DOWN)
    {
        fNew_Y += (float)GameSnake.SB_Head.h/Level.TL_Y_Size;
    }
    else if(dir == LEFT)
    {
        fNew_X -= (float)GameSnake.SB_Head.w/Level.TL_X_Size;
    }
    else if(dir == RIGHT)
    {
        fNew_X += (float)GameSnake.SB_Head.w/Level.TL_X_Size;
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
    for(p = 0; p<3; p++)
    {
        if(Level.TL_Map[New_Y][New_X] == Level.MD_Details.DMG[p].Type)
        {
            // Do DMG
            if(!GameSnake.S_GODMODE)
                GameSnake.S_Health -= Level.MD_Details.DMG[p].Damage;

            // Change Direction
            Snake_Hit = true;
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
                for(p = 0; p<3; p++)
                    if(Level.TL_Map[New_Y][New_X-1] == Level.MD_Details.DMG[p].Type)
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
                for(p = 0; p<3; p++)
                    if(Level.TL_Map[New_Y][New_X+1] == Level.MD_Details.DMG[p].Type)
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
                for(p = 0; p<3; p++)
                    if(Level.TL_Map[New_Y+1][New_X] == Level.MD_Details.DMG[p].Type)
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
                for(p = 0; p<3; p++)
                    if(Level.TL_Map[New_Y-1][New_X] == Level.MD_Details.DMG[p].Type)
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
    int p;

    if(dir == UP)
    {
        fNew_Y -= (float)GameSnake.SB_Head.h/Level.TL_Y_Size;
    }
    else if(dir == DOWN)
    {
        fNew_Y += (float)GameSnake.SB_Head.h/Level.TL_Y_Size;
    }
    else if(dir == LEFT)
    {
        fNew_X -= (float)GameSnake.SB_Head.w/Level.TL_X_Size;
    }
    else if(dir == RIGHT)
    {
        fNew_X += (float)GameSnake.SB_Head.w/Level.TL_X_Size;
    }

    // Iterate over all Body Elements and Check for Collision
    size_t i;
    bool Hit = false;
    for(i = 0; i<GameSnake.SB_Body_Elements; i++)
    {
        // Hit on Body
        if(CompareFloats(GameSnake.SB_Body[i].x, fNew_X) && CompareFloats(GameSnake.SB_Body[i].y, fNew_Y))
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
                for(p = 0; p<3; p++)
                    if(Level.TL_Map[New_Y][New_X-1] == Level.MD_Details.DMG[p].Type)
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
                for(p = 0; p<3; p++)
                    if(Level.TL_Map[New_Y][New_X+1] == Level.MD_Details.DMG[p].Type)
                        Right = false;
            }
            else
            {
                Right = false;
            }

            // Check for Another Self-Hit
            float Left_X = fNew_X - ((float)GameSnake.SB_Head.w/Level.TL_X_Size); // Minus Movement for Left Movement
            float Right_X = fNew_X + ((float)GameSnake.SB_Head.w/Level.TL_X_Size); // Plus Movement for Right Movement

            // Restore Old Values
            float Move_Y = fNew_Y;
            if(dir == UP)
                Move_Y -= (float)GameSnake.SB_Head.h/Level.TL_Y_Size;
            if(dir == DOWN)
                Move_Y += (float)GameSnake.SB_Head.h/Level.TL_Y_Size;

            for(i = 0; i<GameSnake.SB_Body_Elements; i++)
            {
                // Another Self Hit on Left Direction
                if(CompareFloats(GameSnake.SB_Body[i].x, Left_X) && CompareFloats(GameSnake.SB_Body[i].y, Move_Y))
                    Left = false;

                // Another Self Hit on Right Direction
                if(CompareFloats(GameSnake.SB_Body[i].x, Right_X) && CompareFloats(GameSnake.SB_Body[i].y, Move_Y))
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
                for(p = 0; p<3; p++)
                    if(Level.TL_Map[New_Y+1][New_X] == Level.MD_Details.DMG[p].Type)
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
                for(p = 0; p<3; p++)
                    if(Level.TL_Map[New_Y-1][New_X] == Level.MD_Details.DMG[p].Type)
                        Up = false;
            }
            else
            {
                Up = false;
            }

            // Check for Another Self-Hit
            float Up_Y = fNew_Y - ((float)GameSnake.SB_Head.h/Level.TL_Y_Size); // Minus Movement for Left Movement
            float Down_Y = fNew_Y + ((float)GameSnake.SB_Head.h/Level.TL_Y_Size); // Plus Movement for Right Movement

            // Restore Old Values
            float Move_X = fNew_X;
            if(dir == LEFT)
                Move_X -= (float)GameSnake.SB_Head.w/Level.TL_X_Size;
            if(dir == RIGHT)
                Move_X += (float)GameSnake.SB_Head.w/Level.TL_X_Size;

            for(i = 0; i<GameSnake.SB_Body_Elements; i++)
            {
                // Another Self Hit on Up Direction
                if(CompareFloats(GameSnake.SB_Body[i].x, Move_X) && CompareFloats(GameSnake.SB_Body[i].y, Up_Y))
                    Up = false;

                // Another Self Hit on Down Direction
                if(CompareFloats(GameSnake.SB_Body[i].x, Move_X) && CompareFloats(GameSnake.SB_Body[i].y, Down_Y))
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

    // Check TileMap Collide, Self Collide, if Noclip On, Ignore both
    // Its a bit Tricky, if first equation true, the if skips next elements, so they will not execute :)
    if(GameSnake.S_NOCLIP || (CSFMLCheckTileMapCollision() && CSFMLCheckSelfCollision()))
    {
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
                GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
            }
            else
            {
                // Move
                CSFMLPushSnakeBlock(LEFT);
                GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
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
                GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
            }
            else
            {
                // Move
                CSFMLPushSnakeBlock(RIGHT);
                GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
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
                GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
            }
            else
            {
                // Move
                CSFMLPushSnakeBlock(DOWN);
                GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
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
                GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
            }
            else
            {
                // Move
                CSFMLPushSnakeBlock(UP);
                GameSnake.S_Prev_Direction = GameSnake.S_Actual_Direction;
            }
        }
    }

    return true;
}
