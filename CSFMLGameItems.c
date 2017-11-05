#include "CSFMLGameItems.h"

// Init all Items
bool CSFMLInitItems()
{
    // Setup General Things
    DIR *dir;
    struct dirent *entrys;
    GameItem.GI_Items_Count = 0;

    // Get Amount of Files
    // Open Directory
    if((dir = opendir("Bilder/Game/ItemData")) == NULL)
    {
        printf("Game Subsystem Fehler 'GameItems': Keine Item Files vorhanden!\n");
        return false;
    }
    // Read Files
    while((entrys = readdir(dir)) != NULL)
    {
        // Ignore File Name "." and ".."
        if(strcmp(entrys->d_name, ".") && strcmp(entrys->d_name, ".."))
        {
            GameItem.GI_Items_Count++;
        }
    }

    // Close Dir
    closedir(dir);
    dir = NULL;

    if(GameItem.GI_Items_Count == 0)
    {
        printf("Game Subsystem Fehler 'GameItems': Keine Item Dateien gefunden!\n");
        return false;
    }

    // Set Memory
    GameItem.GI_Items = malloc(GameItem.GI_Items_Count*sizeof(Item));
    GameItem.GI_Shaders = malloc(GameItem.GI_Items_Count*sizeof(sfShader*));
    GameItem.GI_Textures = malloc(GameItem.GI_Items_Count*sizeof(sfTexture*));
    GameItem.GI_States = malloc(GameItem.GI_Items_Count*sizeof(sfRenderStates));
    GameItem.GI_Blocks = malloc(GameItem.GI_Items_Count*sizeof(iBlock));
    GameItem.GI_Placed = malloc(GameItem.GI_Items_Count*sizeof(bool));
    GameItem.GI_Coordinates_Setted = malloc(GameItem.GI_Items_Count*sizeof(bool));

    GameItem.GI_Sprite = sfSprite_create();

    if(GameItem.GI_Items == NULL || GameItem.GI_Shaders == NULL || GameItem.GI_Textures == NULL || GameItem.GI_States == NULL || GameItem.GI_Blocks == NULL || GameItem.GI_Placed == NULL || GameItem.GI_Coordinates_Setted == NULL || GameItem.GI_Sprite == NULL)
    {
        printf("Game Subsystem Fehler 'GameItems': Kann Speicher nicht reservieren!\n");
        return false;
    }

    // Open Directory
    if((dir = opendir("Bilder/Game/ItemData")) == NULL)
    {
        printf("Game Subsystem Fehler 'GameItems': Keine Item Files vorhanden!\n");
        return false;
    }

    // Read Files
    size_t counter = 0;
    while((entrys = readdir(dir)) != NULL)
    {
        // Ignore File Name "." and ".."
        if(strcmp(entrys->d_name, ".") && strcmp(entrys->d_name, ".."))
        {
            // Open Actual File
            char filepath[50] = {0};
            strcat(filepath, "Bilder/Game/ItemData/");
            strcat(filepath, entrys->d_name);
            FILE *actual = fopen(filepath, "rb");
            if(actual == NULL)
            {
                printf("Game Subsystem Fehler 'GameItems': Kann Datei nicht oeffnen!\n");
                return false;
            }

            // Read Actual File and put Item to Structure
            if(fread(&GameItem.GI_Items[counter], sizeof(Item), 1, actual) != 1)
            {
                printf("Game Subsystem Fehler 'GameItems': Kann Datei nicht lesen!\n");
                return false;
            }

            // Setup Shader
            GameItem.GI_Shaders[counter] = sfShader_createFromFile(NULL, NULL, GameItem.GI_Items[counter].I_ShaderPath);
            if(GameItem.GI_Shaders[counter] == NULL)
            {
                printf("Game Subsystem Fehler 'GameItems': Kann Shader nicht laden!\n");
                return false;
            }

            // Setup Texture
            GameItem.GI_Textures[counter] = sfTexture_createFromFile(GameItem.GI_Items[counter].I_TilePath, NULL);
            if(GameItem.GI_Textures[counter] == NULL)
            {
                printf("Game Subsystem Fehler 'GameItems': Kann Textur nicht laden!\n");
                return false;
            }

            // Setup Shader Values
            sfShader_setTextureUniform(GameItem.GI_Shaders[counter], "Texture", GameItem.GI_Textures[counter]);
            sfShader_setFloatUniform(GameItem.GI_Shaders[counter], "Time", GameClock.GC_Time);

            // Setup Block Values
            GameItem.GI_Blocks[counter].x = 0;
            GameItem.GI_Blocks[counter].y = 0;
            GameItem.GI_Blocks[counter].w = (float)sfTexture_getSize(GameItem.GI_Textures[counter]).x;
            GameItem.GI_Blocks[counter].h = (float)sfTexture_getSize(GameItem.GI_Textures[counter]).y;

            // Setup RenderState
            GameItem.GI_States[counter].blendMode = sfBlendAlpha;
            GameItem.GI_States[counter].shader = GameItem.GI_Shaders[counter];
            GameItem.GI_States[counter].texture = GameItem.GI_Textures[counter];
            GameItem.GI_States[counter].transform = sfTransform_Identity;

            // All Items all first not Placed and Coordinates are not setted
            GameItem.GI_Placed[counter] = false;
            GameItem.GI_Coordinates_Setted[counter] = false;

            // Update Item Count
            counter++;

            // Close Actual File
            fclose(actual);
            actual = NULL;
        }
    }

    // Close Dir
    closedir(dir);
    dir = NULL;

    // Set Memory to Allowed Items here
    GameItem.GI_AllowedItems = malloc(6 * sizeof(AllowedItems));

    if(GameItem.GI_AllowedItems == NULL)
    {
        printf("Game Subsystem Fehler 'GameItems': Kann Speicher fuer erlaubte Items nicht anlegen!\n");
        return false;
    }

    size_t i;
    for(i = 0; i<6; i++)
    {
        GameItem.GI_AllowedItems[i].Type = Level.MD_Details.ItemsAllowed[i].Type;
        GameItem.GI_AllowedItems[i].Actual_Count = 0;
        GameItem.GI_AllowedItems[i].Max_Count = Level.MD_Details.ItemsAllowed[i].Count;
    }

    // Setup general things
    GameItem.GI_Is_Init = true;
    GameItem.GI_AllowedItems_Count = 6;

    // Init Random Number Generator
    time_t t;
    srand((unsigned) time(&t));

    return true;
}

