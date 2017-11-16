#include "CSFMLGameTilemap.h"

// Load Level
bool CSFMLLoadlevel(int id)
{
    if(!id)
        return false;

    // Build-up Level Path
    char lID[5];
    strcat(Level.Level_Path, "Bilder/Game/LevelData/Level");
    itoa(id, lID, 10);
    strcat(Level.Level_Path, lID);
    strcat(Level.Level_Path, "/");
    Level.Is_Init = true;

    // Init all sub functions
    bool InitError = false;

    InitError |= !CSFMLLoadBackground();
    InitError |= !CSFMLLoadTilemap();
    InitError |= !CSFMLLoadEventmap();
    InitError |= !CSFMLLoadMapDescriptor();

    if(InitError)
    {
        printf("Game Subsystem Fehler 'GameTilemap': Fehler beim Laden!\n");
        return false;
    }

    // General things
    Level.Level_ID = id;
    Level.Is_Loaded = true;

    return true;
}

bool CSFMLRenderLevel()
{
    if(!Level.Is_Init || !Level.Is_Loaded)
        return false;

    // Render
    if(shader_enabled)
        sfRenderWindow_drawSprite(screen, Level.BG_Sprite, &Level.BG_Texture_State);
    else
        sfRenderWindow_drawSprite(screen, Level.BG_Sprite, NULL);


    return true;
}

// Quit Level
void CSFMLQuitLevel()
{
    // Delete Current Frame
    sfSprite_setColor(Level.BG_Sprite, sfBlack);
    sfRenderWindow_drawSprite(screen, Level.BG_Sprite, NULL);

    // Clear Shader
    if(shader_enabled)
    {
        sfShader_destroy(Level.BG_Texture_Shader);
        Level.BG_Texture_Shader = NULL;
    }

    // Clear SFML Stuff
    sfTexture_destroy(Level.BG_Texture_Diffuse);
    sfTexture_destroy(Level.BG_Texture_Normal);
    sfTexture_destroy(Level.BG_Texture_Specular);
    sfSprite_destroy(Level.BG_Sprite);
    sfView_destroy(Level.BG_View);
    Level.BG_Texture_Diffuse = NULL;
    Level.BG_Texture_Normal = NULL;
    Level.BG_Texture_Specular = NULL;
    Level.BG_Sprite = NULL;
    Level.BG_View = NULL;

    // Clear Event Stuff
    free(Level.EV_Map);
    Level.EV_Map = NULL;

    // Reset general things
    Level.Is_Init = false;
    Level.BG_Is_Init = false;
    Level.EV_Is_Init = false;
    Level.Is_Loaded = false;
    Level.TL_Is_Init = false;
    Level.Level_Path[0] = '\0';
}

// Loading MapDescriptor
bool CSFMLLoadMapDescriptor()
{
    if(!Level.Is_Init || !Level.BG_Is_Init || !Level.TL_Is_Init || !Level.EV_Is_Init)
        return false;

    // Define Path
    char CompletePath_MapDescriptor[100] = {0};
    strncat(CompletePath_MapDescriptor, Level.Level_Path, strlen(Level.Level_Path)+1);
    strncat(CompletePath_MapDescriptor, "MapDescriptor.DAT", 18);

    // Load Data
    FILE *Mapdesc = fopen(CompletePath_MapDescriptor, "rb");

    if(Mapdesc == NULL)
    {
        printf("Game Subsystem Fehler 'GameTilemap': Kann Mapdescriptor nicht laden!\n");
        return false;
    }

    if(fread(&Level.MD_Details, sizeof(MapDescriptor), 1, Mapdesc) != 1)
    {
        printf("Game Subsystem Fehler 'GameTilemap': Kann keine Daten aus Mapdescriptor lesen!\n");
        return false;
    }

    // Setting up general things
    Level.MD_Is_Init = true;

    // Cleanup things
    fclose(Mapdesc);
    Mapdesc = NULL;

    return true;
}

