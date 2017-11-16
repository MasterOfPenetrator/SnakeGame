#include "CSFMLMenu.h"

bool CSFMLInit()
{
    // Subsysteme initialisieren
    sfVideoMode mode = {SCREEN_WIDTH, SCREEN_HEIGHT, 32};
    screen = sfRenderWindow_create(mode, "Snake Game", sfClose, NULL);

    if(screen == NULL)
    {
        printf("Video Subsystem Fehler: Kann CSFML nicht initialisieren.\n");
        return false;
    }

    // FPS Timer und Font initialisieren
    clck = sfClock_create();
    if(clck == NULL)
    {
        printf("Video Subsystem Fehler: Kann FPS Timer nicht initialisieren.\n");
        return false;
    }

    fps_font = sfFont_createFromFile("Bilder/Schriftarten/3Dventure.ttf");
    if(fps_font == NULL)
    {
        printf("Video Subsystem Fehler: Kann FPS Font nicht laden!\n");
        return false;
    }

    fps_text = sfText_create();
    if(fps_text == NULL)
    {
        printf("Video Subsystem Fehler: Kann FPS Text nicht laden!\n");
        return false;
    }

    // Bilder laden
    // Speicher für Buttons und Hintergründe reservieren
    media.buttons = malloc(11 * sizeof(mButton));
    media.bg = malloc (4 * sizeof(mBackground));

    if(media.buttons == NULL || media.bg == NULL)
    {
        printf("Video Subsystem Fehler: Kann keinen Speicher fuer Media initialisieren!\n");
        return false;
    }

    // Sonstige Medien Dateien laden
    media.header = sfTexture_createFromFile("Bilder/Menu/MenuTitleBorder.png", NULL);
    media.mega_font = sfFont_createFromFile("Bilder/Schriftarten/baby blocks.ttf");
    media.spezial_font = sfFont_createFromFile("Bilder/Schriftarten/3Dventure.ttf");
    media.normal_font = sfFont_createFromFile("Bilder/Schriftarten/SEASERN.ttf");

    if(media.header == NULL || media.mega_font == NULL || media.normal_font == NULL || media.spezial_font == NULL)
    {
        printf("Video Subsystem Fehler: Kann Sonstige Medien Dateien nicht laden!\n");
        return false;
    }


    // Buttons und Hintergründe laden
    media.bg[0].background = sfTexture_createFromFile("Bilder/Menu/MenuGameBorder.png", NULL);
    media.bg[1].background = sfTexture_createFromFile("Bilder/Menu/MenuMainBorder.png", NULL);
    media.bg[2].background = sfTexture_createFromFile("Bilder/Menu/MenuScoresBorder.png", NULL);
    media.bg[3].background = sfTexture_createFromFile("Bilder/Menu/MenuSettingsBorder.png", NULL);

    media.buttons[0].normal_button = sfTexture_createFromFile("Bilder/Menu/BeendenButtonNormal.png", NULL);
    media.buttons[0].click_button = sfTexture_createFromFile("Bilder/Menu/BeendenButtonClicked.png", NULL);
    media.buttons[0].over_button = sfTexture_createFromFile("Bilder/Menu/BeendenButtonOnMouseOver.png", NULL);
    media.buttons[1].normal_button = sfTexture_createFromFile("Bilder/Menu/DefaultButtonNormal.png", NULL);
    media.buttons[1].click_button = sfTexture_createFromFile("Bilder/Menu/DefaultButtonClicked.png", NULL);
    media.buttons[1].over_button = sfTexture_createFromFile("Bilder/Menu/DefaultButtonOnMouseOver.png", NULL);
    media.buttons[2].normal_button = sfTexture_createFromFile("Bilder/Menu/SaveButtonNormal.png", NULL);
    media.buttons[2].click_button = sfTexture_createFromFile("Bilder/Menu/SaveButtonClicked.png", NULL);
    media.buttons[2].over_button = sfTexture_createFromFile("Bilder/Menu/SaveButtonOnMouseOver.png", NULL);
    media.buttons[3].normal_button = sfTexture_createFromFile("Bilder/Menu/ScoresButtonNormal.png", NULL);
    media.buttons[3].click_button = sfTexture_createFromFile("Bilder/Menu/ScoresButtonClicked.png", NULL);
    media.buttons[3].over_button = sfTexture_createFromFile("Bilder/Menu/ScoresButtonOnMouseOver.png", NULL);
    media.buttons[4].normal_button = sfTexture_createFromFile("Bilder/Menu/SettingsButtonNormal.png", NULL);
    media.buttons[4].click_button = sfTexture_createFromFile("Bilder/Menu/SettingsButtonClicked.png", NULL);
    media.buttons[4].over_button = sfTexture_createFromFile("Bilder/Menu/SettingsButtonOnMouseOver.png", NULL);
    media.buttons[5].normal_button = sfTexture_createFromFile("Bilder/Menu/SpielenButtonNormal.png", NULL);
    media.buttons[5].click_button = sfTexture_createFromFile("Bilder/Menu/SpielenButtonClicked.png", NULL);
    media.buttons[5].over_button = sfTexture_createFromFile("Bilder/Menu/SpielenButtonOnMouseOver.png", NULL);
    media.buttons[6].normal_button = sfTexture_createFromFile("Bilder/Menu/VorButtonNormal.png", NULL);
    media.buttons[6].click_button = sfTexture_createFromFile("Bilder/Menu/VorButtonClicked.png", NULL);
    media.buttons[6].over_button = sfTexture_createFromFile("Bilder/Menu/VorButtonOnMouseOver.png", NULL);
    media.buttons[7].normal_button = sfTexture_createFromFile("Bilder/Menu/ZurButtonNormal.png", NULL);
    media.buttons[7].click_button = sfTexture_createFromFile("Bilder/Menu/ZurButtonClicked.png", NULL);
    media.buttons[7].over_button = sfTexture_createFromFile("Bilder/Menu/ZurButtonOnMouseOver.png", NULL);
    media.buttons[8].normal_button = sfTexture_createFromFile("Bilder/Menu/ZurueckButtonNormal.png", NULL);
    media.buttons[8].click_button = sfTexture_createFromFile("Bilder/Menu/ZurueckButtonClicked.png", NULL);
    media.buttons[8].over_button = sfTexture_createFromFile("Bilder/Menu/ZurueckButtonOnMouseOver.png", NULL);
    media.buttons[9].normal_button = sfTexture_createFromFile("Bilder/Menu/AktivButtonNormal.png", NULL);
    media.buttons[9].click_button = sfTexture_createFromFile("Bilder/Menu/AktivButtonClicked.png", NULL);
    media.buttons[9].over_button = sfTexture_createFromFile("Bilder/Menu/AktivButtonOnMouseOver.png", NULL);
    media.buttons[10].normal_button = sfTexture_createFromFile("Bilder/Menu/AusButtonNormal.png", NULL);
    media.buttons[10].click_button = sfTexture_createFromFile("Bilder/Menu/AusButtonClicked.png", NULL);
    media.buttons[10].over_button = sfTexture_createFromFile("Bilder/Menu/AusButtonOnMouseOver.png", NULL);

    // Allgemeine Sachen
    CSFMLIsInit = true;
    shader_enabled = true;
    vsync_enabled = true;

    // FPS Sachen
    actualtime.microseconds = 0;
    lasttime.microseconds = 0;
    FPS = 0;

    // Shader Variablen
    // Buttons
    fButtonBlur = 0.0f;
    iButtonBlur = 0;
    iButtonBlur = false;

    return true;
}

