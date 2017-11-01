#ifndef CSFMLMENU_H

#define CSFMLMENU_H
// Includes
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <SFML/System.h>
#include <SFML/Config.h>
#include <SFML/OpenGL.h>
#include "CSFMLFileHandling.h"
#include "CSFMLGameMain.h"

// Eigenschaften
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 700
#define ENTRYS_PER_PAGE 10

// Aufzählungen
typedef enum { MAINSITE, SCORESITE, SCORESITE_NEXT, SCORESITE_BACK, SETTINGSSITE, SETTING_SHAD, SETTING_VSYNC, SETTINGSAVE, SETTINGDEF, GAMESITE_USERENTER, EXITSITE } MENUSITES;

// Medien Strukturen
typedef struct {
    sfTexture *normal_button;
    sfTexture *over_button;
    sfTexture *click_button;
} mButton;

typedef struct {
    sfTexture *background;
    MENUSITES destination;
} mBackground;

typedef struct {
    mButton *buttons;
    mBackground *bg;
    sfTexture *header;
    sfFont *normal_font;
    sfFont *spezial_font;
    sfFont *mega_font;
} mMedia;

// Strukturen
typedef struct {
    char owner[50];
    int score;
} HIGHSCORE;

typedef struct {
    int effect;
    int actual_value;
} SETTINGS;

typedef struct {
    sfVector2f pos;
    sfVector2f siz;
} Rect;

typedef struct {
    Rect coordinates;
    MENUSITES target;
    int buttonID;
    bool recentpress;
    bool clickable;
    MENUSITES destination;
    bool visible;
} BUTTONS;

typedef struct {
    Rect scrollbar;
    Rect slider;
    sfColor scrollbar_color;
    sfColor slidernormal_color;
    sfColor sliderpress_color;
    int effect;
    int actualvalue;
    bool recentpress;
    bool clickable;
    MENUSITES destination;
    bool visible;
} SLIDER;

typedef struct
{
    // Menu
    bool isInit;
    bool isPress;
    sfVector2i mouse_pos;
    sfVector2f n_mouse_pos;
    MENUSITES actualsite;
    MENUSITES prevsite;
    int scorepages;
    int actualpage;

    // Buttons
    BUTTONS *btn;
    size_t btn_stack;
    bool btnCleared;

    // Sliders
    SLIDER *sliders;
    size_t sld_stack;
    bool sliderCleared;

    // Settings
    SETTINGS *setting;
    size_t set_stack;
    bool setCleared;

    // Highscore
    HIGHSCORE *entries;
    size_t entry_stack;
    bool entryCleared;

    // Start Game
    bool startgame;

} MenuState;

// Variablen Allgemein
sfRenderWindow *screen;
MenuState mstate;
mMedia media;
bool CSFMLIsInit;
sfEvent EventLoop;

// FPS Variablen
sfTime actualtime;
sfTime lasttime;
sfClock *clock;
float FPS;
sfFont *fps_font;
sfText *fps_text;

// Einstellungen
bool shader_enabled;
bool vsync_enabled;

// Shader Variablen
// Buttons
bool iButtonBlur_end;
int iButtonBlur;
float fButtonBlur;

// Prototypen
bool CSFMLInit();
bool CSFMLMenuInit();
void ChangeBlur();
void CSFMLMenuQuit();
void CSFMLQuit();
void CSFMLCLRSCR();
void CSFMLShowFPS();

bool MenuSetButtonStack();
void MenuClearButtonStack();

bool MenuSetTextStack();
void MenuClearTextStack();

bool MenuSetSliderStack();
void MenuClearSliderStack();

bool MenuSetSettingStack();
void MenuClearSettingStack();

bool MenuSetHighScoreStack();
void MenuClearHighScoreStack();

bool MenuSetElementsVisible(MENUSITES page);
bool MenuPlaceRect(Rect r, sfColor color, bool border, sfColor bordercolor, int thickness);
bool MenuPlaceIMG(int x, int y, bool type, int id);
bool MenuPlacePopup(const char *text, MENUSITES target, MENUSITES destination, bool visible);
bool MenuPlaceSlider(int x, int y, int w, int h, sfColor sb_color, sfColor sn_color, sfColor sh_color, int effect, MENUSITES destination, bool visible);
bool MenuPlaceButton(int state, int id, int x, int y, bool clickable, MENUSITES target, MENUSITES destination, bool visible);
bool MenuPlaceText(const char *txt, int x, int y, unsigned int txtsize, sfColor color);
bool MenuPlaceHighScore(bool visible);

bool MenuProcessButtons();
bool MenuProcessSliders();
bool MenuPlace();


#endif /* CSFMLMENU_H */
