#include <Arduino.h>

#define i2cAddress 8 // Define the I2C address of the device

void sendYnvisibleScore(const String &rawContent, int numModules, uint8_t i2c_address);