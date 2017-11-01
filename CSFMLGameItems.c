#include "CSFMLGameItems.h"

// Init all Items
bool CSFMLInitItems()
{
    // Getting Amount of Item Files
    DIR *dir;
    struct dirent *entrys;

    // Set First Memory
    GameItem.GI_Items = malloc(sizeof(Item));
    GameItem.GI_Shaders = malloc(sizeof(sfShader*));
    GameItem.GI_Textures = malloc(sizeof(sfTexture*));
    GameItem.GI_States = malloc(sizeof(sfRenderStates));
    GameItem.GI_Blocks = malloc(sizeof(iBlock));
    GameItem.GI_Placed = malloc(sizeof(bool));
    GameItem.GI_Coordinates_Setted = malloc(sizeof(bool));
    GameItem.GI_Items_Count = 0;

    if(GameItem.GI_Items == NULL || GameItem.GI_Shaders == NULL || GameItem.GI_Textures == NULL || GameItem.GI_States == NULL || GameItem.GI_Blocks == NULL || GameItem.GI_Placed == NULL || GameItem.GI_Coordinates_Setted == NULL)
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
    while((entrys = readdir(dir)) != NULL)
    {
        // Ignore File Name "." and ".."
        if(strcmp(entrys->d_name, ".") && strcmp(entrys->d_name, ".."))
        {
            // Set Memory
            GameItem.GI_Items = realloc(GameItem.GI_Items, (GameItem.GI_Items_Count+1)*sizeof(Item));
            GameItem.GI_Textures = realloc(GameItem.GI_Textures, (GameItem.GI_Items_Count+1)*sizeof(sfTexture*));
            GameItem.GI_Shaders = realloc(GameItem.GI_Shaders, (GameItem.GI_Items_Count+1)*sizeof(sfShader*));
            GameItem.GI_States = realloc(GameItem.GI_States, (GameItem.GI_Items_Count+1)*sizeof(sfRenderStates));
            GameItem.GI_Blocks = realloc(GameItem.GI_Blocks, (GameItem.GI_Items_Count+1)*sizeof(iBlock));
            GameItem.GI_Placed = realloc(GameItem.GI_Placed, (GameItem.GI_Items_Count+1)*sizeof(bool));
            GameItem.GI_Coordinates_Setted = realloc(GameItem.GI_Coordinates_Setted, (GameItem.GI_Items_Count+1)*sizeof(bool));
            if(GameItem.GI_Items == NULL || GameItem.GI_Textures == NULL || GameItem.GI_Shaders == NULL || GameItem.GI_States == NULL || GameItem.GI_Blocks == NULL || GameItem.GI_Placed == NULL || GameItem.GI_Coordinates_Setted == NULL)
            {
                printf("Game Subsystem Fehler 'GameItems': Kann Neuen Speicher nicht reservieren!\n");
                return false;
            }

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
            if(fread(&GameItem.GI_Items[GameItem.GI_Items_Count], sizeof(Item), 1, actual) != 1)
            {
                printf("Game Subsystem Fehler 'GameItems': Kann Datei nicht lesen!\n");
                return false;
            }

            // Setup Shader
            GameItem.GI_Shaders[GameItem.GI_Items_Count] = sfShader_createFromFile(NULL, NULL, GameItem.GI_Items[GameItem.GI_Items_Count].I_ShaderPath);
            if(GameItem.GI_Shaders[GameItem.GI_Items_Count] == NULL)
            {
                printf("Game Subsystem Fehler 'GameItems': Kann Shader nicht laden!\n");
                return false;
            }

            // Setup Texture
            GameItem.GI_Textures[GameItem.GI_Items_Count] = sfTexture_createFromFile(GameItem.GI_Items[GameItem.GI_Items_Count].I_TilePath, NULL);
            if(GameItem.GI_Textures[GameItem.GI_Items_Count] == NULL)
            {
                printf("Game Subsystem Fehler 'GameItems': Kann Textur nicht laden!\n");
                return false;
            }

            // Setup Shader Values
            sfShader_setTextureUniform(GameItem.GI_Shaders[GameItem.GI_Items_Count], "Texture", GameItem.GI_Textures[GameItem.GI_Items_Count]);
            sfShader_setFloatUniform(GameItem.GI_Shaders[GameItem.GI_Items_Count], "Time", GameClock.GC_Time);

            // Setup Block Values
            GameItem.GI_Blocks[GameItem.GI_Items_Count].x = 0;
            GameItem.GI_Blocks[GameItem.GI_Items_Count].y = 0;
            GameItem.GI_Blocks[GameItem.GI_Items_Count].w = (float)sfTexture_getSize(GameItem.GI_Textures[GameItem.GI_Items_Count]).x;
            GameItem.GI_Blocks[GameItem.GI_Items_Count].h = (float)sfTexture_getSize(GameItem.GI_Textures[GameItem.GI_Items_Count]).y;

            // Setup RenderState
            GameItem.GI_States[GameItem.GI_Items_Count].blendMode = sfBlendAlpha;
            GameItem.GI_States[GameItem.GI_Items_Count].shader = GameItem.GI_Shaders[GameItem.GI_Items_Count];
            GameItem.GI_States[GameItem.GI_Items_Count].texture = GameItem.GI_Textures[GameItem.GI_Items_Count];
            GameItem.GI_States[GameItem.GI_Items_Count].transform = sfTransform_Identity;

            // All Items all first not Placed and Coordinates are not setted
            GameItem.GI_Placed[GameItem.GI_Items_Count] = false;
            GameItem.GI_Coordinates_Setted[GameItem.GI_Items_Count] = false;

            // Update Item Count
            GameItem.GI_Items_Count++;

            // Close Actual File
            fclose(actual);
            actual = NULL;
        }
    }

    // Close Dir
    closedir(dir);
    dir = NULL;

    // Setup general things
    GameItem.GI_Is_Init = true;
    srand(time(NULL));

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

    // Clear Other Stuff
    free(GameItem.GI_Shaders);
    GameItem.GI_Shaders = NULL;

    free(GameItem.GI_Textures);
    GameItem.GI_Textures = NULL;

    free(GameItem.GI_Blocks);
    GameItem.GI_Blocks = NULL;

    free(GameItem.GI_States);
    GameItem.GI_States = NULL; // to Here, if this Active, at Quit, it leads to Segmentation Fault..

    free(GameItem.GI_Items);
    GameItem.GI_Items = NULL;

    free(GameItem.GI_Coordinates_Setted);
    GameItem.GI_Coordinates_Setted = NULL;

    free(GameItem.GI_Placed);
    GameItem.GI_Placed = NULL;

    GameItem.GI_Is_Init = false;
}

