#ifndef CSFMLFileHandling_H

#define CSFMLFileHandling_H

// Includes
#include "CSFMLMenu.h"

// Funktionen
void WriteHighScore(const char *owner, uint32_t score);
size_t GetHighScoreEntries();
void ReadHighScore();

bool WriteSettings();
bool WriteSettingsDefault();
size_t GetSettingsEntries();
void ReadSetting();

#endif /* CSFMLFileHandling_H */
