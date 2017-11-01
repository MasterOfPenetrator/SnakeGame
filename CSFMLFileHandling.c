#include "CSFMLFileHandling.h"

void WriteHighScore(const char *owner, uint32_t score)
{
    FILE *highscore_file = fopen("Bilder/Einstellungen/HighscoresDATA.MOP", "a+b");
    HIGHSCORE toWrite;

    if(highscore_file == NULL)
    {
        printf("File Subsystem Fehler: Highscore Datei kann nicht geoffnet werden.\n");

    }
    else
    {
        strncpy(toWrite.owner, owner, 50);
        toWrite.score = score;
        fwrite(&toWrite, sizeof(HIGHSCORE), 1, highscore_file);
    }

    fclose(highscore_file);
    highscore_file = NULL;
}

size_t GetHighScoreEntries()
{
    FILE *highscore_file = fopen("Bilder/Einstellungen/HighscoresDATA.MOP", "rb");
    long file_size = 0;
    size_t ret = 0;

    if(highscore_file == NULL)
    {
        printf("File Subsystem Fehler: Highscores Datei kann nicht geöffnet werden.\n");
    }
    else
    {
        fseek(highscore_file, 0L, SEEK_END);
        file_size = ftell(highscore_file);

        ret = (size_t)file_size / sizeof(HIGHSCORE);
    }

    fclose(highscore_file);
    highscore_file = NULL;

    return ret;
}

void ReadHighScore()
{
    FILE *highscore_file = fopen("Bilder/Einstellungen/HighscoresDATA.MOP", "rb");

    if(highscore_file == NULL)
    {
        printf("File Subsystem Fehler: Highscores Datei kann nicht geöffnet werden.\n");
    }
    else
    {

        if(mstate.entries != NULL && mstate.entry_stack > 0)
        {
            fread(mstate.entries, sizeof(HIGHSCORE), mstate.entry_stack, highscore_file);
        }
        else
        {
            printf("File Subsystem Fehler: Speicher nicht reserviert!\n");
        }
    }

    fclose(highscore_file);
    highscore_file = NULL;
}

void PutHighScore()
{
    HIGHSCORE eins[25] = {{"Peter", 1},
    {"Peter", 2},
    {"Peter", 3},
    {"Peter", 4},
    {"Peter", 5},
    {"Peter", 7},
    {"Peter", 8},
    {"Peter", 9},
    {"Peter", 10},
    {"Peter", 11},
    {"Peter", 111},
    {"Peter", 112},
    {"Peter", 113},
    {"Peter", 411},
    {"Peter", 511},
    {"Peter", 116},
    {"Peter", 7111},
    {"Peter", 118},
    {"Peter", 911},
    {"Peter", 1110},
    {"Peter", 11111},
    {"Peter", 118},
    {"Peter", 911},
    {"Peter", 1110},
    {"Peter", 11111} };

    FILE *test = fopen("Bilder/Einstellungen/HighscoresDATA.MOP", "ab+");

    fwrite(eins, sizeof(HIGHSCORE), 25, test);

    fclose(test);
}

bool WriteSettings()
{
    FILE *setting_file = fopen("Bilder/Einstellungen/SettingsDATA.MOP", "rb+");
    bool ok = false;

    if(setting_file == NULL)
    {
        printf("File Subsystem Fehler: Settings Datei kann nicht geoffnet werden.\n");
    }
    else
    {
        if(fwrite(mstate.setting, sizeof(SETTINGS), mstate.set_stack, setting_file) == mstate.set_stack)
            ok = true;
    }

    fclose(setting_file);
    setting_file = NULL;

    return ok;
}

bool WriteSettingsDefault()
{
    FILE *setting_file = fopen("Bilder/Einstellungen/SettingsDATA.MOP", "wb");
    bool ok = false;

    if(setting_file == NULL)
    {
        printf("File Subsystem Fehler: Settings Datei kann nicht geoffnet werden.\n");
    }
    else
    {
        mstate.setting[0].effect = 0;
        mstate.setting[0].actual_value = 50;
        mstate.setting[1].effect = 1;
        mstate.setting[1].actual_value = 50;
        mstate.setting[2].effect = 2;
        mstate.setting[2].actual_value = 1;
        mstate.setting[3].effect = 3;
        mstate.setting[3].actual_value = 1;

        if(fwrite(mstate.setting, sizeof(SETTINGS), mstate.set_stack, setting_file) == mstate.set_stack)
            ok = true;

    }

    fclose(setting_file);
    setting_file = NULL;

    return ok;
}

size_t GetSettingsEntries()
{
    FILE *setting_file = fopen("Bilder/Einstellungen/SettingsDATA.MOP", "rb");
    long file_size = 0;
    size_t ret = 0;

    if(setting_file == NULL)
    {
        printf("File Subsystem Fehler: Settings Datei kann nicht geöffnet werden.\n");
    }
    else
    {
        fseek(setting_file, 0L, SEEK_END);
        file_size = ftell(setting_file);

        ret = (size_t)file_size / sizeof(SETTINGS);
    }

    fclose(setting_file);
    setting_file = NULL;

    return ret;
}

void ReadSetting()
{
    FILE *setting_file = fopen("Bilder/Einstellungen/SettingsDATA.MOP", "rb");

    if(setting_file == NULL)
    {
        printf("File Subsystem Fehler: Highscores Datei kann nicht geöffnet werden.\n");
    }
    else
    {

        if(mstate.setting != NULL)
        {
            size_t ret = fread(mstate.setting, sizeof(SETTINGS), mstate.set_stack, setting_file);

            if(ret != mstate.set_stack)
            {
                printf("Konnte Daten nicht lesen. Ret: %d, Normal: %d", ret, mstate.set_stack);
            }
        }
        else
        {
            printf("File Subsystem Fehler: Speicher nicht reserviert!\n");
        }
    }

    fclose(setting_file);
    setting_file = NULL;
}
