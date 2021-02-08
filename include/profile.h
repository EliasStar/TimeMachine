#pragma once

#include <stdint.h>

#define INDEX_ADDR 0

typedef struct
{
    uint16_t year;
    uint32_t waitMs;
} Profile;

void save();
void load();

Profile next();
Profile current();
Profile previous();