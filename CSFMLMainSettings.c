#include "CSFMLMainSettings.h"

// Init Settings
bool CSFMLInitSettings(void)
{
    // Open File
    FILE *SE_File = fopen("Bilder/Einstellungen/Settings.DAT", "rb");

    // File does not exist
    if(SE_File == NULL)
    {
        // Create a file Default Settings
        SE_File = fopen("Bilder/Einstellungen/Settings.DAT", "wb+");

        if(SE_File == NULL)
        {
            printf("Main Subsystem Fehler 'Settings': Kann keine Settingsdatei erstellen!\n");
            return false;
        }

        // Allocate Memory for Default Settings
        MainSettings.MS_Settings = malloc(4 * sizeof(Settings));

        if(MainSettings.MS_Settings == NULL)
        {
            printf("Main Subsystem Fehler 'Settings': Kann keinen Speicher fuer Settings reservieren!\n");
            return false;
        }

        // Fill Settings
        // Sound Volume
        MainSettings.MS_Settings[0].Actual = 50;
        MainSettings.MS_Settings[0].Lower_Limit = 0;
        MainSettings.MS_Settings[0].Upper_Limit = 100;
        MainSettings.MS_Settings[0].Type = SOUND_VOLUME;
        MainSettings.MS_Settings[0].Value = INTEGER_VALUE;

        // Music Volume
        MainSettings.MS_Settings[1].Actual = 50;
        MainSettings.MS_Settings[1].Lower_Limit = 0;
        MainSettings.MS_Settings[1].Upper_Limit = 100;
        MainSettings.MS_Settings[1].Type = MUSIC_VOLUME;
        MainSettings.MS_Settings[1].Value = INTEGER_VALUE;

        // Shader Active
        MainSettings.MS_Settings[2].Actual = 1;
        MainSettings.MS_Settings[2].Lower_Limit = 0;
        MainSettings.MS_Settings[2].Upper_Limit = 1;
        MainSettings.MS_Settings[2].Type = SHADER_ON;
        MainSettings.MS_Settings[2].Value = BOOLEAN_VALUE;

        // VSYNC Active
        MainSettings.MS_Settings[3].Actual = 1;
        MainSettings.MS_Settings[3].Lower_Limit = 0;
        MainSettings.MS_Settings[3].Upper_Limit = 1;
        MainSettings.MS_Settings[3].Type = VSYNC_ON;
        MainSettings.MS_Settings[3].Value = BOOLEAN_VALUE;

        // Write it to the file
        if(fwrite(MainSettings.MS_Settings, sizeof(Settings), 4, SE_File) != 4)
        {
            printf("Main Subsystem Fehler 'Settings': Kann Default Settings nicht in Datei schreiben!\n");
            return false;
        }

        // General stuff
        MainSettings.MS_Settings_Entries = 4;
        MainSettings.MS_Settings_Is_Init = true;

        // Cleanup
        fclose(SE_File);
        SE_File = NULL;
    }
    // File exists
    else
    {
        // Get Entries of the file
        long SE_FileSize = 0;
        fseek(SE_File, 0L, SEEK_END);
        SE_FileSize = ftell(SE_File);

        MainSettings.MS_Settings_Entries = (size_t)SE_FileSize / sizeof(Setting);

        // Move to the beginning
        fseek(SE_File, 0L, SEEK_SET);

        // Validate Entries
        // Not 4 Settings?
        if(MainSettings.MS_Settings_Entries != 4)
        {
            // Print Warning Message
            printf("Main Subsystem Warning 'Settings': Setting Datei ist korrumpiert! Es wird mit den Default Settings geschrieben!\n");

            // Close actual File Handle
            fclose(SE_File);

            // Open File with Write
            SE_File = fopen("Bilder/Einstellungen/Settings.DAT", "wb+");

            if(SE_File == NULL)
            {
                printf("Main Subsystem Fehler 'Settings': Kann Datei nicht oeffnen!\n");
                return false;
            }

            // Allocate Memory for Default Settings
            MainSettings.MS_Settings = malloc(4 * sizeof(Settings));

            if(MainSettings.MS_Settings == NULL)
            {
                printf("Main Subsystem Fehler 'Settings': Kann keinen Speicher fuer Settings reservieren!\n");
                return false;
            }

            // Fill Settings
            // Sound Volume
            MainSettings.MS_Settings[0].Actual = 50;
            MainSettings.MS_Settings[0].Lower_Limit = 0;
            MainSettings.MS_Settings[0].Upper_Limit = 100;
            MainSettings.MS_Settings[0].Type = SOUND_VOLUME;
            MainSettings.MS_Settings[0].Value = INTEGER_VALUE;

            // Music Volume
            MainSettings.MS_Settings[1].Actual = 50;
            MainSettings.MS_Settings[1].Lower_Limit = 0;
            MainSettings.MS_Settings[1].Upper_Limit = 100;
            MainSettings.MS_Settings[1].Type = MUSIC_VOLUME;
            MainSettings.MS_Settings[1].Value = INTEGER_VALUE;

            // Shader Active
            MainSettings.MS_Settings[2].Actual = 1;
            MainSettings.MS_Settings[2].Lower_Limit = 0;
            MainSettings.MS_Settings[2].Upper_Limit = 1;
            MainSettings.MS_Settings[2].Type = SHADER_ON;
            MainSettings.MS_Settings[2].Value = BOOLEAN_VALUE;

            // VSYNC Active
            MainSettings.MS_Settings[3].Actual = 1;
            MainSettings.MS_Settings[3].Lower_Limit = 0;
            MainSettings.MS_Settings[3].Upper_Limit = 1;
            MainSettings.MS_Settings[3].Type = VSYNC_ON;
            MainSettings.MS_Settings[3].Value = BOOLEAN_VALUE;

            // Write it to the file
            if(fwrite(MainSettings.MS_Settings, sizeof(Settings), 4, SE_File) != 4)
            {
                printf("Main Subsystem Fehler 'Settings': Kann Default Settings nicht in Datei schreiben!\n");
                return false;
            }

            // General stuff
            MainSettings.MS_Settings_Entries = 4;
            MainSettings.MS_Settings_Is_Init = true;

            // Cleanup
            fclose(SE_File);
            SE_File = NULL;

        }
        // 4 Settings, but check all Entries!
        else
        {
            // Allocate Memory for Default Settings
            MainSettings.MS_Settings = malloc(MainSettings.MS_Settings_Entries * sizeof(Settings));

            if(MainSettings.MS_Settings == NULL)
            {
                printf("Main Subsystem Fehler 'Settings': Kann keinen Speicher fuer Settings reservieren!\n");
                return false;
            }

            // Read from file
            if(fread(MainSettings.MS_Settings, sizeof(Setting), MainSettings.MS_Settings_Entries, SE_File) != MainSettings.MS_Settings_Entries)
            {
                printf("Main Subsystem Fehler 'Settings': Kann Datensaetze nicht lesen!\n");
                return false;
            }

            // Now Check all Entries for validity
            size_t i;
            bool SFX = false, MFX = false, SON = false, VON = false;
            for(i = 0; i<MainSettings.MS_Settings_Entries; i++)
            {
                switch(MainSettings.MS_Settings[i].Type)
                {
                    case SOUND_VOLUME:

                        // Check for Limits
                        if(MainSettings.MS_Settings[i].Actual >= MainSettings.MS_Settings[i].Lower_Limit && MainSettings.MS_Settings[i].Actual <= MainSettings.MS_Settings[i].Upper_Limit)
                            SFX = true;

                    break;

                    case MUSIC_VOLUME:

                        // Check for Limits
                        if(MainSettings.MS_Settings[i].Actual >= MainSettings.MS_Settings[i].Lower_Limit && MainSettings.MS_Settings[i].Actual <= MainSettings.MS_Settings[i].Upper_Limit)
                            MFX = true;

                    break;

                    case SHADER_ON:

                        // Check for Limits
                        if(MainSettings.MS_Settings[i].Actual >= MainSettings.MS_Settings[i].Lower_Limit && MainSettings.MS_Settings[i].Actual <= MainSettings.MS_Settings[i].Upper_Limit)
                            SON = true;

                    break;

                    case VSYNC_ON:

                        // Check for Limits
                        if(MainSettings.MS_Settings[i].Actual >= MainSettings.MS_Settings[i].Lower_Limit && MainSettings.MS_Settings[i].Actual <= MainSettings.MS_Settings[i].Upper_Limit)
                            VON = true;
                    break;

                    default:
                    break;
                }
            }

            // Some Entity is creepy
            if(!(SFX && MFX && SON && VON))
            {
                // Close actual handle
                fclose(SE_File);

                // Open with write
                SE_File = fopen("Bilder/Einstellungen/Settings.DAT", "wb+");

                // Setup all to Default Values
                // Sound Volume
                MainSettings.MS_Settings[0].Actual = 50;
                MainSettings.MS_Settings[0].Lower_Limit = 0;
                MainSettings.MS_Settings[0].Upper_Limit = 100;
                MainSettings.MS_Settings[0].Type = SOUND_VOLUME;
                MainSettings.MS_Settings[0].Value = INTEGER_VALUE;

                // Music Volume
                MainSettings.MS_Settings[1].Actual = 50;
                MainSettings.MS_Settings[1].Lower_Limit = 0;
                MainSettings.MS_Settings[1].Upper_Limit = 100;
                MainSettings.MS_Settings[1].Type = MUSIC_VOLUME;
                MainSettings.MS_Settings[1].Value = INTEGER_VALUE;

                // Shader Active
                MainSettings.MS_Settings[2].Actual = 1;
                MainSettings.MS_Settings[2].Lower_Limit = 0;
                MainSettings.MS_Settings[2].Upper_Limit = 1;
                MainSettings.MS_Settings[2].Type = SHADER_ON;
                MainSettings.MS_Settings[2].Value = BOOLEAN_VALUE;

                // VSYNC Active
                MainSettings.MS_Settings[3].Actual = 1;
                MainSettings.MS_Settings[3].Lower_Limit = 0;
                MainSettings.MS_Settings[3].Upper_Limit = 1;
                MainSettings.MS_Settings[3].Type = VSYNC_ON;
                MainSettings.MS_Settings[3].Value = BOOLEAN_VALUE;

                // Write it to the File
                if(fwrite(MainSettings.MS_Settings, sizeof(Setting), MainSettings.MS_Settings_Entries, SE_File) != MainSettings.MS_Settings_Entries)
                {
                    printf("Main Subsystem Fehler 'Settings': Kann Settings nicht schreiben!\n");
                    return false;
                }

                // General stuff
                MainSettings.MS_Settings_Is_Init = true;

                // Cleanup
                fclose(SE_File);
                SE_File = NULL;

            }

            // general stuff
            MainSettings.MS_Settings_Is_Init = true;

            // clean up
            fclose(SE_File);
            SE_File = NULL;
        }
    }

    return true;
}

