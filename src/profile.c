#include <profile.h>

#include <avr/eeprom.h>

#define PROFILE_COUNT 4

const Profile profiles[PROFILE_COUNT] = {
    { 1884, 30000 },
    { 1940, 30000 },
    { 2040, 30000 },
    { 8600, 30000 }
};

int8_t currentProfile = 0;

void save()
{
    eeprom_write_byte(INDEX_ADDR, currentProfile);
}

void load()
{
    currentProfile = (int8_t) eeprom_read_byte(INDEX_ADDR);
    if (currentProfile >= PROFILE_COUNT || currentProfile < 0) currentProfile = 0;
}

Profile next()
{
    currentProfile++;
    if (currentProfile >= PROFILE_COUNT) currentProfile = 0;
    return profiles[currentProfile];
}

Profile current()
{
    return profiles[currentProfile];
}

Profile previous()
{
    currentProfile--;
    if (currentProfile < 0) currentProfile = PROFILE_COUNT - 1;
    return profiles[currentProfile];
}