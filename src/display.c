#include <display.h>

#include <Arduino.h>

#include <utils/digit.h>
#include <utils/pins.h>
#include <utils/segments.h>
#include <utils/timer.h>

uint16_t localNumber = 0;
uint16_t remoteNumber = 0;

int8_t newDigit = 0;

int8_t digits[] = { 0, 0, 0, 0 };
int8_t digitModifiers[] = { 0, 0, 0, 0 };
int8_t targetDigits[] = { 0, 0, 0, 0 };

void testDisplay()
{
    for (int8_t i = 0; i < 4; i++)
    {
        shiftOut(LOCAL_DATA_PIN, LOCAL_CLOCK_PIN, MSBFIRST, segments[8] ^ 0xFF);
        shiftOut(REMOTE_DATA_PIN, REMOTE_CLOCK_PIN, MSBFIRST, segments[8]);
    }

    digitalWrite(LOCAL_LATCH_PIN, HIGH);
    digitalWrite(LOCAL_LATCH_PIN, LOW);

    digitalWrite(REMOTE_LATCH_PIN, HIGH);
    digitalWrite(REMOTE_LATCH_PIN, LOW);

    digitalWrite(CHANGED_LED_PIN, HIGH);
    digitalWrite(CHANGING_LED_PIN, HIGH);

    delay(2500);

    for (int8_t i = 0; i < 4; i++)
    {
        shiftOut(LOCAL_DATA_PIN, LOCAL_CLOCK_PIN, MSBFIRST, CLEAR_SEGMENT ^ 0xFF);
        shiftOut(REMOTE_DATA_PIN, REMOTE_CLOCK_PIN, MSBFIRST, CLEAR_SEGMENT);
    }

    digitalWrite(LOCAL_LATCH_PIN, HIGH);
    digitalWrite(LOCAL_LATCH_PIN, LOW);

    digitalWrite(REMOTE_LATCH_PIN, HIGH);
    digitalWrite(REMOTE_LATCH_PIN, LOW);

    digitalWrite(CHANGED_LED_PIN, LOW);
    digitalWrite(CHANGING_LED_PIN, LOW);
}

void clearDisplay()
{
    for (int8_t i = 0; i < 4; i++)
    {
        shiftOut(LOCAL_DATA_PIN, LOCAL_CLOCK_PIN, MSBFIRST, CLEAR_SEGMENT ^ 0xFF);
        shiftOut(REMOTE_DATA_PIN, REMOTE_CLOCK_PIN, MSBFIRST, CLEAR_SEGMENT);
    }

    digitalWrite(LOCAL_LATCH_PIN, HIGH);
    digitalWrite(LOCAL_LATCH_PIN, LOW);

    digitalWrite(REMOTE_LATCH_PIN, HIGH);
    digitalWrite(REMOTE_LATCH_PIN, LOW);

    digitalWrite(CHANGED_LED_PIN, LOW);
    digitalWrite(CHANGING_LED_PIN, LOW);
}

void initDisplay(Profile profile)
{
    localNumber = remoteNumber = min(profile.year, 9999);

    toDigits(digits, localNumber);

    for (int8_t i = 0; i < 4; i++)
    {
        shiftOut(LOCAL_DATA_PIN, LOCAL_CLOCK_PIN, MSBFIRST, segments[digits[i]] ^ 0xFF);
        shiftOut(REMOTE_DATA_PIN, REMOTE_CLOCK_PIN, MSBFIRST, segments[digits[i]]);
    }

    digitalWrite(LOCAL_LATCH_PIN, HIGH);
    digitalWrite(LOCAL_LATCH_PIN, LOW);

    digitalWrite(REMOTE_LATCH_PIN, HIGH);
    digitalWrite(REMOTE_LATCH_PIN, LOW);

    initTimer(CLOCK_DIV_256);
    SET_TIMER(0x8000);
}

void updateLocalDisplay(Profile profile)
{
    if (profile.year == localNumber) return;
    localNumber = min(profile.year, 9999);

    toDigits(digits, localNumber);

    for (int8_t i = 0; i < 4; i++)
    {
        shiftOut(LOCAL_DATA_PIN, LOCAL_CLOCK_PIN, MSBFIRST, segments[digits[i]] ^ 0xFF);
    }

    digitalWrite(LOCAL_LATCH_PIN, HIGH);
    digitalWrite(LOCAL_LATCH_PIN, LOW);

    if (localNumber != remoteNumber)
        digitalWrite(CHANGED_LED_PIN, HIGH);
    else
        digitalWrite(CHANGED_LED_PIN, LOW);
}

void updateRemoteDisplay(Profile profile)
{
    if (profile.year == remoteNumber) return;
    uint16_t targetYear = min(profile.year, 9999);
    int8_t direction = targetYear > remoteNumber ? 1 : -1;

    toDigits(digits, remoteNumber);
    toDigits(targetDigits, targetYear);

    digitalWrite(CHANGING_LED_PIN, HIGH);
    ENABLE_TIMER();

    for (int8_t j = 0; j < 4; j++)
    {
        for (int8_t i = 0; i < 10; i++)
        {
            newDigit = digits[j] + direction;
            if (newDigit < 0)
                digits[j] = 9;
            else if (newDigit > 9)
                digits[j] = 0;
            else
                digits[j] = newDigit;

            delay(1000 - 90 * i);
        }

        digitModifiers[j] = direction;
    }

    delay(profile.waitMs);

    for (int8_t j = 3; j >= 0; j--)
    {
        digitModifiers[j] = 0;

        for (int8_t i = 0; i < 10; i++)
        {
            newDigit = digits[j] + direction;
            if (newDigit < 0)
                digits[j] = 9;
            else if (newDigit > 9)
                digits[j] = 0;
            else
                digits[j] = newDigit;

            delay(100 + 90 * i);

            if (digits[j] == targetDigits[j]) break;
        }
    }

    DISABLE_TIMER();
    digitalWrite(CHANGING_LED_PIN, LOW);

    remoteNumber = targetYear;

    digitalWrite(CHANGED_LED_PIN, LOW);
}

ISR(TIMER1_OVF_vect)
{
    for (int8_t i = 0; i < 4; i++)
    {
        newDigit = digits[i] + digitModifiers[i];
        if (newDigit < 0)
            digits[i] = 9;
        else if (newDigit > 9)
            digits[i] = 0;
        else
            digits[i] = newDigit;
    }

    for (int8_t i = 0; i < 4; i++)
        shiftOut(REMOTE_DATA_PIN, REMOTE_CLOCK_PIN, MSBFIRST, segments[digits[i]]);

    digitalWrite(REMOTE_LATCH_PIN, HIGH);
    digitalWrite(REMOTE_LATCH_PIN, LOW);

    SET_TIMER(0x8000);
}