bool CSFMLMenuInit()
{
    if(!CSFMLIsInit)
        return 0;

    mstate.isInit = true;
    mstate.isPress = false;
    mstate.actualsite = MAINSITE;
    mstate.prevsite = MAINSITE;
    mstate.scorepages = 1;
    mstate.actualpage = 1;
    mstate.btnCleared = true;
    mstate.sliderCleared = true;
    mstate.entryCleared = true;
    mstate.startgame = false;
    mstate.hs_written = false;
    mstate.Level_Count = 0;
    mstate.Level_Dir_Opened = false;
    mstate.Level_Selected = false;
    mstate.Level_ID = 0;
    mstate.Allow_UserEnter = false;
    mstate.Text_Field_Clicked = false;
    mstate.UserName_Counter = 0;
    mstate.Level_Username[0] = '\0';
    mstate.User_Final_Enter = false;

    // Elemente des Menüs
    MenuClearButtonStack();
    MenuClearHighScoreStack();
    MenuClearSettingStack();
    MenuClearSliderStack();
    MenuSetButtonStack();
    MenuSetHighScoreStack();
    MenuSetSettingStack();
    MenuSetSliderStack();

    // Elemente vorab platzieren und unsichtbar machen
    sfColor grey = {77,77,77,255};
    sfColor other = {254,84,0,255};
    // MainSite
    MenuPlaceButton(0, 5, 100, 230, true, GAMESITE_LEVEL, MAINSITE, false);
    MenuPlaceButton(0, 3, 100, 330, true, SCORESITE, MAINSITE, false);
    MenuPlaceButton(0, 4, 100, 430, true, SETTINGSSITE, MAINSITE, false);
    MenuPlaceButton(0, 0, 100, 530, true, EXITSITE, MAINSITE, false);

    // ScoreSites
    MenuPlaceButton(0, 8, 480, 630, true, MAINSITE, SCORESITE, false);
    MenuPlaceButton(0, 8, 480, 630, true, MAINSITE, SCORESITE_NEXT, false);
    MenuPlaceButton(0, 8, 480, 630, true, MAINSITE, SCORESITE_BACK, false);
    ReadHighScore(&mstate.entries);

    // SettingsSites
    ReadSetting();
    MenuPlaceButton(0, 2, 280, 630, true, SETTINGSAVE, SETTINGSSITE, false);
    MenuPlaceButton(0, 1, 380, 630, true, SETTINGDEF, SETTINGSSITE, false);
    MenuPlaceButton(0, 8, 480, 630, true, MAINSITE, SETTINGSSITE, false);
    MenuPlaceSlider(260, 225, 300, 40, sfWhite, grey, other, 0, SETTINGSSITE, false);
    MenuPlaceSlider(260, 275, 300, 40, sfWhite, grey, other, 1, SETTINGSSITE, false);

    // GameSite
    MenuPlaceButton(0, 8, 480, 630, true, MAINSITE, GAMESITE_LEVEL, false);
    MenuPlaceButton(0, 8, 480, 630, true, MAINSITE, GAMESITE_USER, false);
    MenuPlaceButton(0, 8, 480, 630, true, MAINSITE, GAMESITE_END, false);


    // Shader Effekte Aktiv ?
    size_t i;
    for(i = 0; i<mstate.set_stack; i++)
    {
        if(mstate.setting[i].effect == 2)
        {
            // Shader Effekte Aktiv
            if(mstate.setting[i].actual_value == 1)
            {
                shader_enabled = true;
            }
            // Shader Effeke inaktiv
            else
            {
                shader_enabled = false;
            }
        }
    }

    MenuPlaceButton(0, 9, 480, 335, true, SETTING_SHAD, SETTINGSSITE, (shader_enabled) ? false : true);
    MenuPlaceButton(0, 10, 480, 335, true, SETTING_SHAD, SETTINGSSITE, (shader_enabled) ? true : false);


    // Vsync Aktiv ?
    for(i = 0; i<mstate.set_stack; i++)
    {
        if(mstate.setting[i].effect == 3)
        {
            // Shader Effekte Aktiv
            if(mstate.setting[i].actual_value == 1)
            {
                vsync_enabled = true;
            }
            // Shader Effeke inaktiv
            else
            {
                vsync_enabled = false;
            }
        }
    }

    MenuPlaceButton(0, 9, 480, 385, true, SETTING_VSYNC, SETTINGSSITE, (vsync_enabled) ? false : true);
    MenuPlaceButton(0, 10, 480, 385, true, SETTING_VSYNC, SETTINGSSITE, (vsync_enabled) ? true : false);

    // Vsync nach Einstellungen aktivieren
    sfRenderWindow_setVerticalSyncEnabled(screen, vsync_enabled);

    return true;
}

void CSFMLMenuQuit()
{
    // Menu States löschen
    MenuClearSliderStack();
    MenuClearButtonStack();
    MenuClearHighScoreStack();
    MenuClearSettingStack();

    // Level names löschen!
    size_t i = 0;
    for(i = 0; i<mstate.Level_Count; i++)
    {
        free(mstate.Level_Names[i]);
        mstate.Level_Names = NULL;
    }

    free(mstate.Level_Names);
    mstate.Level_Names = NULL;

    mstate.hs_written = false;
}

void CSFMLQuit()
{
    // Bildschirm reinigen
    CSFMLCLRSCR();

    // Media löschen
    sfFont_destroy(media.mega_font);
    sfFont_destroy(media.normal_font);
    sfFont_destroy(media.spezial_font);
    sfTexture_destroy(media.header);
    media.mega_font = NULL;
    media.normal_font = NULL;
    media.spezial_font = NULL;
    media.header = NULL;

    int s;
    for(s = 0; s<4; s++)
    {
        sfTexture_destroy(media.bg[s].background);
        media.bg[s].background = NULL;
    }
    free(media.bg);
    media.bg = NULL;

    int i;
    for(i = 0; i<9; i++)
    {
        sfTexture_destroy(media.buttons[i].click_button);
        media.buttons[i].click_button = NULL;
        sfTexture_destroy(media.buttons[i].normal_button);
        media.buttons[i].normal_button = NULL;
        sfTexture_destroy(media.buttons[i].over_button);
        media.buttons[i].over_button = NULL;
    }
    free(media.buttons);
    media.buttons = NULL;

    // Allgemein
    sfClock_destroy(clck);
    clck = NULL;
    sfFont_destroy(fps_font);
    fps_font = NULL;
    sfText_destroy(fps_text);
    fps_text = NULL;
    sfRenderWindow_destroy(screen);
    screen = NULL;
}

void CSFMLCLRSCR()
{
    if(mstate.isInit)
    {
        sfColor black = {0, 0, 0, 0};
        sfRenderWindow_clear(screen, black);
    }
}

void CSFMLShowFPS()
{
    sfVector2f position = {20, 163};
    sfColor color = {233, 77, 0, 255};

    actualtime = sfClock_getElapsedTime(clck);
    FPS = 1000000.f/(actualtime.microseconds - lasttime.microseconds);
    lasttime = actualtime;

    sfText_setPosition(fps_text, position);
    sfText_setFont(fps_text, fps_font);
    sfText_setCharacterSize(fps_text, 20);
    sfText_setColor(fps_text, color);

    char fps_string[50] = {0};
    char start[6] = "FPS: ";
    char value[6];
    strcat(fps_string, start);
    itoa((int)FPS, value, 10);
    strcat(fps_string, value);

    sfText_setString(fps_text, fps_string);

    sfRenderWindow_drawText(screen, fps_text, NULL);
}

