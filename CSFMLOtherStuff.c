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