// Clear Item Stuff
void CSFMLQuitItems()
{
    // Clear SFML Stuff /// From Here
    size_t i;
    for(i = 0; i<GameItem.GI_Items_Count; i++)
    {
        sfShader_destroy(GameItem.GI_Shaders[i]);
        GameItem.GI_Shaders[i] = NULL;

        sfTexture_destroy(GameItem.GI_Textures[i]);
        GameItem.GI_Textures[i] = NULL;
    }

    sfSprite_destroy(GameItem.GI_Sprite);
    GameItem.GI_Sprite = NULL;

    free(GameItem.GI_Shaders);
    GameItem.GI_Shaders = NULL;

    free(GameItem.GI_Textures);
    GameItem.GI_Textures = NULL;

    free(GameItem.GI_AllowedItems);
    GameItem.GI_AllowedItems = NULL;

    free(GameItem.GI_Blocks);
    GameItem.GI_Blocks = NULL;

    free(GameItem.GI_States);
    GameItem.GI_States = NULL;

    free(GameItem.GI_Items);
    GameItem.GI_Items = NULL;

    free(GameItem.GI_Coordinates_Setted);
    GameItem.GI_Coordinates_Setted = NULL;

    free(GameItem.GI_Placed);
    GameItem.GI_Placed = NULL;

    GameItem.GI_Is_Init = false;
}

// Increase Item Count
// Return True for Place and False for dont Place
// This Doesnt work really now.,,
bool CSFMLIncreaseItemCount(size_t ItemIndex)
{
    // Get Current ItemType
    ItemType actualtype = CSFMLGetItemType(GameItem.GI_Items[ItemIndex]);

    // Find ItemType in Allowed Items
    size_t i;
    bool Result = false;
    for(i = 0; i<GameItem.GI_AllowedItems_Count; i++)
    {
        if(GameItem.GI_AllowedItems[i].Type == actualtype)
        {
            if(GameItem.GI_AllowedItems[i].Actual_Count < GameItem.GI_AllowedItems[i].Max_Count)
            {
                GameItem.GI_AllowedItems[i].Actual_Count++;
                Result = true;
            }
            else
            {
                Result = false;
            }
        }
    }

    return Result;
}

void CSFMLDecreaseItemCount(size_t ItemIndex)
{
    // Get Current ItemType
    ItemType actualtype = CSFMLGetItemType(GameItem.GI_Items[ItemIndex]);

    // Find ItemType in Allowed Items
    size_t i;
    for(i = 0; i<GameItem.GI_AllowedItems_Count; i++)
    {
        if(GameItem.GI_AllowedItems[i].Type == actualtype)
        {
            if(GameItem.GI_AllowedItems[i].Actual_Count > 0)
            {
                GameItem.GI_AllowedItems[i].Actual_Count--;
            }
        }
    }
}

