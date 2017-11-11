#include "CSFMLOtherStuff.h"

// Compare two Floats
bool CompareFloats(float Value1, float Value2)
{
    return fabs(Value1 - Value2) < 0.01f;
}

// Getting a Random Float Number
float GetRandomFloatNumber(int MAX)
{
    float val =  (MAX) * ((float)rand() / RAND_MAX);

    short sign = val < 0 ? -1 : 1;

    return floorf(fabs(val)*2.f) / 2.f * sign;
}

// Check if a char is an Number / Alphabet
bool CheckUnicode(char Check)
{
    if((Check >= 48 && Check <= 57) || (Check >= 97 && Check <= 122) || (Check >= 65 && Check <= 97))
        return true;
    else
        return false;
}
