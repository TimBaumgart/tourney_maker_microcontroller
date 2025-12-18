#pragma once

#include <BLECharacteristic.h>
#include <BLEService.h>

class IdCharacteristic
{
public:
    explicit IdCharacteristic(BLEService *service);

private:
    BLECharacteristic *characteristic;
};