bool MenuPlaceRect(Rect r, sfColor color, bool border, sfColor bordercolor, int thickness)
{
    if(!CSFMLIsInit || !mstate.isInit)
        return false;

    // Zeichnen
    sfRectangleShape *rect = sfRectangleShape_create();
    sfRectangleShape_setPosition(rect, r.pos);
    sfRectangleShape_setSize(rect, r.siz);
    sfRectangleShape_setFillColor(rect, color);
    if(border)
    {
        sfRectangleShape_setOutlineThickness(rect, thickness);
        sfRectangleShape_setOutlineColor(rect, bordercolor);
    }
    sfRenderWindow_drawRectangleShape(screen, rect, NULL);

    // Aufräumen
    sfRectangleShape_destroy(rect);
    rect = NULL;

    return true;
}

bool MenuPlaceIMG(int x, int y, bool type, int id)
{
    // Initialisieren
    if(!CSFMLIsInit || !mstate.isInit)
        return false;

    sfSprite *sprite = sfSprite_create();

    if(sprite == NULL)
    {
        printf("Sprite kann nicht geladen werden!\n");
        return false;
    }

    sfVector2f position = {x, y};
    sfSprite_setPosition(sprite, position);

    // Bild zeichnen
    if(type == false)
        sfSprite_setTexture(sprite, media.header, sfTrue);
    else
        sfSprite_setTexture(sprite, media.bg[id].background, sfTrue);

    sfRenderWindow_drawSprite(screen, sprite, NULL);

    // Speicher freigeben
    sfSprite_destroy(sprite);
    sprite = NULL;

    return true;
}

bool MenuPlaceButton(int state, int id, int x, int y, bool clickable, MENUSITES target, MENUSITES destination, bool visible)
{
    // Initialisieren
    if(!CSFMLIsInit || !mstate.isInit || mstate.btnCleared)
        return false;

    sfVector2f position = {x, y};
    sfVector2u size;

    sfSprite *sprite = sfSprite_create();
    sfSprite_setPosition(sprite, position);


    // Welchen Status soll der Button haben?
    if(state == 0) // Normal
    {
        size = sfTexture_getSize(media.buttons[id].normal_button);
        sfSprite_setTexture(sprite, media.buttons[id].normal_button, sfTrue);
    }
    else if(state == 1) // OnMouseOver
    {
        size = sfTexture_getSize(media.buttons[id].over_button);
        sfSprite_setTexture(sprite, media.buttons[id].over_button, sfTrue);
    }
    else if(state == 3)
    {
        size = sfTexture_getSize(media.buttons[id].click_button);
        sfSprite_setTexture(sprite, media.buttons[id].click_button, sfTrue);
    }


    // Zeichnen
    if(visible)
    {
        sfRenderWindow_drawSprite(screen, sprite, NULL);
    }

    // Buttons in Stack ablegen
    Rect imgrect = {{(float)position.x,(float)position.y}, {(float)size.x, (float)size.y}};
    if(mstate.btn_stack == 0)
    {
        // Allgemeine Sachen
        mstate.btn[0].coordinates = imgrect;
        mstate.btn[0].target = target;
        mstate.btn[0].clickable = clickable;
        mstate.btn[0].recentpress = false;
        mstate.btn[0].buttonID = id;
        mstate.btn[0].destination = destination;
        mstate.btn[0].visible = visible;
        mstate.btn_stack++;
    }
    else
    {
        // Überprüfe ob der Button schon auf den Stack liegt
        size_t s;
        bool placeit = true;
        for(s = 0; s<mstate.btn_stack; s++)
        {
            if(mstate.btn[s].buttonID == id && mstate.btn[s].destination == destination && mstate.btn[s].target == target)
                placeit = false;
        }

        // Wenn Button noch nicht draufliegt, platziere ihn
        if(placeit)
        {
            mstate.btn_stack++;
            mstate.btn = realloc(mstate.btn, mstate.btn_stack * sizeof(BUTTONS));
            if(mstate.btn == NULL)
            {
                printf("Video Subsystem - MenuPlaceButton - Fehler: Kann zusaetzlichen Speicher nicht allozieren.\n");
                return false;
            }
            else
            {
                mstate.btn[mstate.btn_stack-1].coordinates = imgrect;
                mstate.btn[mstate.btn_stack-1].target = target;
                mstate.btn[mstate.btn_stack-1].clickable = clickable;
                mstate.btn[mstate.btn_stack-1].recentpress = false;
                mstate.btn[mstate.btn_stack-1].buttonID = id;
                mstate.btn[mstate.btn_stack-1].destination = destination;
                mstate.btn[mstate.btn_stack-1].visible = visible;
            }
        }
    }

    // Aufräumen
    sfSprite_destroy(sprite);
    sprite = NULL;

    return true;
}

bool MenuPlaceText(const char *txt, int x, int y, unsigned int txtsize, sfColor textcolor)
{
    // Initialisieren
    if(!CSFMLIsInit || !mstate.isInit)
        return false;

    sfVector2f pos = {x,y};

    sfText *text = sfText_create();
    sfText_setPosition(text, pos);
    sfText_setString(text, txt);
    sfText_setFont(text, media.normal_font);
    sfText_setCharacterSize(text, txtsize);
    sfText_setColor(text, textcolor);

    // Text Zeichnen
    sfRenderWindow_drawText(screen, text, NULL);

    // Speicher freigeben

    sfText_destroy(text);
    text = NULL;

    return true;
}

void MenuButtonBlurShaderMix()
{
    if(iButtonBlur < 35 && !iButtonBlur_end)
    {
        iButtonBlur++;
    }
    else if(iButtonBlur <= 35 && iButtonBlur > 0 && iButtonBlur_end)
    {
        iButtonBlur--;
    }
    else if(iButtonBlur <= 35 && !iButtonBlur_end)
    {
        iButtonBlur_end = true;
    }
    else if(iButtonBlur <= 0 && iButtonBlur_end)
    {
        iButtonBlur_end = false;
    }

    fButtonBlur = (float) iButtonBlur / 1500;
}

bool MenuUpdateButton(int state, int buttonid, int stack_id, bool hover_effect)
{
    // Initialisieren
    if(state < 0 || buttonid < 0 || stack_id < 0 || !mstate.btn[stack_id].visible)
        return false;

    // Sprite erzeugen
    sfVector2f position = mstate.btn[stack_id].coordinates.pos;
    sfSprite *sprite = sfSprite_create();
    sfSprite_setPosition(sprite, position);
    sfShader *shader = sfShader_createFromFile(NULL, NULL, "Bilder/Shader/Menu_Button_Blur_Effect.frag");

    // Shader-Effekte auf Buttons
    if(shader_enabled)
    {
        // Shader erzeugen
        if(shader == NULL)
        {
            printf("Shader in MenuUpdateButton kann nicht geladen werden!\n");
            return false;
        }
        MenuButtonBlurShaderMix();
        sfShader_setTextureUniform(shader, "texture", media.buttons[buttonid].over_button);
        sfShader_setFloatUniform(shader, "blur", fButtonBlur);

        sfRenderStates stat;

        stat.blendMode = sfBlendNone;
        stat.shader = shader;
        stat.texture = media.buttons[buttonid].over_button;
        stat.transform = sfTransform_Identity;


        if(state == 0 || state == 2)
        {
            if(state == 0)
                sfSprite_setTexture(sprite, media.buttons[buttonid].normal_button, sfTrue);
            else if(state == 2)
                sfSprite_setTexture(sprite, media.buttons[buttonid].click_button, sfTrue);

            sfRenderWindow_drawSprite(screen, sprite, NULL);
        }
        else if(state == 1 && hover_effect)
        {
            sfSprite_setTexture(sprite, media.buttons[buttonid].over_button, sfTrue);
            sfRenderWindow_drawSprite(screen, sprite, &stat);
        }
        else if(state == 1 && !hover_effect)
        {
            sfSprite_setTexture(sprite, media.buttons[buttonid].over_button, sfTrue);
            sfRenderWindow_drawSprite(screen, sprite, NULL);
        }
    }
    // Buttons ohne Shader Einstellung
    else
    {
        if(state == 0)
            sfSprite_setTexture(sprite, media.buttons[buttonid].normal_button, sfTrue);
        else if(state == 1)
            sfSprite_setTexture(sprite, media.buttons[buttonid].over_button, sfTrue);
        else if(state == 0)
            sfSprite_setTexture(sprite, media.buttons[buttonid].click_button, sfTrue);

        sfRenderWindow_drawSprite(screen, sprite, NULL);
    }

    sfSprite_destroy(sprite);
    sprite = NULL;

    sfShader_destroy(shader);
    shader = NULL;

    return true;
}

