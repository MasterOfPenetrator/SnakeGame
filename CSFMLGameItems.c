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
    GameItem.GI_Textures = malloc(GameItem.GI_Items_Count*sizeof(sfTexture*));
    GameItem.GI_Blocks = malloc(GameItem.GI_Items_Count*sizeof(iBlock));
    GameItem.GI_Placed = malloc(GameItem.GI_Items_Count*sizeof(bool));
    GameItem.GI_Coordinates_Setted = malloc(GameItem.GI_Items_Count*sizeof(bool));

    GameItem.GI_Sprite = sfSprite_create();

    if(GameItem.GI_Items == NULL || GameItem.GI_Textures == NULL || GameItem.GI_Blocks == NULL || GameItem.GI_Placed == NULL || GameItem.GI_Coordinates_Setted == NULL || GameItem.GI_Sprite == NULL)
    {
        printf("Game Subsystem Fehler 'GameItems': Kann Speicher nicht reservieren!\n");
        return false;
    }

    // Set Shader if enabled
    if(shader_enabled)
    {
        GameItem.GI_Shaders = malloc(GameItem.GI_Items_Count*sizeof(sfShader*));
        GameItem.GI_States = malloc(GameItem.GI_Items_Count*sizeof(sfRenderStates));

        if(GameItem.GI_Shaders == NULL || GameItem.GI_States == NULL)
        {
            printf("Game Subsystem Fehler 'GameItems': Kann Speicher fuer Shader nicht reservieren!\n");
            return false;
        }
    }
    else
    {
        GameItem.GI_Shaders = NULL; // Prevent dangling pointer
        GameItem.GI_States = NULL; // Prevent dangling pointer
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

            // Setup Texture
            GameItem.GI_Textures[counter] = sfTexture_createFromFile(GameItem.GI_Items[counter].I_TilePath, NULL);
            if(GameItem.GI_Textures[counter] == NULL)
            {
                printf("Game Subsystem Fehler 'GameItems': Kann Textur nicht laden!\n");
                return false;
            }

            // Setup Shader
            if(shader_enabled)
            {
                GameItem.GI_Shaders[counter] = sfShader_createFromFile(NULL, NULL, GameItem.GI_Items[counter].I_ShaderPath);
                if(GameItem.GI_Shaders[counter] == NULL)
                {
                    printf("Game Subsystem Fehler 'GameItems': Kann Shader nicht laden!\n");
                    return false;
                }

                // Setup Shader Values
                sfShader_setTextureUniform(GameItem.GI_Shaders[counter], "Texture", GameItem.GI_Textures[counter]);
                sfShader_setFloatUniform(GameItem.GI_Shaders[counter], "Time", GameClock.GC_Time);
                sfShader_setBoolUniform(GameItem.GI_Shaders[counter], "Desaturate", false);

                // Setup RenderState
                GameItem.GI_States[counter].blendMode = sfBlendAlpha;
                GameItem.GI_States[counter].shader = GameItem.GI_Shaders[counter];
                GameItem.GI_States[counter].texture = GameItem.GI_Textures[counter];
                GameItem.GI_States[counter].transform = sfTransform_Identity;
            }

            // Setup Block Values
            GameItem.GI_Blocks[counter].x = 0;
            GameItem.GI_Blocks[counter].y = 0;
            GameItem.GI_Blocks[counter].w = (float)sfTexture_getSize(GameItem.GI_Textures[counter]).x;
            GameItem.GI_Blocks[counter].h = (float)sfTexture_getSize(GameItem.GI_Textures[counter]).y;


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
    GameItem.GI_ItemTexts = NULL;
    GameItem.GI_ItemTexts_Count = 0;
    GameItem.GI_ItemTexts_Used = 0;

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
        if(shader_enabled)
        {
            sfShader_destroy(GameItem.GI_Shaders[i]);
            GameItem.GI_Shaders[i] = NULL;
        }

        sfTexture_destroy(GameItem.GI_Textures[i]);
        GameItem.GI_Textures[i] = NULL;
    }

    sfSprite_destroy(GameItem.GI_Sprite);
    GameItem.GI_Sprite = NULL;

    free(GameItem.GI_Textures);
    GameItem.GI_Textures = NULL;

    free(GameItem.GI_AllowedItems);
    GameItem.GI_AllowedItems = NULL;

    free(GameItem.GI_Blocks);
    GameItem.GI_Blocks = NULL;

    free(GameItem.GI_Items);
    GameItem.GI_Items = NULL;

    free(GameItem.GI_Coordinates_Setted);
    GameItem.GI_Coordinates_Setted = NULL;

    free(GameItem.GI_Placed);
    GameItem.GI_Placed = NULL;

    if(shader_enabled)
    {
        free(GameItem.GI_Shaders);
        GameItem.GI_Shaders = NULL;

        free(GameItem.GI_States);
        GameItem.GI_States = NULL;
    }

    if(GameItem.GI_ItemTexts_Count > 0)
    {
        for(i = 0; i<GameItem.GI_ItemTexts_Count; i++)
        {
            sfText_destroy(GameItem.GI_ItemTexts[i].Text);
            GameItem.GI_ItemTexts[i].Text = NULL;
        }

        free(GameItem.GI_ItemTexts);
        GameItem.GI_ItemTexts = NULL;
    }

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
                printf("Spawned Item: %s\n", GameItem.GI_Items[i].I_Name);
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
    float Random_X = GetRandomFloatNumber(MAX_TILES_X);
    float Random_Y = GetRandomFloatNumber(MAX_TILES_Y);

    // Check Snake Collide Head !OK
    if(CompareFloats(Random_X, GameSnake.SB_Head.x) && CompareFloats(Random_Y, GameSnake.SB_Head.y))
        Hit = true;

    // Check Snake Collide Body !OK
    // Todo: Prevent for XXX = Float Value, it could be undefined behavior
    for(i = 0; i<GameSnake.SB_Body_Elements; i++)
    {
        if(CompareFloats(Random_X, GameSnake.SB_Body[i].x) && CompareFloats(Random_Y, GameSnake.SB_Body[i].y))
            Hit = true;
    }

    // Check Item Collide -> Semms now OK
    for(i = 0; i<GameItem.GI_Items_Count; i++)
    {
        if(CompareFloats(Random_X, GameItem.GI_Blocks[i].x) && CompareFloats(Random_Y, GameItem.GI_Blocks[i].y))
            Hit = true;
    }

    // Iterate over all Items
    for(i = 0; i<GameItem.GI_Items_Count && !Hit; i++)
    {
        // Just proceed with Items, that are placed and Coordinates not setted!
        if(GameItem.GI_Placed[i] && !GameItem.GI_Coordinates_Setted[i])
        {
            // Position must be in Tilemap with 0 and doesnt Render in Snake Elements
            if(Level.TL_Map[(size_t)floor(Random_Y)][(size_t)floor(Random_X)] == 0)
            {
                GameItem.GI_Blocks[i].x = Random_X;
                GameItem.GI_Blocks[i].y = Random_Y;
                GameItem.GI_Coordinates_Setted[i] = true;
                printf("Item Coordinates: %f - %f\n", Random_X, Random_Y);
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
            // Set Texture & Position
            sfSprite_setTexture(GameItem.GI_Sprite, GameItem.GI_Textures[i], sfTrue);
            sfSprite_setPosition(GameItem.GI_Sprite, CSFMLItemConvertIndexToVector(GameItem.GI_Blocks[i]));

            // Render it
            if(shader_enabled)
                sfRenderWindow_drawSprite(screen, GameItem.GI_Sprite, &GameItem.GI_States[i]);
            else
                sfRenderWindow_drawSprite(screen, GameItem.GI_Sprite, NULL);
        }

    }
}

