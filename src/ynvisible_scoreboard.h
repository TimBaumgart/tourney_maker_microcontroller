#include <Arduino.h>

#define i2cAddress 8 // Define the I2C address of the device

void setYnvisibleScore(uint8_t score1, uint8_t score2);
String padToLength(const String &input, char padChar, int numModules);