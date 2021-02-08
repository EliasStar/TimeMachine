#include <Arduino.h>

//#define DEBUG

#include <animation.h>
#include <display.h>
#include <profile.h>

#include <utils/pins.h>

void setup()
{
    pinMode(CHANGED_LED_PIN, OUTPUT);
    pinMode(CHANGING_LED_PIN, OUTPUT);

    pinMode(LOCAL_CLOCK_PIN, OUTPUT);
    pinMode(LOCAL_LATCH_PIN, OUTPUT);
    pinMode(LOCAL_DATA_PIN, OUTPUT);

    pinMode(REMOTE_CLOCK_PIN, OUTPUT);
    pinMode(REMOTE_LATCH_PIN, OUTPUT);
    pinMode(REMOTE_DATA_PIN, OUTPUT);

    pinMode(ENTER_PIN, INPUT_PULLUP);
    pinMode(FORWARD_PIN, INPUT_PULLUP);
    pinMode(BACKWARD_PIN, INPUT_PULLUP);

    clearDisplay();

    load();

#ifdef DEBUG
    testDisplay();
    delay(1000);
#else
    displayStartUp();
#endif

    initDisplay(current());
    sei();
}

void loop()
{
    if (digitalRead(FORWARD_PIN) == LOW)
    {
        updateLocalDisplay(next());
        delay(300);
    }

    if (digitalRead(BACKWARD_PIN) == LOW)
    {
        updateLocalDisplay(previous());
        delay(300);
    }

    if (digitalRead(ENTER_PIN) == LOW)
    {
        updateRemoteDisplay(current());
        save();
    }

    delay(5);
}
