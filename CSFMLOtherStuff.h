#ifndef CSFMLOtherStuff_H

#define CSFMLOtherStuff_H

// Includes
#include "CSFMLGameMain.h"

// Variables
static bool Check_Performed = false;
static bool SSE1_Available = false;

// Functions
bool SSEAvail();
bool CompareFloats(float Value1, float Value2);
float GetRandomFloatNumber(int MAX);
bool CheckUnicode(char Check);
char *itoa(int value, char *result, int base);

#endif /* CSFMLOtherStuff_H */