// Set Placed Variable
void CSFMLSetPlaceItems()
{
    // Iterate over all Items
    // And Just Place an Item per Type
    size_t i;
    for(i = 0; i<GameItem.GI_Items_Count; i++)
    {
        // Just proceed with unplaced and >0% Chance Items
        if(!GameItem.GI_Placed[i] && (GameItem.GI_Items[i].I_Chance > 0))
        {
            // Need it to Place ? Calculate it, with the Chance
            int random = rand() % 100 + 1;

            if((random > 0) && (random <= GameItem.GI_Items[i].I_Chance) && (CSFMLIncreaseItemCount(i)))
            {
                GameItem.GI_Placed[i] = true;
            }

        }
    }
}

// Place Variable on Map!
void CSFMLSetCoordinatesItems()
{
    // Generate TileMap Corrected Position - Normalized Index Position
    bool Hit = false;
    size_t i;
    size_t random_x = rand() % MAX_TILES_X;
    size_t random_y = rand() % MAX_TILES_Y;

    // Check Snake Collide Head !OK
    if(random_x == GameSnake.SB_Head.x && random_y == GameSnake.SB_Head.y)
        Hit = true;

    // Check Snake Collide Body !OK
    for(i = 0; i<GameSnake.SB_Body_Elements; i++)
    {
        if(GameSnake.SB_Body[i].x == random_x && GameSnake.SB_Body[i].y == random_y)
            Hit = true;
    }

    // Check Item Collide -> Fail?!
    for(i = 0; i<GameItem.GI_Items_Count; i++)
    {
        if(GameItem.GI_Coordinates_Setted[i])
        {
            size_t G_X = floor(GameItem.GI_Blocks[i].x);
            size_t G_Y = floor(GameItem.GI_Blocks[i].y);
            if(G_X == random_x && G_Y == random_y)
            {
                Hit = true;
            }
        }
    }

    // Iterate over all Items
    for(i = 0; i<GameItem.GI_Items_Count && !Hit; i++)
    {
        // Just proceed with Items, that are placed and Coordinates not setted!
        if(GameItem.GI_Placed[i] && !GameItem.GI_Coordinates_Setted[i])
        {
            // Position must be in Tilemap with 0 and doesnt Render in Snake Elements
            if(Level.TL_Map[random_y][random_x] == 0)
            {
                GameItem.GI_Blocks[i].x = random_x;
                GameItem.GI_Blocks[i].y = random_y;
                GameItem.GI_Coordinates_Setted[i] = true;
            }
        }
    }
}

// Convert Block to Screen Coordinates
sfVector2f CSFMLItemConvertIndexToVector(iBlock blk)
{
    sfVector2f Ret_Value;

    Ret_Value.x = (blk.x * Level.TL_X_Size) + Level.BG_Screenposition.x;
    Ret_Value.y = (blk.y * Level.TL_Y_Size) + Level.BG_Screenposition.y;

    return Ret_Value;
}

// Get Current Item Type
ItemType CSFMLGetItemType(Item Current)
{
    if(strstr(Current.I_Name, "Food"))
    {
        return IT_FOOD;
    }
    else if(strstr(Current.I_Name, "Speed"))
    {
        return IT_SPEED;
    }
    else if(strstr(Current.I_Name, "First"))
    {
        return IT_HEALTH;
    }
    else if(strstr(Current.I_Name, "God"))
    {
        return IT_GOD;
    }
    else if(strstr(Current.I_Name, "Noclip"))
    {
        return IT_NOCLIP;
    }
    else
    {
        return IT_CUSTOM;
    }
}

// Render Items
void CSFMLRenderItems()
{
    // Update Variables
    CSFMLSetPlaceItems();
    CSFMLSetCoordinatesItems();

    // Render it now
    size_t i;
    for(i = 0; i<GameItem.GI_Items_Count; i++)
    {
        // Just Render Items that are placed and got coordinates
        if(GameItem.GI_Coordinates_Setted[i] && GameItem.GI_Placed[i])
        {
            sfSprite_setTexture(GameItem.GI_Sprite, GameItem.GI_Textures[i], sfTrue);
            sfSprite_setPosition(GameItem.GI_Sprite, CSFMLItemConvertIndexToVector(GameItem.GI_Blocks[i]));
            sfRenderWindow_drawSprite(screen, GameItem.GI_Sprite, &GameItem.GI_States[i]);
        }

    }
}

