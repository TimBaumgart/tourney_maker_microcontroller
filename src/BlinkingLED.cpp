#include "BlinkingLED.h"

// Constructor
BlinkingLED::BlinkingLED(int ledPin, unsigned long blinkInterval)
{
    pin = ledPin;
    interval = blinkInterval;
    lastToggle = 0;
    state = false;
    enabled = false;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

// Start blinking
void BlinkingLED::start()
{
    Serial.println("start led");
    enabled = true;
}

// Stop blinking
void BlinkingLED::stop(boolean on)
{

    Serial.println("stop led");
    enabled = false;
    if (on)
    {
        digitalWrite(pin, HIGH);
    }
    else
    {

        digitalWrite(pin, LOW);
    }
    state = false;
}

// Update LED, non-blocking
void BlinkingLED::update()
{
    if (!enabled)
        return;

    unsigned long currentMillis = millis();
    if (currentMillis - lastToggle >= interval)
    {
        state = !state;
        digitalWrite(pin, state ? HIGH : LOW);
        lastToggle = currentMillis;
    }
}

// Set a new blink interval
void BlinkingLED::setInterval(unsigned long blinkInterval)
{
    interval = blinkInterval;
}
