#include "Buzzer.h"

// Constructor
Buzzer::Buzzer(int pin, unsigned long duration)
{
    this->pin = pin;
    this->duration = duration;
    pinMode(pin, OUTPUT);
}

void Buzzer::buzzFor(unsigned long duration)
{
    Serial.println("buzzing for " + String(duration) + "ms...");
    digitalWrite(pin, HIGH);
    delay(duration);
    digitalWrite(pin, LOW);
}

void Buzzer::buzz()
{
    buzzFor(duration);
}