// Quit Settings
void CSFMLQuitSettings(void)
{
    // Cleanup Memory
    if(MainSettings.MS_Settings_Is_Init)
    {
        if(MainSettings.MS_Settings_Entries > 0)
        {
            free(MainSettings.MS_Settings);
            MainSettings.MS_Settings = NULL;
        }
    }

    // Cleanup general
    MainSettings.MS_Settings_Entries = 0;
    MainSettings.MS_Settings_Is_Init = false;
}

// Write Settings to the file
bool CSFMLWriteSettings(void)
{
    // Check for Init
    if(!MainSettings.MS_Settings_Is_Init)
        return false;

    // Open the file
    FILE *SE_File = fopen("Bilder/Einstellungen/Settings.DAT", "wb+");

    if(SE_File == NULL)
    {
        printf("Main Subsystem Fehler 'Settings': Kann Settingsdatei nicht oeffnen fuer schreiben!\n");
        return false;
    }

    // Check Settings for Validity
    size_t i;
    bool Write = true;
    for(i = 0; i<MainSettings.MS_Settings_Entries; i++)
    {
        if(!(MainSettings.MS_Settings[i].Actual >= MainSettings.MS_Settings[i].Lower_Limit && MainSettings.MS_Settings[i].Actual <= MainSettings.MS_Settings[i].Upper_Limit))
            Write = false;
    }

    if(Write)
    {
        // Write now
        if(fwrite(MainSettings.MS_Settings, sizeof(Setting), MainSettings.MS_Settings_Entries, SE_File) != MainSettings.MS_Settings_Entries)
        {
            printf("Main Subsystem Fehler 'Settings': Konnte Datensaetze nicht in Settingsdatei schreiben!\n");
            return false;
        }
    }
    else
    {
            printf("Main Subsystem Fehler 'Settings': Ein oder mehrere Settings sind ausserhalb ihrer Grenzen!\n");
            return false;
    }

    // Cleanup
    fclose(SE_File);
    SE_File = NULL;

    return true;
}

// Update Settings
bool CSFMLUpdateSetting(SettingType Set, int Value)
{
    // Check for Init
    if(!MainSettings.MS_Settings_Is_Init)
        return false;


    // Validate Input and change
    size_t i;
    bool Changed = false;
    for(i = 0; i<MainSettings.MS_Settings_Entries; i++)
    {
        if(MainSettings.MS_Settings[i].Type == Set)
        {
            if(Value >= MainSettings.MS_Settings[i].Lower_Limit && Value <= MainSettings.MS_Settings[i].Upper_Limit)
                Changed = true;
        }
    }

    // Proof
    if(!Changed)
    {
        printf("Main Subsystem Fehler 'Settings': Wert des zu veraendernen Setting oder Setting nicht gueltig!\n");
        return false;
    }

    return true;
}
