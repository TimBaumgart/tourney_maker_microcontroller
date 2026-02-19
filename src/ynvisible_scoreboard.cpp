#include <Arduino.h>
#include <Wire.h>

String padToLength(const String &input, char padChar, int numModules)
{
    if (input.length() >= numModules)
        return input;
    return padToLength(String(padChar) + input, padChar, numModules);
}

void sendYnvisibleScore(const String &rawContent, int numModules, uint8_t i2c_address)
{
    String content = padToLength(rawContent, '0', numModules);

    Serial.println("sending data '" + content + "'...");

    // Define buffer size as 5 header bytes + numModules characters + 3 for checksum and ETX
    int bufferSize = 5 + numModules + 3;
    uint8_t buffer[bufferSize];

    // Fill header bytes
    buffer[0] = 0x02;                    // STX (Start of Text)
    buffer[1] = ((numModules + 2) >> 8); // Length High byte
    buffer[2] = (numModules + 2) & 0xFF; // Length Low byte
    buffer[3] = numModules;              // Number of modules
    buffer[4] = 0x01;                    // Mode (Segment mode by default)

    // Fill content in ASCII values, adding ASCII 32 (space) if content is shorter than numModules
    int contentSize = content.length();
    for (int i = 0; i < numModules; i++)
    {
        buffer[5 + i] = (i < contentSize) ? content[i] : 32; // ASCII 32 for space
    }

    // Compute checksum from buffer[3] to buffer[numModules + 4]
    uint16_t checksum = 0;
    for (int i = 3; i < 5 + numModules; i++)
    {
        checksum += buffer[i];
    }

    // Add checksum and end character to buffer
    buffer[5 + numModules] = (checksum >> 8); // Checksum High byte
    buffer[6 + numModules] = checksum & 0xFF; // Checksum Low byte
    buffer[7 + numModules] = 0x03;            // ETX (End of Text)

    // Send data over I2C
    Wire.begin();
    Wire.setClock(10000);
    Wire.beginTransmission(i2c_address);
    for (int i = 0; i < bufferSize; i++)
    {
        Wire.write(buffer[i]);
        if (i != 0)
            Serial.print("-");
        Serial.print(buffer[i]);
        delay(10);
    }
    Serial.println();
    Wire.endTransmission();
}