bool MenuPlacePopup(const char *text, MENUSITES target, MENUSITES destination, bool visible)
{
    if(!CSFMLIsInit || !mstate.isInit)
        return false;

    // Platziere etwa mittig zum Fenster ein kleines schwarzen Rechteck
    Rect box = {{SCREEN_WIDTH/2 - SCREEN_WIDTH/4, SCREEN_HEIGHT/2}, {SCREEN_WIDTH/2, 150}};
    MenuPlaceRect(box, sfBlack, true, sfWhite, 5);

    // Platziere Text Perfetto
    MenuPlaceText(text, box.pos.x+50, box.pos.y+30, 13, sfWhite);

    // Platziere den genialen OK Button ...
    MenuPlaceButton(0, 8, box.pos.x+100, box.pos.y+box.siz.y-70, true, target, destination, visible);

    return true;

}


bool MenuPlaceSlider(int x, int y, int w, int h, sfColor sb_color, sfColor sn_color, sfColor sh_color, int effect, MENUSITES destination, bool visible)
{
    // Initialisieren
    if(!CSFMLIsInit || !mstate.isInit || mstate.sliderCleared || mstate.setCleared)
        return false;

    // Versuche aktuellen Wert auszulesen
    size_t count;
    int val = 0;

    for(count = 0; count<mstate.set_stack; count++)
    {
        if(mstate.setting[count].effect == effect)
        {
            if(mstate.setting[count].actual_value > 100 || mstate.setting[count].actual_value <= 0)
            {
                if(mstate.setting[count].effect == 0)
                    val = 50;
                if(mstate.setting[count].effect == 1)
                    val = 50;
            }
            else
            {
                val = mstate.setting[count].actual_value;
            }
        }
    }

    Rect slider, scrollbar = {{x, y}, {w,h}};

    // Position des Sliders berechnen
    int posx = w * ((float)val / 100);

    if(x+posx > x+w-10)
        slider.pos.x = x+w-20;
    else
        slider.pos.x = x+posx-15;

    slider.pos.y = y+5;
    slider.siz.y = h-10;
    slider.siz.x = h-10;

    if(visible)
    {
        MenuPlaceRect(scrollbar, sb_color, false, sfWhite, 0);
        MenuPlaceRect(slider, sn_color, true, sfWhite, 0);
    }

    // Slider auf den Stack ablegen
    if(mstate.sld_stack == 0)
    {
        mstate.sliders[0].actualvalue = val;
        mstate.sliders[0].scrollbar = scrollbar;
        mstate.sliders[0].slider = slider;
        mstate.sliders[0].effect = effect;
        mstate.sliders[0].scrollbar_color = sb_color;
        mstate.sliders[0].slidernormal_color = sn_color;
        mstate.sliders[0].sliderpress_color = sh_color;
        mstate.sliders[0].recentpress = false;
        mstate.sliders[0].clickable = true;
        mstate.sliders[0].destination = destination;
        mstate.sliders[0].visible = visible;
        mstate.sld_stack++;
    }
    else
    {

        // Überprüfe ob der Slider schon auf den Stack liegt
        size_t s;
        bool placeit = true;
        for(s = 0; s<mstate.sld_stack; s++)
        {
            if(mstate.sliders[s].effect == effect && mstate.sliders[s].destination == destination)
                placeit = false;
        }

        // Wenn Button noch nicht draufliegt, platziere ihn
        if(placeit)
        {
            mstate.sld_stack++;
            mstate.sliders = realloc(mstate.sliders, mstate.sld_stack * sizeof(SLIDER));

            if(mstate.sliders == NULL)
            {
                printf("Video Subsystem - MenuPlaceSlider - Fehler: Kann zusaetzlichen Speicher nicht allozieren.\n");
                return false;
            }
            else
            {
                mstate.sliders[mstate.sld_stack-1].actualvalue = val;
                mstate.sliders[mstate.sld_stack-1].scrollbar = scrollbar;
                mstate.sliders[mstate.sld_stack-1].slider = slider;
                mstate.sliders[mstate.sld_stack-1].effect = effect;
                mstate.sliders[mstate.sld_stack-1].scrollbar_color = sb_color;
                mstate.sliders[mstate.sld_stack-1].slidernormal_color = sn_color;
                mstate.sliders[mstate.sld_stack-1].sliderpress_color = sh_color;
                mstate.sliders[mstate.sld_stack-1].recentpress = false;
                mstate.sliders[mstate.sld_stack-1].clickable = true;
                mstate.sliders[mstate.sld_stack-1].destination = destination;
                mstate.sliders[mstate.sld_stack-1].visible = visible;
            }
        }
    }

    return true;
}

bool MenuPlaceHighScore(bool visible)
{
    // Initialisieren
    if(!CSFMLIsInit || !mstate.isInit || mstate.btnCleared || mstate.entryCleared)
        return false;

    // Variablen
    size_t s, t;

    char actualtext[100] = {0};
    char platzierung[10];
    const char *trenner1 = ". ";
    char punkte[15];
    const char *trenner2 = " mit ";
    const char *trenner3 = " Punkten ";

    if(mstate.entries != NULL && mstate.entry_stack > 0)
    {
        // Einträge der größe nach sortieren
        for(s = 0; s<mstate.entry_stack-1; s++)
        {
            for(t = 0; t<mstate.entry_stack-s-1; t++)
            {
                if(mstate.entries[t].score < mstate.entries[t+1].score)
                {
                    int temp = mstate.entries[t].score;
                    char ctemp[strlen(mstate.entries[t].owner)+1];
                    strcpy(ctemp, mstate.entries[t].owner);
                    mstate.entries[t].score = mstate.entries[t+1].score;
                    strcpy(mstate.entries[t+1].owner, mstate.entries[t].owner);
                    mstate.entries[t+1].score = temp;
                    strcpy(mstate.entries[t+1].owner, ctemp);

                }
            }
        }

        // Anzahl der Seiten berechnen.
        mstate.scorepages = 1 + (mstate.entry_stack / ENTRYS_PER_PAGE);
        size_t start = (mstate.actualpage-1) * ENTRYS_PER_PAGE;
        size_t end = start + ENTRYS_PER_PAGE;
        if(end > mstate.entry_stack)
        {
            end = mstate.entry_stack;
        }

        // Buttons vor und zurueck platzieren je nach dem
        if(visible)
        {

            if(mstate.scorepages > 1)
            {
                // Nur Button Weiter platzieren
                if(mstate.actualpage == 1)
                {
                    // Vor
                    MenuPlaceButton(0, 6, 380, 630, true, SCORESITE_NEXT, mstate.actualsite, true);
                    MenuPlaceButton(0, 7, 280, 630, true, SCORESITE_BACK, mstate.actualsite, false);
                }
                // Nur Button Zurück platzieren
                else if(mstate.actualpage == mstate.scorepages)
                {
                    // Zur
                    MenuPlaceButton(0, 6, 380, 630, true, SCORESITE_NEXT, mstate.actualsite, false);
                    MenuPlaceButton(0, 7, 280, 630, true, SCORESITE_BACK, mstate.actualsite, true);
                }
                // Beide Buttons platzieren
                else
                {
                   // Beide
                   MenuPlaceButton(0, 6, 380, 630, true, SCORESITE_NEXT, mstate.actualsite, true);
                   MenuPlaceButton(0, 7, 280, 630, true, SCORESITE_BACK, mstate.actualsite, true);
                }
            }

            int i = 0;
            for(s = start; s<end; s++)
            {
                itoa((s+1), platzierung, 10);
                itoa(mstate.entries[s].score, punkte, 10);
                strcat(actualtext, "");
                strcat(actualtext, platzierung);
                strcat(actualtext, trenner1);
                strcat(actualtext, mstate.entries[s].owner);
                strcat(actualtext, trenner2);
                strcat(actualtext, punkte);
                strcat(actualtext, trenner3);
                sfColor color = {244, 185, 0, 255};
                MenuPlaceText(actualtext, 20, 220+(i*30), 20, color);
                strcpy(actualtext, "");
                i++;
            }
        }
    }
    else
    {
        sfColor White = {255,255,255,255};
        if(visible)
            MenuPlaceText("Keine Aktuellen Daten vorhanden!", 90, 220, 20, White);
    }

    return true;
}

