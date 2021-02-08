#include <utils/digit.h>

#include <math.h>

uint8_t countDigits(uint32_t number)
{
    return number > 0 ? ceil(log10(number)) : 1;
}

void toDigits(int8_t digits[], uint16_t number)
{
    int8_t* digit = digits;
    while (number > 0)
    {
        *digit = number % 10;
        number /= 10;
        digit++;
    }
}