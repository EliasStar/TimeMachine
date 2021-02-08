#include <utils/timer.h>

void initTimer(uint8_t clock)
{
    TIMSK1 = 0;
    TIFR1 = 0xFF;

    TCCR1A = 0;
    TCCR2B = clock;
    TCCR1C = 0;
}
