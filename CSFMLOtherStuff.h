#ifndef CSFMLOtherStuff_H

#define CSFMLOtherStuff_H

// Includes
#include "CSFMLGameMain.h"

// Variables
static bool Check_Performed;
static bool R_Init;
static bool SSE1_Available;

// Functions
bool SSEAvail();
void Rand_Init();
bool CompareFloats(float Value1, float Value2);
float GetRandomFloatNumber(int MAX);
bool CheckUnicode(char Check);
char *itoa(int value, char *result, int base);

#endif /* CSFMLOtherStuff_H */