bool MenuSetHighScoreStack()
{
    if(mstate.entryCleared)
    {
        size_t count = GetHighScoreEntries();
        mstate.entries = malloc(count * sizeof(HIGHSCORE));
        if(mstate.entries == NULL)
        {
            printf("Video Subsystem - MenuSetHighScoreStack - Fehler: Kann Speicher nicht allozieren.\n");
            return false;
        }
        else
        {
            mstate.entry_stack = count;
            mstate.entryCleared = false;
        }
    }

    return true;
}

void MenuClearHighScoreStack()
{
    free(mstate.entries);
    mstate.entries = NULL;
    mstate.entry_stack = 0;
    mstate.entryCleared = true;
}

bool MenuSetSettingStack()
{
    if(mstate.setCleared)
    {
        size_t count = GetSettingsEntries();
        mstate.setting = malloc(count * sizeof(SETTINGS));
        if(mstate.setting == NULL)
        {
            printf("Video Subsystem - MenuSetSettingStack - Fehler: Kann Speicher nicht allozieren.\n");
            return false;
        }
        else
        {
            mstate.set_stack = count;
            mstate.setCleared = false;
        }
    }

    return true;
}

void MenuClearSettingStack()
{
    free(mstate.setting);
    mstate.setting = NULL;
    mstate.set_stack = 0;
    mstate.setCleared = true;
}

bool MenuSetSliderStack()
{
    if(mstate.sliderCleared)
    {
        mstate.sliders = malloc(sizeof(SLIDER));
        if(mstate.sliders == NULL)
        {
            printf("Video Subsystem - MenuSetSliderStack - Fehler: Kann Speicher nicht allozieren.\n");
            return false;
        }
        else
        {
            mstate.sld_stack = 0;
            mstate.sliderCleared = false;
        }
    }

    return true;
}

void MenuClearSliderStack()
{
    free(mstate.sliders);
    mstate.sliders = NULL;
    mstate.sld_stack = 0;
    mstate.sliderCleared = true;
}

bool MenuSetButtonStack()
{
    if(mstate.btnCleared)
    {
        mstate.btn = malloc(sizeof(BUTTONS));
        if(mstate.btn == NULL)
        {
            printf("Video Subsystem - MenuSetButtonStack - Fehler: Kann Speicher nicht allozieren.\n");
            return false;
        }
        else
        {
            mstate.btn_stack = 0;
        mstate.btnCleared = false;
        }
    }

    return true;
}

void MenuClearButtonStack()
{
    free(mstate.btn);
    mstate.btn = NULL;
    mstate.btn_stack = 0;
    mstate.btnCleared = true;
}

bool MenuProcessButtons()
{
    if(mstate.btnCleared || !mstate.isInit)
        return false;

    // Überprüfe ob Maus auf allen Buttons liegt
    // state 0: Normal
    // state 1: OnMouseOver
    // state 2: Pressed
    // state 3: Pressed & Released
    size_t i;

    for(i = 0; i<mstate.btn_stack; i++)
    {
        if(mstate.btn[i].clickable && mstate.btn[i].destination == mstate.actualsite && mstate.btn[i].visible)
        {
            if(!(mstate.mouse_pos.x < mstate.btn[i].coordinates.pos.x ||
               mstate.mouse_pos.y < mstate.btn[i].coordinates.pos.y ||
               mstate.mouse_pos.x >= mstate.btn[i].coordinates.pos.x + mstate.btn[i].coordinates.siz.x ||
               mstate.mouse_pos.y >= mstate.btn[i].coordinates.pos.y + mstate.btn[i].coordinates.siz.y))
            {
                if(!mstate.isPress && !mstate.btn[i].recentpress)
                {
                    if(mstate.btn[i].buttonID == 0 || mstate.btn[i].buttonID == 3 || mstate.btn[i].buttonID == 4 || mstate.btn[i].buttonID == 5)
                    {
                        MenuUpdateButton(1, mstate.btn[i].buttonID, i, true);
                    }
                    else
                    {
                        MenuUpdateButton(1, mstate.btn[i].buttonID, i, false);
                    }
                }
                else if(mstate.isPress && !mstate.btn[i].recentpress)
                {
                    MenuUpdateButton(2, mstate.btn[i].buttonID, i, false);
                    mstate.btn[i].recentpress = 1;
                }
                else if(!mstate.isPress && mstate.btn[i].recentpress)
                {
                    if(mstate.btn[i].target == SCORESITE_NEXT)
                        if(mstate.actualpage+1 <= mstate.scorepages)
                            mstate.actualpage++;

                    if(mstate.btn[i].target == SCORESITE_BACK)
                        if(mstate.actualpage-1 >= 1)
                            mstate.actualpage--;

                    mstate.prevsite = mstate.actualsite;
                    mstate.actualsite = mstate.btn[i].target;
                    mstate.btn[i].recentpress = false;
                    MenuPlace();
                }
            }
            else
            {
                MenuUpdateButton(0, mstate.btn[i].buttonID, i, false);
            }
        }
    }

    return true;
}

bool MenuProcessSliders()
{
    if(!mstate.isInit || mstate.sliderCleared)
        return false;

    // Überprüfe ob Maus auf dem Sliderknopf ist
    size_t i, s;

    for(i = 0; i<mstate.sld_stack; i++)
    {
        if(mstate.sliders[i].clickable && mstate.sliders[i].destination == mstate.actualsite)
        {
            if(!(mstate.mouse_pos.x < mstate.sliders[i].slider.pos.x ||
                 mstate.mouse_pos.y < mstate.sliders[i].slider.pos.y ||
                 mstate.mouse_pos.x >= mstate.sliders[i].slider.pos.x + mstate.sliders[i].slider.siz.x ||
                 mstate.mouse_pos.y >= mstate.sliders[i].slider.pos.y + mstate.sliders[i].slider.siz.y))
            {
                if(mstate.isPress)
                {

                    if(mstate.mouse_pos.x >= (mstate.sliders[i].scrollbar.pos.x + mstate.sliders[i].slider.siz.x) &&
                       mstate.mouse_pos.x <= (mstate.sliders[i].scrollbar.pos.x + mstate.sliders[i].scrollbar.siz.x - mstate.sliders[i].slider.siz.x))
                    {
                        mstate.sliders[i].actualvalue = 100 * ((float)(mstate.mouse_pos.x - mstate.sliders[i].scrollbar.pos.x ) / mstate.sliders[i].scrollbar.siz.x);
                        mstate.sliders[i].slider.pos.x = mstate.mouse_pos.x-(mstate.sliders[i].slider.siz.x/2);

                        for(s = 0; s<mstate.set_stack; s++)
                            if(mstate.setting[s].effect == mstate.sliders[i].effect)
                                mstate.setting[s].actual_value = mstate.sliders[i].actualvalue;
                        mstate.sliders[i].recentpress = true;
                        MenuPlaceRect(mstate.sliders[i].scrollbar, mstate.sliders[i].scrollbar_color, false, sfWhite, 0);
                        MenuPlaceRect(mstate.sliders[i].slider, mstate.sliders[i].sliderpress_color, false, sfWhite, 0);
                    }

                }
                if(!mstate.isPress && mstate.sliders[i].recentpress)
                {
                    mstate.sliders[i].recentpress = false;
                }
            }
            MenuPlaceRect(mstate.sliders[i].scrollbar, mstate.sliders[i].scrollbar_color, false, sfWhite, 0);
            MenuPlaceRect(mstate.sliders[i].slider, mstate.sliders[i].slidernormal_color, false, sfWhite, 0);
        }
    }
    return true;
}