// Handle now Items
void CSFMLHandleItems()
{
    // First Handle Timer Events;
    if(GameSnake.S_EndItemTime <= GameClock.GC_Time)
        GameSnake.S_Speed = GameSnake.S_DefaultSpeed;

    // Getting Actual Direction
    Direction dir = GameSnake.S_Actual_Direction;

    // Calculate Direction
    float Next_X = GameSnake.SB_Head.x;
    float Next_Y = GameSnake.SB_Head.y;

    if(dir == UP)
    {
        Next_Y -= (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }
    else if(dir == DOWN)
    {
        Next_Y += (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }
    else if(dir == LEFT)
    {
        Next_X -= (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }
    else if(dir == RIGHT)
    {
        Next_X += (float)SNAKE_PICTURE_SIZE/Level.TL_X_Size;
    }

    // Check Collision
    size_t i;
    for(i = 0; i<GameItem.GI_Items_Count; i++)
    {
        // Just for the placed one
        if(GameItem.GI_Placed[i])
        {
            // Collision Detected
            if(Next_X == GameItem.GI_Blocks[i].x && Next_Y == GameItem.GI_Blocks[i].y)
            {
                // Checkup Propertys
                size_t s;
                for(s = 0; s<GameItem.GI_Items[i].I_PropertyCount; s++)
                {
                    // Proceed Effects
                    // Todo: Place a Nicely Splash Text, if a Item is hitted!
                    if(GameItem.GI_Items[i].I_Propertys[s].I_Effect == I_HEALTH)
                    {
                        // General
                        CSFMLDecreaseItemCount(i);
                        GameItem.GI_Placed[i] = false;
                        GameItem.GI_Coordinates_Setted[i] = false;

                        // Set Health
                        if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_INCREASE)
                        {
                            if((GameSnake.S_Health + GameItem.GI_Items[i].I_Propertys[s].I_Amount) < 100)
                                GameSnake.S_Health += GameItem.GI_Items[i].I_Propertys[s].I_Amount;
                            else
                                GameSnake.S_Health = 100;
                        }
                        else if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_DECREASE)
                        {
                            if((GameSnake.S_Health - GameItem.GI_Items[i].I_Propertys[s].I_Amount) > 0)
                                GameSnake.S_Health -= GameItem.GI_Items[i].I_Propertys[s].I_Amount;
                            else
                            {
                                GameSnake.S_Health = 0;
                                GameSnake.S_Is_Dead = true;
                            }

                        }

                    }
                    else if(GameItem.GI_Items[i].I_Propertys[s].I_Effect == I_SPEED)
                    {
                        // General
                        CSFMLDecreaseItemCount(i);
                        GameItem.GI_Placed[i] = false;
                        GameItem.GI_Coordinates_Setted[i] = false;

                        // Set Speed
                        // Seems a bit buggy..
                        if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_INCREASE)
                        {
                            GameSnake.S_EndItemTime += GameClock.GC_Time + GameItem.GI_Items[i].I_Propertys[s].I_Duration;
                            GameSnake.S_Speed += GameItem.GI_Items[i].I_Propertys[s].I_Amount;
                        }
                        else if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_DECREASE)
                        {
                            GameSnake.S_EndItemTime += GameClock.GC_Time + GameItem.GI_Items[i].I_Propertys[s].I_Duration;
                            GameSnake.S_Speed -= GameItem.GI_Items[i].I_Propertys[s].I_Amount;
                        }
                    }
                    else if(GameItem.GI_Items[i].I_Propertys[s].I_Effect == I_SCORE)
                    {
                        // General
                        CSFMLDecreaseItemCount(i);
                        GameItem.GI_Placed[i] = false;
                        GameItem.GI_Coordinates_Setted[i] = false;
                        CSFMLGrowSnake();

                        // Set Score
                        if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_INCREASE)
                        {
                            GameSnake.S_Score += GameItem.GI_Items[i].I_Propertys[s].I_Amount;
                        }
                        else if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_DECREASE)
                        {
                            if((GameSnake.S_Score - GameItem.GI_Items[i].I_Propertys[s].I_Amount) > 0)
                                GameSnake.S_Score -= GameItem.GI_Items[i].I_Propertys[s].I_Amount;
                            else
                            {
                                GameSnake.S_Score = 0;
                            }

                        }
                    }
                    else if(GameItem.GI_Items[i].I_Propertys[s].I_Effect == I_GOD)
                    {
                        // General
                        CSFMLDecreaseItemCount(i);
                        GameItem.GI_Placed[i] = false;
                        GameItem.GI_Coordinates_Setted[i] = false;

                        // Set God Mode
                        GameSnake.S_GODMODE = true;
                    }
                    else if(GameItem.GI_Items[i].I_Propertys[s].I_Effect == I_CLIP)
                    {
                        // General
                        CSFMLDecreaseItemCount(i);
                        GameItem.GI_Placed[i] = false;
                        GameItem.GI_Coordinates_Setted[i] = false;

                        // Set NoClip Mode
                        GameSnake.S_NOCLIP = true;
                    }
                }
            }
        }
    }
}