// Preload Mapname for the Menu
bool CSFMLPreloadMapName(const char *Path, char *LevelName, size_t LevelName_Size)
{
    // Validate for Input
    if(Path == NULL || LevelName == NULL)
    {
        printf("Game Menu Subsystem Fehler: Null Pointer uebergeben!\n");
        return false;
    }

    // Validate for Illegal Input Size, Maximum is 20
    if(LevelName_Size > 20)
    {
        printf("Game Menu Subsystem Fehler: Groesse der uebergebenen Groesse passt nicht!\n");
        return false;
    }

    // Validate for Path Size
    if(strlen(Path)+1 >= 82)
    {
        printf("Game Menu Subsystem Fehler: Pfadlaenge zu groess!!\n");
        return false;
    }

    // Build Path
    char CompletePath[100] = {0};
    strncat(CompletePath, "Bilder/Game/LevelData/", 22);
    strncat(CompletePath, Path, strlen(Path)+1);
    strncat(CompletePath, "/", 1);
    strncat(CompletePath, "Mapdescriptor.DAT", 18);

    // Loading File
    FILE *MD_File = fopen(CompletePath, "rb");
    MapDescriptor MD_Name;

    if(MD_File == NULL)
    {
        printf("Game Menu Subsystem Fehler: Kann Mapdescriptor Datei nicht oeffnen!\n");
        return false;
    }

    if(fread(&MD_Name, sizeof(MapDescriptor), 1, MD_File) != 1)
    {
        printf("Game Menu Subsystem Fehler: Kann Mapdescriptor nicht laden!\n");
        return false;
    }

    // Building Now Level Name!
    size_t i;
    for(i = 0; i<LevelName_Size; i++)
    {
        LevelName[i] = MD_Name.MapName[i];
    }

    fclose(MD_File);
    MD_File = NULL;

    return true;
}

// Loading Eventmap
bool CSFMLLoadEventmap()
{
    if(!Level.Is_Init || !Level.BG_Is_Init || !Level.TL_Is_Init)
        return false;

    // Define Path
    char CompletePath_EventMap[100] = {0};
    strncat(CompletePath_EventMap, Level.Level_Path, strlen(Level.Level_Path)+1);
    strncat(CompletePath_EventMap, "Eventmap.DAT", 13);

    // Load Data
    FILE *EventMap = fopen(CompletePath_EventMap, "rb");

    if(EventMap == NULL)
    {
        printf("Game Subsystem Fehler 'GameTilemap': Kann Eventmap nicht laden!\n");
        return false;
    }

    // Get Event Entrys
    long file_size = 0;
    size_t entrys;

    fseek(EventMap, 0L, SEEK_END);
    file_size = ftell(EventMap);
    entrys = (size_t)file_size / sizeof(Event);

    // Get Events to structure
    if(entrys <= 0)
    {
        printf("Game Subsystem Warning 'GameTilemap': Keine Events in der Datei!\n");
        Level.EV_Count = 0;
        Level.EV_Map = NULL;
    }
    else
    {
        rewind(EventMap);

        Level.EV_Map = malloc(entrys * sizeof(Event));

        if(Level.EV_Map == NULL)
        {
            printf("Game Subsystem Fehler 'GameTilemap': Kann Speicher fuer Events nicht reservieren!\n");
            return false;
        }

        if(fread(Level.EV_Map, sizeof(Event), entrys, EventMap) != entrys)
        {
            printf("Game Subsystem Fehler 'GameTilemap': Gelesene Events entspricht nicht der Anzahl!\n");
            return false;
        }

        Level.EV_Count = entrys;
    }

    // Setting up general things
    Level.EV_Is_Init = true;

    // Cleanup things
    fclose(EventMap);
    EventMap = NULL;

    return true;
}

// Loading Tilemap
bool CSFMLLoadTilemap()
{
    if(!Level.Is_Init || !Level.BG_Is_Init)
        return false;

    // Define Path
    char CompletePath_Tilemap[100] = {0};
    strncat(CompletePath_Tilemap, Level.Level_Path, strlen(Level.Level_Path)+1);
    strncat(CompletePath_Tilemap, "Tilemap.DAT", 12);

    // Load Data
    FILE *TileMap = fopen(CompletePath_Tilemap, "rb");

    if(TileMap == NULL)
    {
        printf("Game Subsystem Fehler 'GameTilemap': Kann Tilemap nicht laden!\n");
        return false;
    }

    if(fread(Level.TL_Map, sizeof(int), MAX_TILES_X*MAX_TILES_Y, TileMap) != MAX_TILES_X*MAX_TILES_Y)
    {
        printf("Game Subsystem Fehler 'GameTilemap': Es wurden keine Datensaetze gelesen!\n");
        return false;
    }

    // Setting Start and End Position of Level!
    size_t i, n;
    bool start = false, end = false;
    for(i = 0; i<MAX_TILES_Y; i++)
    {
        for(n = 0; n<MAX_TILES_X; n++)
        {
            // Start Position
            if(Level.TL_Map[i][n] == 5)
            {
                Level.TL_Start_Position.x = n;
                Level.TL_Start_Position.y = i;
                start = true;
            }

            // End Position
            if(Level.TL_Map[i][n] == 6)
            {
                Level.TL_End_Position.x = n;
                Level.TL_End_Position.y = i;
                end = true;
            }
        }
        printf("\n");
    }

    if(!start || !end)
    {
        printf("Game Subsystem Fehler 'GameTilemap': Keine Start oder End Position in Tilemap!\n");
        return false;
    }

    // Setting Up general things;
    Level.TL_X_Size = 50;
    Level.TL_Y_Size = 50;
    Level.TL_Is_Init = true;

    // Clean Up things
    fclose(TileMap);
    TileMap = NULL;

    return true;
}