bool MenuSetElementsVisible(MENUSITES page)
{
    // Buttons
    size_t b;
    for(b = 0; b<mstate.btn_stack; b++)
    {
        if(mstate.btn[b].destination == mstate.actualsite)
        {
            mstate.btn[b].visible = true;

            if(mstate.btn[b].buttonID == 9 && mstate.btn[b].target == SETTING_SHAD)
                mstate.btn[b].visible = (shader_enabled) ? false : true;
            if(mstate.btn[b].buttonID == 10 && mstate.btn[b].target == SETTING_SHAD)
                mstate.btn[b].visible = (shader_enabled) ? true : false;
            if(mstate.btn[b].buttonID == 9 && mstate.btn[b].target == SETTING_VSYNC)
                mstate.btn[b].visible = (vsync_enabled) ? false : true;
            if(mstate.btn[b].buttonID == 10 && mstate.btn[b].target == SETTING_VSYNC)
                mstate.btn[b].visible = (vsync_enabled) ? true : false;
        }
        else
        {
            mstate.btn[b].visible = false;
        }
    }

    // Sliders
    size_t s;
    for(s = 0; s<mstate.sld_stack; s++)
    {
        if(mstate.sliders[s].destination == page)
            mstate.sliders[s].visible = true;
        else
            mstate.sliders[s].visible = false;
    }

    if((page == SCORESITE || page == SCORESITE_BACK || page == SCORESITE_NEXT))
    {
        MenuPlaceHighScore(true);
    }


    return false;
}

void ToggleShader()
{
    size_t t;
    shader_enabled = !shader_enabled;
    for(t = 0; t<mstate.set_stack; t++)
    {
        if(mstate.setting[t].effect == 2)
        {
            mstate.setting[t].actual_value = shader_enabled;
        }
    }
}

void ToggleVsync()
{
    vsync_enabled = !vsync_enabled;
    size_t t;
    for(t = 0; t<mstate.set_stack; t++)
    {
        if(mstate.setting[t].effect == 3)
        {
            mstate.setting[t].actual_value = vsync_enabled;
            sfRenderWindow_setVerticalSyncEnabled(screen, sfFalse);
        }
    }
}

bool CSFMLPrintLevels()
{
    // Levels Preloaded?
    if(!mstate.Level_Dir_Opened)
    {
        // Get Count of Levels
        DIR *dir;
        struct dirent *entrys;

        if((dir = opendir("Bilder/Game/LevelData/")) == NULL)
        {
            printf("Menu Subsystem Fehler: Keine Levels verfuegbar!\n");
            return false;
        }

        while((entrys = readdir(dir)) != NULL)
        {
            // Ignore File Name "." and ".."
            if(strcmp(entrys->d_name, ".") && strcmp(entrys->d_name, ".."))
            {
                mstate.Level_Count++;
            }
        }

        closedir(dir);
        dir = NULL;

        if(mstate.Level_Count == 0)
        {
            printf("Menu Subsystem Fehler: Keine Level verfuegbar!\n");
            return false;
        }

        // Init Memory for Level Names
        mstate.Level_Names = malloc(mstate.Level_Count * sizeof(char *));

        if(mstate.Level_Names == NULL)
        {
            printf("Menu Subsystem Fehler: Kann Speicher fuer Level Namen nicht anlegen!\n");
            return false;
        }

        if((dir = opendir("Bilder/Game/LevelData/")) == NULL)
        {
            printf("Menu Subsystem Fehler: Keine Levels verfuegbar!\n");
            return false;
        }

        size_t counter = 0;
        while((entrys = readdir(dir)) != NULL)
        {
            // Ignore File Name "." and ".."
            if(strcmp(entrys->d_name, ".") && strcmp(entrys->d_name, ".."))
            {
                mstate.Level_Names[counter] = malloc(strlen(entrys->d_name)+1 * sizeof(char));

                if(mstate.Level_Names[counter] == NULL)
                {
                    printf("Menu Subsystem Fehler: Kann Speicher fuer Levelstring nicht anlegen!\n");
                    return false;
                }

                strcpy(mstate.Level_Names[counter], entrys->d_name);
            }
        }

        closedir(dir);
        dir = NULL;

        mstate.Level_Dir_Opened = true;
    }

    // Print Now the Levels
    sfColor Title = {255, 74, 0, 255};
    MenuPlaceText("Level Auswahl:", 100, 220, 45, Title);

    size_t i;
    for(i = 0; i<mstate.Level_Count; i++)
    {
        // Get Text
        char CompleteString[100] = {0};
        char Mapname[20];
        CSFMLPreloadMapName(mstate.Level_Names[i], Mapname, 20);
        strncat(CompleteString, mstate.Level_Names[i], strlen(mstate.Level_Names[i])+1);
        strncat(CompleteString, ": ", 2);
        strncat(CompleteString, Mapname, 20);

        // Print Button
        // Max 4 Entries per Page
        // Todo: Make it
        if(i < 4)
        {
           CSFMLPrintLevelButton(CompleteString, i);
        }
    }

    return true;
}

void CSFMLPrintLevelButton(const char *Text, size_t i)
{
    // Init Stuff
    sfText *actualtext = sfText_create();
    sfRectangleShape *shape = sfRectangleShape_create();

    // Set Text Properties
    sfText_setString(actualtext, Text);
    sfText_setFont(actualtext, media.normal_font);
    sfText_setCharacterSize(actualtext, 30);

    // Init Position & Size
    sfVector2f LocalBounds = {sfText_getLocalBounds(actualtext).width, sfText_getLocalBounds(actualtext).height};
    sfVector2f Position = {(SCREEN_WIDTH/2) - (LocalBounds.x/2), (320 + (LocalBounds.y*i)) };
    sfVector2f Position_Rect = {Position.x - 10, Position.y - 7};
    sfVector2f Size_Rect = {LocalBounds.x + 20, LocalBounds.y + 20};

    // Prepare for Click
    sfColor Tex_Col;
    if(!(mstate.mouse_pos.x < Position_Rect.x ||
                 mstate.mouse_pos.y < Position_Rect.y ||
                 mstate.mouse_pos.x >= Position_Rect.x + Size_Rect.x ||
                 mstate.mouse_pos.y >= Position_Rect.y + Size_Rect.y))
    {
        // Set Highlight Color
        Tex_Col.a = 255;
        Tex_Col.r = 254;
        Tex_Col.g = 74;
        Tex_Col.b = 0;

        // Proceed Click
        if(mstate.isPress)
        {
            // Get LevelID
            mstate.Level_ID = i +1;
            mstate.Level_Selected = true;
            mstate.Allow_UserEnter = true;
            mstate.prevsite = mstate.actualsite;
            mstate.actualsite = GAMESITE_USER;
        }
    }
    else
    {
        Tex_Col.a = 255;
        Tex_Col.r = 131;
        Tex_Col.g = 131;
        Tex_Col.b = 131;
    }
    sfColor Box_Col = {0,0,0,0};

    // Set Positions
    sfText_setPosition(actualtext, Position);
    sfRectangleShape_setPosition(shape, Position_Rect);
    sfRectangleShape_setSize(shape, Size_Rect);

    // Set Colors
    sfText_setColor(actualtext, Tex_Col);
    sfRectangleShape_setFillColor(shape, Box_Col);
    sfRectangleShape_setOutlineColor(shape, Tex_Col);
    sfRectangleShape_setOutlineThickness(shape, 5.0f);

    // Render Stuff
    sfRenderWindow_drawRectangleShape(screen, shape, NULL);
    sfRenderWindow_drawText(screen, actualtext, NULL);

    // Clear Stuff
    sfText_destroy(actualtext);
    sfRectangleShape_destroy(shape);
    shape = NULL;
    actualtext = NULL;
}