// Spawn Text
bool CSFMLItemSpawnText(const char *text)
{
    // Allocate Memory
    if(GameItem.GI_ItemTexts_Count == 0)
    {
        GameItem.GI_ItemTexts = malloc(sizeof(ItemSpawnText));

        if(GameItem.GI_ItemTexts == NULL)
        {
            printf("Game Subsystem Fehler 'GameItems': Kann Speicher fuer Spawn Texte nicht reservieren!\n");
            return false;
        }

        GameItem.GI_ItemTexts[0].Text = sfText_create();

        if(GameItem.GI_ItemTexts[0].Text == NULL)
        {
            printf("Game Subsystem Fehler 'GameItems': Kann Text Objekt fuer Spawn Texte erstellen!\n");
            return false;
        }

        sfText_setFont(GameItem.GI_ItemTexts[0].Text, media.normal_font);
        sfText_setCharacterSize(GameItem.GI_ItemTexts[0].Text, 20);
        sfText_setString(GameItem.GI_ItemTexts[0].Text, text);

        sfVector2f Position = {GetRandomFloatNumber(572.0f - sfText_getLocalBounds(GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].Text).width) + Level.BG_Screenposition.x , GetRandomFloatNumber(472.0f - sfText_getLocalBounds(GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].Text).height) + Level.BG_Screenposition.y};
        sfColor Color = {255, 184, 0, 255};

        sfText_setPosition(GameItem.GI_ItemTexts[0].Text, Position);
        sfText_setColor(GameItem.GI_ItemTexts[0].Text, Color);

        GameItem.GI_ItemTexts[0].RenderIt = true;
        GameItem.GI_ItemTexts[0].Alpha = 255.0f;

        GameItem.GI_ItemTexts_Count = 1;
        GameItem.GI_ItemTexts_Used = 1;
    }
    else if(GameItem.GI_ItemTexts_Count > 0)
    {
        // Realloc if need
        if(GameItem.GI_ItemTexts_Count == GameItem.GI_ItemTexts_Used)
        {
            // Increment count
            GameItem.GI_ItemTexts_Count++;
            GameItem.GI_ItemTexts = realloc(GameItem.GI_ItemTexts, GameItem.GI_ItemTexts_Count * sizeof(ItemSpawnText));

            if(GameItem.GI_ItemTexts == NULL)
            {
                printf("Game Subsystem Fehler 'GameItems': Kann keinen zusaetzlichen Speicher fuer Spawn Texte reservieren!\n");
                return false;
            }
        }

        // Append to the next
        GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].Text = sfText_create();

        if(GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].Text == NULL)
        {
           printf("Game Subsystem Fehler 'GameItems': Kann Text Objekt fuer Spawn Texte erstellen!\n");
            return false;
        }

        sfText_setFont(GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].Text, media.normal_font);
        sfText_setCharacterSize(GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].Text, 20);
        sfText_setString(GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].Text, text);

        sfVector2f Position = {GetRandomFloatNumber(572.0f - sfText_getLocalBounds(GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].Text).width) + Level.BG_Screenposition.x , GetRandomFloatNumber(472.0f - sfText_getLocalBounds(GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].Text).height) + Level.BG_Screenposition.y};
        sfColor Color = {255, 184, 0, 255};

        sfText_setPosition(GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].Text, Position);
        sfText_setColor(GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].Text, Color);

        GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].RenderIt = true;
        GameItem.GI_ItemTexts[GameItem.GI_ItemTexts_Used].Alpha = 255.0f;

        // Increment Used
        GameItem.GI_ItemTexts_Used++;
    }
}

