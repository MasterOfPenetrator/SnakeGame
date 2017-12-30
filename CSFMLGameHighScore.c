#include "CSFMLGameHighScore.h"

// Init Highscore Subsystem
bool CSFMLInitHighscore(void)
{
    // Open File
    FILE *HS_File = fopen("Bilder/Game/HighscoreData/Highscore.DAT", "rb");

    // File not existent
    if(HS_File == NULL)
    {
        // Create a file without anything
        HS_File = fopen("Bilder/Game/HighscoreData/Highscore.DAT", "ab+");

        // Check for creation
        if(HS_File == NULL)
        {
            printf("Game Subsystem Fehler 'Highscore': Kann keine Highscore Datei anlegen!\n");
            return false;
        }

        // Setup general things
        GameHighscore.HE_Entries = NULL;
        GameHighscore.HE_Entries_Length = 0;
        GameHighscore.HE_Is_Init = true;
    }
    // File existent
    else
    {
        // Get the entries of the Highscore file
        long HE_FileSize = 0;

        fseek(HS_File, 0L, SEEK_END);
        HE_FileSize = ftell(HS_File);

        GameHighscore.HE_Entries_Length = (size_t)HE_FileSize / sizeof(Highscore_Entry);

        // Jump to the start of the file
        fseek(HS_File, 0L, SEEK_SET);

        // Read Entries
        if(GameHighscore.HE_Entries_Length > 0)
        {
            // Allocate new Memory
            GameHighscore.HE_Entries = malloc(GameHighscore.HE_Entries_Length * sizeof(Highscore_Entry));

            if(GameHighscore.HE_Entries == NULL)
            {
                printf("Game Subsystem Fehler 'Highscore': Kann keinen Speicher fuer Highscore Eintraege anlegen!\n");
                return false;
            }

            // Read now
            if(fread(GameHighscore.HE_Entries, sizeof(Highscore_Entry), GameHighscore.HE_Entries_Length, HS_File) != GameHighscore.HE_Entries_Length)
            {
                printf("Game Subsystem Fehler 'Highscore': Kann die Highscore Datei nicht lesen!\n");
                return false;
            }

            // Checkup for Checksum
            size_t i,s;
            for(i = 0; i<GameHighscore.HE_Entries_Length; i++)
            {
                if(GameHighscore.HE_Entries[i].HE_Checksum != GameHighscore.HE_Entries[i].HE_Score*GameHighscore.HE_Entries[i].HE_SnakeLength)
                    GameHighscore.HE_Entries[i].HE_BadChecksum = true;
            }

            // Order Entries in the right position
            for(i = 0; i<GameHighscore.HE_Entries_Length-1 && GameHighscore.HE_Entries_Length > 1; i++)
            {
                for(s = 0; s<GameHighscore.HE_Entries_Length-i-1; s++)
                {
                    // Temp Var
                    Highscore_Entry Temp;

                    // On the same Map by Score
                    if(strcmp(GameHighscore.HE_Entries[s].HE_Levelname, GameHighscore.HE_Entries[s+1].HE_Levelname) == 0)
                    {
                        if(GameHighscore.HE_Entries[s].HE_Score < GameHighscore.HE_Entries[s+1].HE_Score)
                        {
                            Temp = GameHighscore.HE_Entries[s];
                            GameHighscore.HE_Entries[s] = GameHighscore.HE_Entries[s+1];
                            GameHighscore.HE_Entries[s+1] = Temp;
                        }
                    }
                    // Otherwise by the Map Name
                    else if(strcmp(GameHighscore.HE_Entries[s].HE_Levelname, GameHighscore.HE_Entries[s+1].HE_Levelname) > 0)
                    {
                        Temp = GameHighscore.HE_Entries[s];
                        GameHighscore.HE_Entries[s] = GameHighscore.HE_Entries[s+1];
                        GameHighscore.HE_Entries[s+1] = Temp;
                    }
                }
            }

            // Setup general things
            GameHighscore.HE_Is_Init = true;
        }
    }

    // Clean up
    fclose(HS_File);
    HS_File = NULL;

    return true;
}

// Quit Game Highscore
void CSFMLQuitHighscore(void)
{
    // Cleanup memory
    if(GameHighscore.HE_Is_Init)
    {
        if(GameHighscore.HE_Entries_Length > 0)
        {
            free(GameHighscore.HE_Entries);
            GameHighscore.HE_Entries = NULL;
        }
    }

    // Cleanup general things
    GameHighscore.HE_Entries_Length = 0;
    GameHighscore.HE_Is_Init = false;
}