void CSFMLCLRLevelStuff()
{
    mstate.Level_ID = 0;
    mstate.Level_Selected = false;
    mstate.Allow_UserEnter = false;
    mstate.Text_Field_Clicked = false;
    mstate.User_Final_Enter = false;
    mstate.UserName_Counter = 0;

    // Fill UserName with Zeros
    size_t i;
    for(i = 0; i<20; i++)
    {
        mstate.Level_Username[i] = '\0';
    }
}

void CSFMLPrintUserEnter()
{
    // Print Text Stuff
    sfColor Title = {255, 74, 0, 255};
    sfColor Grey = {131,131,131,255};
    sfColor Black = {0,0,0,0};
    MenuPlaceText("Enter your Name", 65, 220, 45, Title);
    MenuPlaceText("Maximum Size of your Name: 20 Chars!", 50, 300, 20, sfWhite);
    MenuPlaceText("Minimum Size of your Name: 5 Chars!", 50, 340, 20, sfWhite);
    MenuPlaceText("Username", 35, 400, 25, sfWhite);

    // Setup Text Input Box and Outline Box for Text
    sfRectangleShape *Box = sfRectangleShape_create();
    sfRectangleShape *B2 = sfRectangleShape_create();
    sfText *UserEnterStuff = sfText_create();
    sfText_setFont(UserEnterStuff, media.normal_font);
    sfText_setColor(UserEnterStuff, Title);
    sfText_setCharacterSize(UserEnterStuff, 20);
    sfRectangleShape_setFillColor(Box, Black);
    sfRectangleShape_setOutlineColor(Box, Grey);
    sfRectangleShape_setOutlineThickness(Box, 3.0f);
    sfRectangleShape_setFillColor(B2, Black);
    sfRectangleShape_setOutlineColor(B2, Grey);
    sfRectangleShape_setOutlineThickness(B2, 3.0f);

    // Set Positions
    sfVector2f Size = {375, 50};
    sfVector2f Position = {195, 390};

    sfVector2f Size_2 = {155, 50};
    sfVector2f Position_2 = {30, 390};

    sfVector2f Position_Text = {200, 403};

    sfRectangleShape_setPosition(Box, Position);
    sfRectangleShape_setSize(Box, Size);

    sfRectangleShape_setPosition(B2, Position_2);
    sfRectangleShape_setSize(B2, Size_2);

    sfText_setPosition(UserEnterStuff, Position_Text);

    // Proceed User Enter
    if(!(mstate.mouse_pos.x < Position.x ||
                 mstate.mouse_pos.y < Position.y ||
                 mstate.mouse_pos.x >= Position.x + Size.x ||
                 mstate.mouse_pos.y >= Position.y + Size.y))
    {
        if(mstate.isPress)
        {
            mstate.Text_Field_Clicked = true;
        }
    }
    else
    {
        if(mstate.isPress)
        {
            mstate.Text_Field_Clicked = false;
        }
    }

    // Set Text
    sfText_setString(UserEnterStuff, mstate.Level_Username);

    // Print Button if Next Step
    if(mstate.UserName_Counter >= 5)
        CSFMLUserLevelButton("Now Go to Play!");

    // Render Stuff
    sfRenderWindow_drawText(screen, UserEnterStuff, NULL);
    sfRenderWindow_drawRectangleShape(screen, Box, NULL);
    sfRenderWindow_drawRectangleShape(screen, B2, NULL);

    // Clear Stuff
    sfRectangleShape_destroy(Box);
    sfRectangleShape_destroy(B2);
    sfText_destroy(UserEnterStuff);
    UserEnterStuff = NULL;
    Box = NULL;
    B2 = NULL;
}

void CSFMLUserLevelButton(const char *Text)
{
    // Init Stuff
    sfText *actualtext = sfText_create();
    sfRectangleShape *shape = sfRectangleShape_create();

    // Set Text Properties
    sfText_setString(actualtext, Text);
    sfText_setFont(actualtext, media.normal_font);
    sfText_setCharacterSize(actualtext, 30);

    // Init Position & Size
    sfVector2f Position = {140, 500 };
    sfVector2f Position_Rect = {Position.x - 10, Position.y - 7};
    sfVector2f Size_Rect = {335, 50};


    // Prepare for Click
    sfColor Tex_Col;
    if(!(mstate.mouse_pos.x < Position_Rect.x ||
                 mstate.mouse_pos.y < Position_Rect.y ||
                 mstate.mouse_pos.x >= Position_Rect.x + Size_Rect.x ||
                 mstate.mouse_pos.y >= Position_Rect.y + Size_Rect.y))
    {
        // Set Highlight Color
        Tex_Col.a = 255;
        Tex_Col.r = 254;
        Tex_Col.g = 74;
        Tex_Col.b = 0;

        // Proceed Click
        if(mstate.isPress)
        {
            // Go to next Page
            mstate.prevsite = mstate.actualsite;
            mstate.actualsite = GAMESITE_PLAY;
            MenuPlace();

            // Setup Game Variables
            mstate.Level_Username[mstate.UserName_Counter] = '\0';
            strncpy(GameSnake.S_Name, mstate.Level_Username, strlen(mstate.Level_Username)+1);
            GameMain.GM_Level_ID = mstate.Level_ID;
        }
    }
    else
    {
        Tex_Col.a = 255;
        Tex_Col.r = 131;
        Tex_Col.g = 131;
        Tex_Col.b = 131;
    }
    sfColor Box_Col = {0,0,0,0};

    // Set Positions
    sfText_setPosition(actualtext, Position);
    sfRectangleShape_setPosition(shape, Position_Rect);
    sfRectangleShape_setSize(shape, Size_Rect);

    // Set Colors
    sfText_setColor(actualtext, Tex_Col);
    sfRectangleShape_setFillColor(shape, Box_Col);
    sfRectangleShape_setOutlineColor(shape, Tex_Col);
    sfRectangleShape_setOutlineThickness(shape, 5.0f);

    // Render Stuff
    sfRenderWindow_drawRectangleShape(screen, shape, NULL);
    sfRenderWindow_drawText(screen, actualtext, NULL);

    // Clear Stuff
    sfText_destroy(actualtext);
    sfRectangleShape_destroy(shape);
    shape = NULL;
    actualtext = NULL;
}