// Render Text
void CSFMLRenderItemText()
{
    // Iterate over all Items
    size_t i;
    for(i = 0; i<GameItem.GI_ItemTexts_Count; i++)
    {
        // Render Text
        if(GameItem.GI_ItemTexts[i].RenderIt)
        {
            // Get Actual Color and Scale
            sfColor ActualColor = sfText_getColor(GameItem.GI_ItemTexts[i].Text);
            sfVector2f ActualScale = sfText_getScale(GameItem.GI_ItemTexts[i].Text);

            // Set Fade Out and Scale Out
            GameItem.GI_ItemTexts[i].Alpha -= GameClock.GC_DeltaTime * 1/GameClock.GC_DeltaTime;
            ActualColor.a = (sfUint8) GameItem.GI_ItemTexts[i].Alpha;
            ActualScale.x = (GameItem.GI_ItemTexts[i].Alpha / 255.0f);
            ActualScale.y = (GameItem.GI_ItemTexts[i].Alpha / 255.0f);

            sfText_setColor(GameItem.GI_ItemTexts[i].Text, ActualColor);
            sfText_setScale(GameItem.GI_ItemTexts[i].Text, ActualScale);
            sfRenderWindow_drawText(screen, GameItem.GI_ItemTexts[i].Text, NULL);

            if(GameItem.GI_ItemTexts[i].Alpha < 1.0f)
            {
                GameItem.GI_ItemTexts[i].Alpha = 0.0f;
                GameItem.GI_ItemTexts[i].RenderIt = false;
                sfText_destroy(GameItem.GI_ItemTexts[i].Text);
                GameItem.GI_ItemTexts[i].Text = NULL;
                GameItem.GI_ItemTexts_Used--;
            }
        }
    }
}

