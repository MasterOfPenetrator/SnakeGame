#include "CSFMLGameWeapons.h"

// Init Game Weapons
bool CSFMLInitWeapons()
{
    // Setup
    DIR *dir;
    struct dirent *entrys;
    GameWeapons.GW_Weapon_Count = 0;

    // Get Count of Weapons
    if((dir = opendir("Bilder/Game/WeaponData")) == NULL)
    {
        printf("Game Subsystem Fehler 'GameWeapons': Kann Verzeichnis zu den Waffen nicht oeffnen!\n");
        return false;
    }

    while((entrys = readdir(dir)) != NULL)
    {
        // Ignore File Name "." and ".."
        if(strcmp(entrys->d_name, ".") && strcmp(entrys->d_name, ".."))
        {
            GameWeapons.GW_Weapon_Count++;
        }
    }

    // Rewind it
    rewinddir(dir);

    // Check Weapon Count
    if(GameWeapons.GW_Weapon_Count == 0)
    {
        printf("Game Subsystem Fehler 'GameWeapons': Keine Waffen hinterlegt!\n");
        return false;
    }

    // Setup General Memory Stuff
    GameWeapons.GW_Weapons = malloc(GameWeapons.GW_Weapon_Count * sizeof(Weapon));
    GameWeapons.GW_BarrelTexture = malloc(GameWeapons.GW_Weapon_Count * sizeof(sfTexture*));
    GameWeapons.GW_WeaponTexture = malloc(GameWeapons.GW_Weapon_Count * sizeof(sfTexture*));
    GameWeapons.GW_WeaponSprite = sfSprite_create();
    GameWeapons.GW_BarrelSprite = sfSprite_create();
    GameWeapons.GW_Active = malloc(GameWeapons.GW_Weapon_Count * sizeof(bool));
    GameWeapons.GW_Weapon_Block = malloc(GameWeapons.GW_Weapon_Count * sizeof(wBlock));
    GameWeapons.GW_Barrel_Block = malloc(GameWeapons.GW_Weapon_Count * sizeof(wBlock*));
    GameWeapons.GW_Barrel_Block_Count = malloc(GameWeapons.GW_Weapon_Count * sizeof(size_t));

    if(GameWeapons.GW_Weapons == NULL || GameWeapons.GW_BarrelTexture == NULL || GameWeapons.GW_WeaponTexture == NULL || GameWeapons.GW_WeaponSprite == NULL || GameWeapons.GW_BarrelSprite == NULL || GameWeapons.GW_Active == NULL || GameWeapons.GW_Weapon_Block == NULL || GameWeapons.GW_Barrel_Block == NULL || GameWeapons.GW_Barrel_Block_Count == NULL)
    {
        printf("Game Subsystem Fehler 'GameWeapons': Kann Speicher nicht reservieren!\n");
        return false;
    }

    // Setupt Shader Stuff
    if(shader_enabled)
    {
        GameWeapons.GW_WeaponShader = malloc(GameWeapons.GW_Weapon_Count * sizeof(sfShader*));
        GameWeapons.GW_WeaponShaderState = malloc(GameWeapons.GW_Weapon_Count * sizeof(sfRenderStates));
        GameWeapons.GW_BarrelShader = malloc(GameWeapons.GW_Weapon_Count * sizeof(sfShader*));
        GameWeapons.GW_BarrelShaderState = malloc(GameWeapons.GW_Weapon_Count * sizeof(sfRenderStates));

        if(GameWeapons.GW_WeaponShader == NULL || GameWeapons.GW_WeaponShaderState == NULL || GameWeapons.GW_BarrelShader == NULL || GameWeapons.GW_BarrelShaderState == NULL)
        {
            printf("Game Subsystem Fehler 'GameWeapons': Kann Shader Speicher nicht setzen!\n");
            perror("Fehler: ");
            return false;
        }
    }
    else
    {
        GameWeapons.GW_WeaponShader = NULL; // prevent dangling Pointer
        GameWeapons.GW_WeaponShaderState = NULL;
        GameWeapons.GW_BarrelShader = NULL;
        GameWeapons.GW_BarrelShaderState = NULL;
    }

    // Now get Data
    size_t counter = 0;
    while((entrys = readdir(dir)) != NULL)
    {
        // Ignore File Name "." and ".."
        if(strcmp(entrys->d_name, ".") && strcmp(entrys->d_name, ".."))
        {
            // Build Path
            char Path[50] = {0};
            char W_Tex[100] = {0}, B_Tex[100] = {0};
            strncat(Path, "Bilder/Game/WeaponData/", 24);
            strncat(Path, entrys->d_name, strlen(entrys->d_name)+1);
            strncat(Path, "/", 2);

            // Loading Textures
            strncat(W_Tex, Path, strlen(Path)+1);
            strncat(W_Tex, "Weapon_Texture.png", 18);
            strncat(B_Tex, Path, strlen(Path)+1);
            strncat(B_Tex, "Barrel_Texture.png", 18);
            GameWeapons.GW_WeaponTexture[counter] = sfTexture_createFromFile(W_Tex, NULL);
            GameWeapons.GW_BarrelTexture[counter] = sfTexture_createFromFile(B_Tex, NULL);

            if(GameWeapons.GW_WeaponTexture[counter] == NULL || GameWeapons.GW_BarrelTexture[counter] == NULL)
            {
                printf("Game Subsystem Fehler 'GameWeapons': Kann Texturen nicht laden!\n");
                return false;
            }

            // Loading Shader
            if(shader_enabled)
            {
                char W_Shader[100] = {0}, B_Shader[100] = {0};
                strncat(W_Shader, Path, strlen(Path)+1);
                strncat(W_Shader, "Weapon_Shader.frag", 19);
                strncat(B_Shader, Path, strlen(Path)+1);
                strncat(B_Shader, "Barrel_Shader.frag", 19);

                GameWeapons.GW_WeaponShader[counter] = sfShader_createFromFile(NULL, NULL, W_Shader);
                GameWeapons.GW_BarrelShader[counter] = sfShader_createFromFile(NULL, NULL, B_Shader);

                if(GameWeapons.GW_WeaponShader[counter] == NULL || GameWeapons.GW_BarrelShader[counter] == NULL)
                {
                    printf("Game Subsystem Fehler 'GameWeapons': Kann Shader nicht laden!\n");
                    return false;
                }

                // Setup RenderStates
                GameWeapons.GW_WeaponShaderState[counter].blendMode = sfBlendAlpha;
                GameWeapons.GW_WeaponShaderState[counter].shader = GameWeapons.GW_WeaponShader[counter];
                GameWeapons.GW_WeaponShaderState[counter].texture = GameWeapons.GW_WeaponTexture[counter];
                GameWeapons.GW_WeaponShaderState[counter].transform = sfTransform_Identity;
                GameWeapons.GW_BarrelShaderState[counter].blendMode = sfBlendAlpha;
                GameWeapons.GW_BarrelShaderState[counter].shader = GameWeapons.GW_BarrelShader[counter];
                GameWeapons.GW_BarrelShaderState[counter].texture = GameWeapons.GW_BarrelTexture[counter];
                GameWeapons.GW_BarrelShaderState[counter].transform = sfTransform_Identity;

                // Set Shader Values
                sfVector2f W_Tex_Size = {sfTexture_getSize(GameWeapons.GW_WeaponTexture[counter]).x, sfTexture_getSize(GameWeapons.GW_WeaponTexture[counter]).y};
                sfShader_setVec2Uniform(GameWeapons.GW_WeaponShader[counter], "Resolution", W_Tex_Size);
                sfShader_setTextureUniform(GameWeapons.GW_WeaponShader[counter], "Texture", GameWeapons.GW_WeaponTexture[counter]);
                sfShader_setTextureUniform(GameWeapons.GW_BarrelShader[counter], "Texture", GameWeapons.GW_BarrelTexture[counter]);
            }

            // Loading Weapon Descriptor
            char W_Descriptor[100] = {0};
            strncat(W_Descriptor, Path, strlen(Path)+1);
            strncat(W_Descriptor, "Weapon_Descriptor.DAT", 22);

            FILE *Desc = fopen(W_Descriptor, "rb");

            if(Desc == NULL)
            {
                printf("Game Subsystem Fehler 'GameWeapons': Kann Waffen Desciptor nicht oeffnen!\n");
                return false;
            }

            if(fread(&GameWeapons.GW_Weapons[counter], sizeof(Weapon), 1, Desc) != 1)
            {
                printf("Game Subsystem Fehler 'GameWeapons': Kann Waffen Descriptor nicht laden!\n");
                return false;
            }

            // Set it Inactive
            // For Debug & Testing it is every Active
            GameWeapons.GW_Active[counter] = true;

            // Set Weapon Blocks
            GameWeapons.GW_Weapon_Block[counter].w = sfTexture_getSize(GameWeapons.GW_WeaponTexture[counter]).x;
            GameWeapons.GW_Weapon_Block[counter].h = sfTexture_getSize(GameWeapons.GW_WeaponTexture[counter]).y;
            GameWeapons.GW_Weapon_Block[counter].x = GameSnake.SB_Head.x + 0.5f;
            GameWeapons.GW_Weapon_Block[counter].y = GameSnake.SB_Head.y;

            // Init Barrel Blocks
            GameWeapons.GW_Barrel_Block_Count[counter] = 0;
            GameWeapons.GW_Barrel_Block[counter] = NULL; // Prevent dangling Pointers

            // Close things
            fclose(Desc);
            Desc = NULL;

            // Count up
            counter++;
        }
    }

    // Close things
    closedir(dir);
    dir = NULL;

    GameWeapons.GW_Fire_Pressed = false;
    GameWeapons.GW_Is_Init = true;

    return true;
}