// Set Placed Variable
void CSFMLSetPlaceItems()
{
    // Iterate over all Items
    size_t i;
    for(i = 0; i<GameItem.GI_Items_Count; i++)
    {
        // Just proceed with unplaced and >0% Chance Items
        if(!GameItem.GI_Placed[i] && (GameItem.GI_Items[i].I_Chance > 0))
        {
            // Need it to Place ? Calculate it, with the Chance
            bool Place_It = false;
            int random = rand() % 100 + 1;
            if(random > 0 && random <= GameItem.GI_Items[i].I_Chance)
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

    // Check Snake Collide Head
    if(random_x == GameSnake.SB_Head.x && random_y == GameSnake.SB_Head.y)
        Hit = true;

    // Check Snake Collide Body
    for(i = 0; i<GameSnake.SB_Body_Elements; i++)
    {
        if(GameSnake.SB_Body[i].x == random_x && GameSnake.SB_Body[i].y == random_y)
            Hit = true;
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
// Converting Head Array Indices to Screen Space Coordinates
sfVector2f CSFMLItemConvertIndexToVector(iBlock blk)
{
    sfVector2f Ret_Value;

    Ret_Value.x = (blk.x * Level.TL_X_Size) + Level.BG_Screenposition.x;
    Ret_Value.y = (blk.y * Level.TL_Y_Size) + Level.BG_Screenposition.y;

    return Ret_Value;
}

// Handle Items
void CSFMLHandleItems()
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
            sfSprite *item = sfSprite_create();
            sfSprite_setTexture(item, GameItem.GI_Textures[i], sfTrue);
            sfSprite_setPosition(item, CSFMLItemConvertIndexToVector(GameItem.GI_Blocks[i]));
            sfRenderWindow_drawSprite(screen, item, &GameItem.GI_States[i]);
            sfSprite_destroy(item);
            item = NULL;
        }
    }
}


