#include "CSFMLGameLights.h"

// Init Lights
bool CSFMLInitLights()
{
    if(!Level.Is_Init || !Level.Is_Loaded)
        return false;

    // Allocate maximum Lights in Memory
    GameLight_Ambientpower = malloc(MAX_LIGHTS * sizeof(float));
    GameLight_Coneactive = malloc(MAX_LIGHTS * sizeof(float));
    GameLight_Coneangle = malloc(MAX_LIGHTS * sizeof(float));
    GameLight_Conepos = malloc(MAX_LIGHTS * sizeof(sfGlslVec3));
    GameLight_FBMactive = malloc(MAX_LIGHTS * sizeof(float));
    GameLight_Lightpos = malloc(MAX_LIGHTS * sizeof(sfGlslVec3));
    GameLight_ID = malloc(MAX_LIGHTS * sizeof(int));
    GameLight_Active = malloc(MAX_LIGHTS * sizeof(float));

    if(GameLight_Ambientpower == NULL || GameLight_Coneactive == NULL || GameLight_Coneangle == NULL || GameLight_FBMactive == NULL || GameLight_Lightpos == NULL || GameLight_Conepos == NULL || GameLight_ID == NULL || GameLight_Active == NULL)
    {
        printf("Game Subsystem Fehler 'GameLights': Kann Speicher fuer Lichter nicht initialisieren!\n");
        return false;
    }

    GameLight_Is_Init = true;
    GameLight_Viewport_Setted = false;
    GameLight_Count = 0;

    return true;
}

// Render Lights
bool CSFMLRenderLights()
{
    if(!GameLight_Is_Init)
        return false;

    // Set Shader Variables
    if(shader_enabled)
    {
        sfShader_setFloatUniformArray(Level.BG_Texture_Shader, "Licht_Kegel_Aktiv", GameLight_Coneactive, GameLight_Count);
        sfShader_setFloatUniformArray(Level.BG_Texture_Shader, "FBM_Kegel_Aktiv", GameLight_FBMactive, GameLight_Count);
        sfShader_setFloatUniformArray(Level.BG_Texture_Shader, "Ambient_Staerke", GameLight_Ambientpower, GameLight_Count);
        sfShader_setVec3UniformArray(Level.BG_Texture_Shader, "Licht_Position", GameLight_Lightpos, GameLight_Count);
        sfShader_setVec3UniformArray(Level.BG_Texture_Shader, "Kegel_Position", GameLight_Conepos, GameLight_Count);
        sfShader_setFloatUniformArray(Level.BG_Texture_Shader, "Licht_Kegel_Winkel", GameLight_Coneangle, GameLight_Count);
        sfShader_setFloatUniformArray(Level.BG_Texture_Shader, "Licht_Aktiv", GameLight_Active, GameLight_Count);
        sfShader_setIntUniform(Level.BG_Texture_Shader, "Anzahl_Lichter", GameLight_Count);
    }

    return true;
}

// Quit Lights
void CSFMLQuitLights()
{
    // Clear Stuff
    free(GameLight_Ambientpower);
    free(GameLight_Coneactive);
    free(GameLight_Coneangle);
    free(GameLight_Conepos);
    free(GameLight_FBMactive);
    free(GameLight_Lightpos);
    free(GameLight_ID);
    free(GameLight_Active);
    GameLight_Ambientpower = NULL;
    GameLight_Coneactive = NULL;
    GameLight_Coneangle = NULL;
    GameLight_Conepos = NULL;
    GameLight_FBMactive = NULL;
    GameLight_Lightpos = NULL;
    GameLight_ID = NULL;
    GameLight_Active = NULL;

    // Reset general things
    GameLight_Is_Init = false;
    GameLight_Viewport_Setted = false;
    GameLight_Count = 0;
}

// Set Viewport Light
bool CSFMLSetViewportLight()
{
    if(!GameLight_Is_Init || GameLight_Viewport_Setted)
        return false;


    // Setup Viewport Light, it is set at 0
    sfGlslVec3 light_pos = {0.0f, 1.0f, 0.75f};
    GameLight_Ambientpower[0] = 0.005f;
    GameLight_Coneactive[0] = 0.0f;
    GameLight_Coneangle[0] = 0.0f;
    GameLight_FBMactive[0] = 0.0f;
    GameLight_Lightpos[0] = light_pos;
    GameLight_Conepos[0] = light_pos;
    GameLight_ID[0] = 0;
    GameLight_Active[0] = 1.0f;
    GameLight_Count++;

    // Setup general things
    GameLight_Viewport_Setted = true;

    return true;
}

// Set Light
bool CSFMLSetLight(int light_x, int light_y, int cone_x, int cone_y, float ambientpower, bool coneactive, bool fbmactive, float coneangle, int id, bool active)
{
    if(!GameLight_Is_Init)
        return false;

    // Setup Lights
    if(GameLight_Count <= 9)
    {
        // Is the ID already insert ?
        size_t element = 0;
        bool Is_Insert = false;
        size_t i;
        for(i = 0; i<GameLight_Count; i++)
        {
            if(GameLight_ID[i] == id)
            {
                element = i;
                Is_Insert = true;
            }
        }
        if(!Is_Insert)
        {
            if(GameLight_Count == 0)
                element = 1;
            else
                element = GameLight_Viewport_Setted ? GameLight_Count : (GameLight_Count + 1);

            GameLight_Count++;
        }

        // Setup Light
        sfGlslVec3 light_pos = {(float)light_x / SCREEN_WIDTH, 1.0f - (float)light_y / SCREEN_HEIGHT, 0.0005f};
        sfGlslVec3 cone_pos = {(float)cone_x / SCREEN_WIDTH, 1.0f - (float)cone_y / SCREEN_HEIGHT, 0.0005f};
        GameLight_Ambientpower[element] = ambientpower;
        GameLight_Coneactive[element] = coneactive ? 1.0f : 0.0f;
        GameLight_Coneangle[element] = coneangle;
        GameLight_FBMactive[element] = fbmactive ? 1.0f : 0.0f;
        GameLight_Lightpos[element] = light_pos;
        GameLight_Conepos[element] = cone_pos;
        GameLight_ID[element] = id;
        GameLight_Active[element] = active ? 1.0f : 0.0f;

        return true;
    }
    else
    {
        printf("Game Subsystem Fehler 'GameLights': Maximale Anzahl an Lichter erreicht!\n");
        return false;
    }
}

