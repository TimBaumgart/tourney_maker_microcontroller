#include "BatteryManager.h"

// Constructor
BatteryManager::BatteryManager(int pin)
{
    this->pin = pin;
}

int BatteryManager::readRawValue()
{
    // Read the voltage from the specified pin and convert to a percentage
    int rawValue = analogRead(pin);
    return rawValue;
    // // Assuming a 3.3V reference and 12-bit ADC (0-4095)
    // float voltage = (rawValue / 4095.0) * 3.3;
    // // Convert voltage to percentage (assuming 3.0V is empty and 4.2V is full)
    // uint8_t percentage = (voltage - 3.0) / (4.2 - 3.0) * 100;
    // return percentage;
}

uint8_t BatteryManager::readPercentage()
{
    int rawValue = readRawValue();
    // Assuming a 4.0V reference and 12-bit ADC (0-4095)
    float voltage = (rawValue / 4095.0) * 2.0;
    // Convert voltage to percentage (assuming 3.3V is empty and 4.0V is full)
    uint8_t percentage = (voltage - 3.3) / (4.0 - 3.3) * 100;
    return percentage;
}