// Quit Weapons
void CSFMLQuitWeapons()
{
    size_t i;

    // Clean up shader
    if(shader_enabled)
    {
        for(i = 0; i<GameWeapons.GW_Weapon_Count; i++)
        {
            sfShader_destroy(GameWeapons.GW_WeaponShader[i]);
            GameWeapons.GW_WeaponShader[i] = NULL;

            sfShader_destroy(GameWeapons.GW_BarrelShader[i]);
            GameWeapons.GW_BarrelShader[i] = NULL;
        }

        free(GameWeapons.GW_WeaponShader);
        GameWeapons.GW_WeaponShader = NULL;

        free(GameWeapons.GW_BarrelShader);
        GameWeapons.GW_BarrelShader = NULL;

        free(GameWeapons.GW_WeaponShaderState);
        GameWeapons.GW_WeaponShaderState = NULL;

        free(GameWeapons.GW_BarrelShaderState);
        GameWeapons.GW_BarrelShaderState = NULL;
    }

    // Clean up Weapon Blocks
    free(GameWeapons.GW_Weapon_Block);
    GameWeapons.GW_Weapon_Block = NULL;

    // Clean up other things
    for(i = 0; i<GameWeapons.GW_Weapon_Count; i++)
    {
        sfTexture_destroy(GameWeapons.GW_WeaponTexture[i]);
        GameWeapons.GW_WeaponTexture[i] = NULL;

        sfTexture_destroy(GameWeapons.GW_BarrelTexture[i]);
        GameWeapons.GW_BarrelTexture[i] = NULL;

        if(GameWeapons.GW_Barrel_Block_Count[i] > 0)
        {
            free(GameWeapons.GW_Barrel_Block[i]);
            GameWeapons.GW_Barrel_Block[i] = NULL;
        }
    }

    free(GameWeapons.GW_Barrel_Block);
    GameWeapons.GW_Barrel_Block = NULL;

    free(GameWeapons.GW_WeaponTexture);
    GameWeapons.GW_WeaponTexture = NULL;

    free(GameWeapons.GW_BarrelTexture);
    GameWeapons.GW_BarrelTexture = NULL;

    free(GameWeapons.GW_Weapons);
    GameWeapons.GW_Weapons = NULL;

    free(GameWeapons.GW_Active);
    GameWeapons.GW_Active = NULL;

    sfSprite_destroy(GameWeapons.GW_WeaponSprite);
    GameWeapons.GW_WeaponSprite = NULL;

    sfSprite_destroy(GameWeapons.GW_BarrelSprite);
    GameWeapons.GW_BarrelSprite = NULL;

    GameWeapons.GW_Weapon_Count = 0;
    GameWeapons.GW_Is_Init = false;
}

