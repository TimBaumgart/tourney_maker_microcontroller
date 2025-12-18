#include "IdCharacteristic.h"
#include <BLE2902.h>
#include "ScoreboardPrefs.h"

#define ID_CHARACTERISTIC_UUID "e8b0bae2-630a-4726-a9ec-ec7d562420ba"

class IdCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *c) override
    {
        std::string value = c->getValue();
        ScoreboardPrefs::setGlobalFieldId(value);
    }
};

IdCharacteristic::IdCharacteristic(BLEService *service)
{
    characteristic = service->createCharacteristic(
        ID_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);

    characteristic->addDescriptor(new BLE2902());
    characteristic->setCallbacks(new IdCallbacks());
}