// Handle now Items
void CSFMLHandleItems()
{
    // Getting Actual Direction
    Direction dir = GameSnake.S_Actual_Direction;

    // Calculate Direction
    float Next_X = GameSnake.SB_Head.x;
    float Next_Y = GameSnake.SB_Head.y;

    if(dir == UP)
    {
        Next_Y -= (float)GameSnake.SB_Head.h/Level.TL_X_Size;
    }
    else if(dir == DOWN)
    {
        Next_Y += (float)GameSnake.SB_Head.h/Level.TL_X_Size;
    }
    else if(dir == LEFT)
    {
        Next_X -= (float)GameSnake.SB_Head.w/Level.TL_X_Size;
    }
    else if(dir == RIGHT)
    {
        Next_X += (float)GameSnake.SB_Head.w/Level.TL_X_Size;
    }

    // Check Collision
    size_t i;
    for(i = 0; i<GameItem.GI_Items_Count; i++)
    {
        // Just for the placed one
        if(GameItem.GI_Placed[i])
        {
            // Collision Detected
            if(CompareFloats(Next_X, GameItem.GI_Blocks[i].x) && CompareFloats(Next_Y, GameItem.GI_Blocks[i].y))
            {
                // General
                // Delete Hitted Item
                CSFMLDecreaseItemCount(i);
                GameItem.GI_Placed[i] = false;
                GameItem.GI_Coordinates_Setted[i] = false;

                // Spawn the Item Text
                if(strstr(GameItem.GI_Items[i].I_Name, "First Aid"))
                {
                    CSFMLItemSpawnText("First Aid Picked!");
                }
                else if(strstr(GameItem.GI_Items[i].I_Name, "Speed"))
                {
                    CSFMLItemSpawnText("Speed Up Picked!");
                }
                else if(strstr(GameItem.GI_Items[i].I_Name, "Food"))
                {
                    CSFMLItemSpawnText("Food Picked!");
                }
                else if(strstr(GameItem.GI_Items[i].I_Name, "clip"))
                {
                    CSFMLItemSpawnText("Noclip Picked!");
                }
                else if(strstr(GameItem.GI_Items[i].I_Name, "god"))
                {
                    CSFMLItemSpawnText("Godmode Picked!");
                }

                // Make the Item Effect now
                // There could be up to 3 Effects per Item simultanly
                size_t s;
                for(s = 0; s<GameItem.GI_Items[i].I_PropertyCount; s++)
                {
                    // Proceed Effects
                    if(GameItem.GI_Items[i].I_Propertys[s].I_Effect == I_HEALTH)
                    {
                        // Set Health
                        // Timelimited Health Item
                        if(GameItem.GI_Items[i].I_Propertys[s].I_Duration > 0)
                        {
                            if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_INCREASE)
                            {
                                CSFMLAddTimeEvent(HEALTH, GameItem.GI_Items[i].I_Propertys[s].I_Duration, GameItem.GI_Items[i].I_Propertys[s].I_Amount, 0);
                            }
                            else if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_DECREASE)
                            {
                                CSFMLAddTimeEvent(HEALTH, GameItem.GI_Items[i].I_Propertys[s].I_Duration, GameItem.GI_Items[i].I_Propertys[s].I_Amount, 1);
                            }
                        }
                        // Fix Health Item
                        else
                        {
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

                    }
                    else if(GameItem.GI_Items[i].I_Propertys[s].I_Effect == I_SPEED)
                    {
                        // Set Speed
                        // Timelimited Speed Item
                        if(GameItem.GI_Items[i].I_Propertys[s].I_Duration > 0)
                        {
                            if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_INCREASE)
                            {
                                CSFMLAddTimeEvent(SPEED, GameItem.GI_Items[i].I_Propertys[s].I_Duration, GameItem.GI_Items[i].I_Propertys[s].I_Amount, 0);
                            }
                            else if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_DECREASE)
                            {
                                CSFMLAddTimeEvent(SPEED, GameItem.GI_Items[i].I_Propertys[s].I_Duration, GameItem.GI_Items[i].I_Propertys[s].I_Amount, 1);
                            }
                        }
                        // Fixed Speed Item
                        else
                        {
                            if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_INCREASE)
                            {
                                GameSnake.S_Speed += GameItem.GI_Items[i].I_Propertys[s].I_Amount;
                            }
                            else if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_DECREASE)
                            {
                                GameSnake.S_Speed -= GameItem.GI_Items[i].I_Propertys[s].I_Amount;
                            }
                        }
                    }
                    else if(GameItem.GI_Items[i].I_Propertys[s].I_Effect == I_SCORE)
                    {
                        // Grow Snake
                        CSFMLGrowSnake();

                        // Set Score
                        // Timelimited Score Event
                        if(GameItem.GI_Items[i].I_Propertys[s].I_Duration > 0)
                        {
                            if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_INCREASE)
                            {
                                CSFMLAddTimeEvent(SCORE, GameItem.GI_Items[i].I_Propertys[s].I_Duration, GameItem.GI_Items[i].I_Propertys[s].I_Amount, 0);
                            }
                            else if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_DECREASE)
                            {
                                CSFMLAddTimeEvent(SCORE, GameItem.GI_Items[i].I_Propertys[s].I_Duration, GameItem.GI_Items[i].I_Propertys[s].I_Amount, 1);
                            }
                        }
                        // Fixed Score Event
                        else
                        {
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
                    }
                    else if(GameItem.GI_Items[i].I_Propertys[s].I_Effect == I_GOD)
                    {
                        // Set God Mode
                        // Timelimited God Item
                        if(GameItem.GI_Items[i].I_Propertys[s].I_Duration > 0)
                        {
                            if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_INCREASE)
                            {
                                CSFMLAddTimeEvent(GOD, GameItem.GI_Items[i].I_Propertys[s].I_Duration, GameItem.GI_Items[i].I_Propertys[s].I_Amount, 0);
                            }
                            else if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_DECREASE)
                            {
                                CSFMLAddTimeEvent(GOD, GameItem.GI_Items[i].I_Propertys[s].I_Duration, GameItem.GI_Items[i].I_Propertys[s].I_Amount, 1);
                            }
                        }
                        // Fixed God Item
                        else
                        {
                            GameSnake.S_GODMODE = true;
                        }
                    }
                    else if(GameItem.GI_Items[i].I_Propertys[s].I_Effect == I_CLIP)
                    {
                        // Set NoClip Mode
                        // Timelimited Noclip Item
                        if(GameItem.GI_Items[i].I_Propertys[s].I_Duration > 0)
                        {
                            if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_INCREASE)
                            {
                                CSFMLAddTimeEvent(NOCLIP, GameItem.GI_Items[i].I_Propertys[s].I_Duration, GameItem.GI_Items[i].I_Propertys[s].I_Amount, 0);
                            }
                            else if(GameItem.GI_Items[i].I_Propertys[s].I_Direction == I_DECREASE)
                            {
                                CSFMLAddTimeEvent(NOCLIP, GameItem.GI_Items[i].I_Propertys[s].I_Duration, GameItem.GI_Items[i].I_Propertys[s].I_Amount, 1);
                            }
                        }
                        // Fixed Noclip Item
                        else
                        {
                            GameSnake.S_NOCLIP = true;
                        }
                    }
                }
            }
        }
    }
}


