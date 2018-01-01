#ifndef CSFMLMainSettings_H

#define CSFMLMainSettings_H

// Includes
#include "CSFMLMain.h"

// Enums
typedef enum { SOUND_VOLUME, MUSIC_VOLUME, SHADER_ON, VSYNC_ON } SettingType;
typedef enum { BOOLEAN_VALUE, INTEGER_VALUE } SettingValue;

// Structs
typedef struct {
    SettingType Type;
    SettingValue Value;
    int Actual;
    int Upper_Limit;
    int Lower_Limit;
} Setting;

typedef struct {
    Setting *MS_Settings;
    size_t MS_Settings_Entries;
    bool MS_Settings_Is_Init;
} Settings;

// Variables
Settings MainSettings;

// Functions
bool CSFMLInitSettings();
void CSFMLQuitSettings();
bool CSFMLWriteSettings();
bool CSFMLUpdateSetting(SettingType Set, int Value);

#endif /* CSFMLMainSettings_H */