// Render Weapon
void CSFMLRenderWeapons()
{
    size_t i;
    for(i = 0; i<GameWeapons.GW_Weapon_Count; i++)
    {
        // Is Weapon Active?
        if(GameWeapons.GW_Active[i])
        {
            // Set Weapon Texture and Position
            sfSprite_setTexture(GameWeapons.GW_WeaponSprite, GameWeapons.GW_WeaponTexture[i], sfTrue);
            sfSprite_setPosition(GameWeapons.GW_WeaponSprite, CSFMLWeaponConvertIndexToVector(GameWeapons.GW_Weapon_Block[i]));

            // Render It
            if(shader_enabled)
            {
                sfShader_setIntUniform(GameWeapons.GW_WeaponShader[i], "Rotation", GameSnake.S_Rotate);
                sfRenderWindow_drawSprite(screen, GameWeapons.GW_WeaponSprite, &GameWeapons.GW_WeaponShaderState[i]);
            }
            else
                sfRenderWindow_drawSprite(screen, GameWeapons.GW_WeaponSprite, NULL);
        }
    }
}

void CSFMLHandleWeapons()
{

    // Handle Weapon Sprite (on the Snakehead)
    size_t i;
    for(i = 0; i<GameWeapons.GW_Weapon_Count; i++)
    {
        // Update Weapon Position
        if(GameSnake.S_Actual_Direction == UP)
        {
            GameWeapons.GW_Weapon_Block[i].x = GameSnake.SB_Head.x;
            GameWeapons.GW_Weapon_Block[i].y = GameSnake.SB_Head.y - (2*((float)GameWeapons.GW_Weapon_Block[i].h/Level.TL_Y_Size));
        }
        else if(GameSnake.S_Actual_Direction == DOWN)
        {
            GameWeapons.GW_Weapon_Block[i].x = GameSnake.SB_Head.x;
            GameWeapons.GW_Weapon_Block[i].y = GameSnake.SB_Head.y + (2*((float)GameWeapons.GW_Weapon_Block[i].h/Level.TL_Y_Size));
        }
        else if(GameSnake.S_Actual_Direction == LEFT)
        {
            GameWeapons.GW_Weapon_Block[i].x = GameSnake.SB_Head.x - (2*((float)GameWeapons.GW_Weapon_Block[i].w/Level.TL_X_Size));
            GameWeapons.GW_Weapon_Block[i].y = GameSnake.SB_Head.y;
        }
        else if(GameSnake.S_Actual_Direction == RIGHT)
        {
            GameWeapons.GW_Weapon_Block[i].x = GameSnake.SB_Head.x + (2*((float)GameWeapons.GW_Weapon_Block[i].w/Level.TL_X_Size));
            GameWeapons.GW_Weapon_Block[i].y = GameSnake.SB_Head.y;
        }
    }
}

// Converting Array Indices to Realtime Coordinates!
sfVector2f CSFMLWeaponConvertIndexToVector(wBlock blk)
{
    sfVector2f Ret_Value;

    Ret_Value.x = (blk.x * Level.TL_X_Size) + Level.BG_Screenposition.x;
    Ret_Value.y = (blk.y * Level.TL_Y_Size) + Level.BG_Screenposition.y;

    return Ret_Value;
}