// Loading Background with Shader
bool CSFMLLoadBackground()
{
    if(!Level.Is_Init)
        return false;

    // Defining Paths
    char CompletePath_Diffuse_Texture[100] = {0};
    char CompletePath_Normal_Texture[100] = {0};
    char CompletePath_Specular_Texture[100] = {0};

    strncat(CompletePath_Diffuse_Texture, Level.Level_Path, strlen(Level.Level_Path)+1);
    strncat(CompletePath_Diffuse_Texture, "Diffuse.png", 12);
    strncat(CompletePath_Normal_Texture, Level.Level_Path, strlen(Level.Level_Path)+1);
    strncat(CompletePath_Normal_Texture, "Normal.png", 11);
    strncat(CompletePath_Specular_Texture, Level.Level_Path, strlen(Level.Level_Path)+1);
    strncat(CompletePath_Specular_Texture, "Specular.png", 13);

    // Initialize Textures, Sprite and Shader
    Level.BG_Texture_Diffuse = sfTexture_createFromFile(CompletePath_Diffuse_Texture, NULL);
    Level.BG_Texture_Normal = sfTexture_createFromFile(CompletePath_Normal_Texture, NULL);
    Level.BG_Texture_Specular = sfTexture_createFromFile(CompletePath_Specular_Texture, NULL);
    Level.BG_Sprite = sfSprite_create();
    Level.BG_View = sfView_create();

    if(Level.BG_Texture_Diffuse == NULL || Level.BG_Texture_Normal == NULL || Level.BG_Texture_Specular == NULL || Level.BG_Sprite == NULL || Level.BG_View == NULL)
    {
        printf("Game Subsystem Fehler 'GameTilemap': Dateien zu BG konnte nicht geladen werden!\n");
        return false;
    }

    if(shader_enabled)
    {
        // Build Path
        char CompletePath_Shader[100] = {0};
        strncat(CompletePath_Shader, Level.Level_Path, strlen(Level.Level_Path)+1);
        strncat(CompletePath_Shader, "Shader.frag", 12);

        // Load Shader
        Level.BG_Texture_Shader = sfShader_createFromFile(NULL, NULL, CompletePath_Shader);

        if(Level.BG_Texture_Shader == NULL)
        {
            printf("Game Subsystem Fehler 'GameTilemap': Konnte Shader fuer Background nicht laden!\n");
            return false;
        }

        // Setting up Shader Parameters
        sfShader_setTextureUniform(Level.BG_Texture_Shader, "Diffuse_Textur", Level.BG_Texture_Diffuse);
        sfShader_setTextureUniform(Level.BG_Texture_Shader, "Normal_Textur", Level.BG_Texture_Normal);
        sfShader_setTextureUniform(Level.BG_Texture_Shader, "Spiegel_Textur", Level.BG_Texture_Specular);
        sfShader_setBoolUniform(Level.BG_Texture_Shader, "Desaturate", false);
        Level.BG_Texture_State.blendMode = sfBlendNone;
        Level.BG_Texture_State.shader = Level.BG_Texture_Shader;
        Level.BG_Texture_State.texture = Level.BG_Texture_Diffuse;
        Level.BG_Texture_State.transform = sfTransform_Identity;
    }
    else
    {
        Level.BG_Texture_Shader = NULL; // prevent dangling pointer
    }

    // Setup Views
    sfFloatRect View_Rect;
    View_Rect.width = 572.0f;
    View_Rect.height = 472.0f;
    View_Rect.left = 14.0f;
    View_Rect.top = 214.0f;
    sfView_reset(Level.BG_View, View_Rect);

    // Setup Target Views
    sfFloatRect Rec;
    Rec.width = View_Rect.width / SCREEN_WIDTH;
    Rec.height = View_Rect.height / SCREEN_HEIGHT;
    Rec.left = View_Rect.left / SCREEN_WIDTH;
    Rec.top = View_Rect.top / SCREEN_HEIGHT;
    sfView_setViewport(Level.BG_View, Rec);

    // Setting up general things
    sfVector2f Position = {14, 214};
    Level.BG_Width = sfTexture_getSize(Level.BG_Texture_Diffuse).x;
    Level.BG_Height = sfTexture_getSize(Level.BG_Texture_Diffuse).y;
    Level.BG_Screenposition = Position;
    sfSprite_setPosition(Level.BG_Sprite, Position);
    sfSprite_setTexture(Level.BG_Sprite, Level.BG_Texture_Diffuse, sfTrue);
    Level.BG_Is_Init = true;

    return true;
}
