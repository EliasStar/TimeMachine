#pragma once

#include <avr/io.h>
#include <stdint.h>

#define NO_CLOCK 0
#define CLOCK_DIV_1 1
#define CLOCK_DIV_8 2
#define CLOCK_DIV_64 3
#define CLOCK_DIV_256 4
#define CLOCK_DIV_1024 5

#define SET_TIMER(x) TCNT1 = x

#define ENABLE_TIMER() TIMSK1 = 1
#define DISABLE_TIMER() TIMSK1 = 0

void initTimer(uint8_t clock);