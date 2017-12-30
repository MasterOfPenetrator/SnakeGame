#ifndef CSFMLGameHighScore_H

#define CSFMLGameHighScore_H

// Includes
#include "CSFMLGameMain.h"

// Structs
typedef struct {
    char HE_Username[50];
    char HE_Levelname[50];
    size_t HE_SnakeLength;
    size_t HE_Score;
    float HE_TimeAlive;
    size_t HE_Checksum;
    bool HE_BadChecksum;
} Highscore_Entry;

typedef struct {
    Highscore_Entry *HE_Entries;
    size_t HE_Entries_Length;
    bool HE_Is_Init;
} Highscores;

// Variables
Highscores GameHighscore;

// Functions
bool CSFMLInitHighscore();
void CSFMLQuitHighscore();
bool CSFMLRefreshHighscore();
bool CSFMLWriteHighscore();


#endif /* CSFMLGameHighScore_H */
