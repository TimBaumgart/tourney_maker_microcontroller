#include "BatteryManager.h"

// Constructor
BatteryManager::BatteryManager(int pin)
{
    this->pin = pin;
}

uint16_t BatteryManager::readRawValue()
{
    // Read the voltage from the specified pin and convert to a percentage
    uint16_t rawValue = analogRead(pin);
    return rawValue;
    // // Assuming a 3.3V reference and 12-bit ADC (0-4095)
    // float voltage = (rawValue / 4095.0) * 3.3;
    // // Convert voltage to percentage (assuming 3.0V is empty and 4.2V is full)
    // uint8_t percentage = (voltage - 3.0) / (4.2 - 3.0) * 100;
    // return percentage;
}

uint8_t BatteryManager::readPercentage()
{
    uint16_t rawValue = readRawValue();
    // Assuming a 4.0V reference and 12-bit ADC (0-4095)
    float voltage = (rawValue / 4095.0 / 2 * 3.3 );
    // Convert voltage to percentage (assuming 3.3V is empty and 4.0V is full)
    uint8_t percentage = (voltage - 3.0) / (4.1 ) * 100;
    // return percentage;
    Serial.println("Raw: " + String(rawValue) + " (" + String(percentage) + "%)");
    return rawValue;
    
}