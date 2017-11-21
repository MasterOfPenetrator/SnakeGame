#include "CSFMLOtherStuff.h"
#define SSE 1

// Compare two Floats
// Use SSE for exactly Compare
// Some weird thing, at any Optimizion Step, the
// Compiler never use SIMD instructions!
// He uses just the FPU
inline bool CompareFloats(float Value1, float Value2)
{
    if(SSE)
    {
        __m128 val1, val2, val3;
        register int result;

        val1 = _mm_set1_ps(Value1);
        val2 = _mm_set1_ps(Value2);
        val3 = _mm_cmpeq_ps(val1, val2);

        result = _mm_movemask_ps(val3);

        // Result = 0x0 Not Equal, > 0x0 equal
        return (bool)result;

    }
    else
    {
        return fabs(Value1 - Value2) < 0.00001f;
    }
}

// Getting a Random Float Number
inline float GetRandomFloatNumber(int MAX)
{
    unsigned int rnd_val = 0;
    register int attempts = 10;
    register bool success = false;

    while(attempts-- && !success)
        success = _rdrand32_step(&rnd_val);

    if(!success)
        rnd_val = rand();

    float val =  (MAX) * ((float)rnd_val / UINT32_MAX);
    short sign = val < 0 ? -1 : 1;

    return floorf(fabs(val)*2.f) / 2.f * sign;
}

// Check if a char is an Number / Alphabet
bool CheckUnicode(char Check)
{
    if((Check >= 48 && Check <= 57) || (Check >= 97 && Check <= 122) || (Check >= 65 && Check <= 97) || Check == 32)
        return true;
    else
        return false;
}

// Number to a String
char* itoa(int value, char* result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

// Make Vector Addition
sfVector2f Vector2fAddition(sfVector2f Vec1, sfVector2f Vec2)
{
    sfVector2f Result;

    Result.x = Vec1.x + Vec2.x;
    Result.y = Vec1.y + Vec2.y;

    return Result;
}