// Refresh Highscore List
bool CSFMLRefreshHighscore(void)
{
    // Open File
    FILE *HS_File = fopen("Bilder/Game/HighscoreData/Highscore.DAT", "rb");

    if(HS_File == NULL)
    {
        printf("Game Subsystem Fehler 'Highscore': Kann beim Refresh Highscore Datei nicht oeffnen!\n");
        return false;
    }

    // Get Total Entries in the file
    size_t HS_Entries = 0, i, s;
    long HS_FileSize = 0;

    fseek(HS_File, 0L, SEEK_END);
    HS_FileSize = ftell(HS_File);
    HS_Entries = (size_t)HS_FileSize / sizeof(Highscore_Entry);

    // More Entries in the file
    if(HS_Entries > GameHighscore.HE_Entries_Length)
    {
        // Move to the beginning
        fseek(HS_File, 0L, SEEK_SET);

        // Get new memory
        if(GameHighscore.HE_Entries_Length == 0)
        {
            GameHighscore.HE_Entries = malloc(HS_Entries * sizeof(Highscore_Entry));

            if(GameHighscore.HE_Entries == NULL)
            {
                printf("Game Subsystem Fehler 'Highscore': Kann keinen Speicher fuer Highscore Refresh anlegen!\n");
                return false;
            }
        }
        else
        {
            GameHighscore.HE_Entries = realloc(GameHighscore.HE_Entries, HS_Entries * sizeof(Highscore_Entry));

            if(GameHighscore.HE_Entries == NULL)
            {
                printf("Game Subsystem Fehler 'Highscore': Kann keinen Speicher fuer Highscore Refresh anlegen!\n");
                return false;
            }
        }

        // Read Entries
        if(fread(GameHighscore.HE_Entries, sizeof(Highscore_Entry), HS_Entries, HS_File) != HS_Entries)
        {
            printf("Game Subsystem Fehler 'Highscore': Kann beim Highscore Refresh Datensaetze nicht lesen!\n");
            return false;
        }

        // Setup general things
        GameHighscore.HE_Entries_Length = HS_Entries;
    }
    // Less entries in the file?!
    else if(HS_Entries < GameHighscore.HE_Entries_Length && HS_Entries != 0)
    {
        // Move to the beginning
        fseek(HS_File, 0L, SEEK_SET);

        // Just read
        if(fread(GameHighscore.HE_Entries, sizeof(Highscore_Entry), HS_Entries, HS_File) != HS_Entries)
        {
            printf("Game Subsystem Fehler 'Highscore': Kann beim Highscore Refresh Datensaetze nicht lesen!\n");
            return false;
        }

        // Setup general things
        GameHighscore.HE_Entries_Length = HS_Entries;
    }

    // Clean up things
    fclose(HS_File);
    HS_File = NULL;

    // Checkup for Checksum
    for(i = 0; i<GameHighscore.HE_Entries_Length; i++)
    {
        if(GameHighscore.HE_Entries[i].HE_Checksum != GameHighscore.HE_Entries[i].HE_Score*GameHighscore.HE_Entries[i].HE_SnakeLength)
            GameHighscore.HE_Entries[i].HE_BadChecksum = true;
    }

    // Order Entries in the right position
    for(i = 0; i<GameHighscore.HE_Entries_Length-1 && GameHighscore.HE_Entries_Length > 1; i++)
    {
        for(s = 0; s<GameHighscore.HE_Entries_Length-i-1; s++)
        {
            // Temp Var
            Highscore_Entry Temp;

            // On the same Map
            if(strcmp(GameHighscore.HE_Entries[s].HE_Levelname, GameHighscore.HE_Entries[s+1].HE_Levelname) == 0)
            {
                if(GameHighscore.HE_Entries[s].HE_Score < GameHighscore.HE_Entries[s+1].HE_Score)
                {
                    Temp = GameHighscore.HE_Entries[s];
                    GameHighscore.HE_Entries[s] = GameHighscore.HE_Entries[s+1];
                    GameHighscore.HE_Entries[s+1] = Temp;
                }
            }
            else if(strcmp(GameHighscore.HE_Entries[s].HE_Levelname, GameHighscore.HE_Entries[s+1].HE_Levelname) > 0)
            {
                Temp = GameHighscore.HE_Entries[s];
                GameHighscore.HE_Entries[s] = GameHighscore.HE_Entries[s+1];
                GameHighscore.HE_Entries[s+1] = Temp;
            }
        }
    }

    return true;
}

// Write a Highscore Entry to the file and memory
bool CSFMLWriteHighscore(void)
{
    // Just write, if the Snake is truly dead and GameTime is setted
    if(GameSnake.S_Is_Dead && GameClock.GC_GameTime > 0.0f)
    {
        // Make Entry
        Highscore_Entry New;
        New.HE_Score = GameSnake.S_Score;
        New.HE_SnakeLength = GameSnake.SB_Body_Elements + 1;
        New.HE_TimeAlive = GameClock.GC_GameTime;
        strncpy(New.HE_Username, GameSnake.S_Name, strlen(GameSnake.S_Name)+1);
        strncpy(New.HE_Levelname, Level.MD_Details.MapName, strlen(Level.MD_Details.MapName)+1);
        New.HE_Checksum = New.HE_Score * New.HE_SnakeLength;
        New.HE_BadChecksum = false;

        // Write it to file
        FILE *HS_File = fopen("Bilder/Game/HighscoreData/Highscore.DAT", "ab+");

        if(HS_File == NULL)
        {
            printf("Game Subsystem Fehler 'Highscore': Kann Datei nicht oeffnen zum schreiben!\n");
            return false;
        }

        if(fwrite(&New, sizeof(Highscore_Entry), 1, HS_File) != 1)
        {
            printf("Game Subsystem Fehler 'Highscore': Kann neuen Datensatz nicht schreiben!\n");
            return false;
        }

        // Cleanup things
        fclose(HS_File);
        HS_File = NULL;

        // Refresh List
        if(!CSFMLRefreshHighscore())
        {
            printf("Game Subsystem Fehler 'Highscore': Fehler beim Refreshen!!\n");
            return false;
        }
    }

    return true;
}
