#ifndef CSFMLMain_H

#define CSFMLMain_H

// Standard + SFML Includes
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <time.h>
#include <immintrin.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <SFML/System.h>

// Global Enums
typedef enum { IT_HEALTH, IT_SPEED, IT_FOOD, IT_GOD, IT_NOCLIP, IT_CUSTOM } ItemType;
typedef enum { NONE, LEFT, RIGHT, UP, DOWN } Direction;

// Global Structs
typedef struct {
    float x;
    float y;
    float w;
    float h;
} Block;

// Subsystems Includes
#include "CSFMLMainAPI.h"
#include "CSFMLMainSettings.h"
#include "CSFMLMenu.h" // to delete
#include "CSFMLGameMain.h"
#include "CSFMLFileHandling.h" // to delete

#endif /* CSFMLMain_H */