// Draw Score
void DrawScore()
{
    // First Text
    sfColor Col = {255, 74, 0, 255};

    sfText *one = sfText_create();
    sfText *two = sfText_create();
    sfText *third = sfText_create();
    sfText_setFont(one, media.normal_font);
    sfText_setFont(two, media.normal_font);
    sfText_setFont(third, media.normal_font);
    sfText_setCharacterSize(one, 25);
    sfText_setCharacterSize(two, 25);
    sfText_setCharacterSize(third, 25);
    sfText_setColor(one, Col);
    sfText_setColor(two, Col);
    sfText_setColor(third, Col);
    sfText_setString(one, "You're died dude!");
    sfText_setString(third, "Go Back and try again!");

    // Second Text
    char Text1[50] = {0};
    strncat(Text1, "You got: ", 10);
    char Score[30] = {0};
    itoa(GameSnake.S_Score, Score, 10);
    strncat(Text1, Score, 30);
    strncat(Text1, " Points", 8);
    sfText_setString(two, Text1);
    sfVector2f Pos1 = {100, 250}, Pos2 = {100, 300}, Pos3 = {100, 350};
    sfText_setPosition(one, Pos1);
    sfText_setPosition(two, Pos2);
    sfText_setPosition(third, Pos3);

    sfRenderWindow_drawText(screen, one, NULL);
    sfRenderWindow_drawText(screen, two, NULL);
    sfRenderWindow_drawText(screen, third, NULL);

    sfText_destroy(one);
    sfText_destroy(two);
    sfText_destroy(third);
    one = NULL;
    two = NULL;
    third = NULL;


}

// Process Menu
bool MenuPlace()
{
    if(!CSFMLIsInit || !mstate.isInit)
        return false;

    bool ok = false;

    sfColor white = {255,255,255,255};
    sfColor other = {255, 74, 0, 255};

    switch(mstate.actualsite)
    {
        // Layout und Funktionen für Hauptseite
        case MAINSITE:

            // Aufräumen
            CSFMLCLRSCR();
            CSFMLCLRLevelStuff();
            sfRenderWindow_setTitle(screen, "Snake Game: Hauptmenü");

            // Normale Elemente Platzieren
            MenuPlaceIMG(0, 0, false, 0);
            MenuPlaceIMG(0, 200, true, 1);

            // Elemente dieser Seite aktiv setzen
            MenuSetElementsVisible(MAINSITE);
        break;

        case SCORESITE:

            // Aufräumen
            CSFMLCLRSCR();
            sfRenderWindow_setTitle(screen, "Snake Game: Highscore");

            // Elemente Platzieren
            MenuPlaceIMG(0, 0, false, 0);
            MenuPlaceIMG(0, 200, true, 2);

            // Variablen setzen
            mstate.actualpage = 1;

            // Elemente dieser Seite aktiv setzen
            MenuSetElementsVisible(SCORESITE);

        break;

        case SCORESITE_NEXT:

            // Aufräumen
            CSFMLCLRSCR();
            sfRenderWindow_setTitle(screen, "Snake Game: Highscore");

            // Elemente Platzieren
            MenuPlaceIMG(0, 0, false, 0);
            MenuPlaceIMG(0, 200, true, 2);

            // Elemente dieser Seite aktiv setze
            MenuSetElementsVisible(SCORESITE_NEXT);
        break;

        case SCORESITE_BACK:

            // Aufräumen
            CSFMLCLRSCR();
            sfRenderWindow_setTitle(screen, "Snake Game: Highscore");

            // Elemente Platzieren
            MenuPlaceIMG(0, 0, false, 0);
            MenuPlaceIMG(0, 200, true, 2);

            // Elemente dieser Seite aktiv setzen
            MenuSetElementsVisible(SCORESITE_BACK);
        break;

        case SETTINGSSITE:

            // Aufräumen
            CSFMLCLRSCR();
            sfRenderWindow_setTitle(screen, "Snake Game: Einstellungen");

            // Elemente platzieren
            MenuPlaceIMG(0, 0, false, 0);
            MenuPlaceIMG(0, 200, true, 3);
            MenuPlaceText("Sound Lautstärke:", 20, 230, 20, white);
            MenuPlaceText("Musik Lautstärke:", 20, 280, 20, white);
            MenuPlaceText("Shader Effekte Status:", 20, 350, 20, white);
            MenuPlaceText("Vertikal Sync Status:", 20, 400, 20, white);
            MenuPlaceText((shader_enabled) ? "-Aktiviert-" : "-Deaktiviert-", 305, 350, 20, other);
            MenuPlaceText((vsync_enabled) ? "-Aktiviert-" : "-Deaktiviert-", 305, 400, 20, other);

            // ELemente dieser Seite aktiv setzen
            MenuSetElementsVisible(SETTINGSSITE);
        break;

        case SETTING_SHAD:

            // Shader aktivieren oder deaktivieren
            ToggleShader();

            // Ruecksprung in Menu
            mstate.prevsite = mstate.actualsite;
            mstate.actualsite = SETTINGSSITE;
            MenuPlace();
        break;

        case SETTING_VSYNC:

            // Vsync aktivieren oder deaktivieren
            ToggleVsync();

            mstate.prevsite = mstate.actualsite;
            mstate.actualsite = SETTINGSSITE;
            MenuPlace();
        break;


        // Aktuelle Einstellungen speichern
        case SETTINGSAVE:
            CSFMLCLRSCR();
            sfRenderWindow_setTitle(screen, "Snake Game: Speichern....");

            ok = WriteSettings();

            if(ok)
                MenuPlacePopup("Daten wurden gespeichert!", SETTINGSSITE, SETTINGSAVE, true);
            else
                MenuPlacePopup("Fehler beim Speichern!", SETTINGSSITE, SETTINGSAVE, true);

            ReadSetting();
        break;

        // Default Einstellungen speichern
        case SETTINGDEF:
            CSFMLCLRSCR();
            sfRenderWindow_setTitle(screen, "Snake Game: Laden...");

            ok = WriteSettingsDefault();

            if(ok)
                MenuPlacePopup("Default-Werte geladen!", SETTINGSSITE, SETTINGDEF, true);
            else
                MenuPlacePopup("Fehler beim Laden !", SETTINGSSITE, SETTINGDEF, true);
        break;

        // Level Selection
        case GAMESITE_LEVEL:
            CSFMLCLRSCR();
            sfRenderWindow_setTitle(screen, "Snake Game: Level Auswahl");

            MenuPlaceIMG(0, 0, false, 0);
            MenuPlaceIMG(0, 200, true, 0);

            // Print Levels
            CSFMLPrintLevels();

            MenuSetElementsVisible(GAMESITE_LEVEL);
        break;

        // User Enter
        case GAMESITE_USER:
            CSFMLCLRSCR();
            sfRenderWindow_setTitle(screen, "Snake Game: Gebe deinen Namen ein");

            MenuPlaceIMG(0,0, false, 0);
            MenuPlaceIMG(0, 200, true, 0);

            // Print User Enter with Button
            CSFMLPrintUserEnter();

            MenuSetElementsVisible(GAMESITE_USER);
        break;

        // Play Now!
        case GAMESITE_PLAY:
            // Aufräumen
            CSFMLCLRSCR();
            sfRenderWindow_setTitle(screen, "Snake Game: Spiele jetzt!");

            mstate.startgame = true;

        break;

        // Snake Dead Page
        case GAMESITE_END:
            sfRenderWindow_setTitle(screen, "Snake Game: Spiel Ende");

            MenuPlaceIMG(0,0, false, 0);
            MenuPlaceIMG(0, 200, true, 0);

            MenuSetElementsVisible(GAMESITE_END);
            if(!mstate.hs_written)
            {
                WriteHighScore(GameSnake.S_Name, (uint32_t)GameSnake.S_Score);
                mstate.hs_written = true;
            }

            DrawScore();
            CSFMLCLRLevelStuff();
        break;

        // Layout und Funktionen für Exitseite
        case EXITSITE:
            // Aufräumen
            CSFMLCLRSCR();
            sfRenderWindow_setTitle(screen, "Snake Game: Beenden");
            sfRenderWindow_close(screen);
        break;
    }
    return true;
}
