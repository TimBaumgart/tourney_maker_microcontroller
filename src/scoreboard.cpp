#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <scoreboard.h>

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;

#define SERVICE_UUID "621c7b43-a755-4456-b3e5-946a58bf20d9"
#define CHARACTERISTIC_UUID "2a08da31-68a4-4047-92af-95145bb7bd07"

TourneyMakerScoreboard::TourneyMakerScoreboard(String name) {
    this->name = name;
}


class MyServerCallbacks : public BLEServerCallbacks {
    public:
        void onConnect(BLEServer *pServer) {
            deviceConnected = true;
        };

        void onDisconnect(BLEServer *pServer) {
            deviceConnected = false;
        }
};
  
  class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
    TourneyMakerScoreboard* scoreboard;
    
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::__cxx11::string value = pCharacteristic->getValue();
        if (value.length() < 2) {
            return;
        }
    
        this->scoreboard->scoreReceived(value[0], value[1]);
    }

    public:
        MyCharacteristicCallbacks(TourneyMakerScoreboard *scoreboard) {
            this->scoreboard = scoreboard;
        }

  };

TourneyMakerScoreboard* TourneyMakerScoreboard::setup(String name) {

    TourneyMakerScoreboard *scoreboard = new TourneyMakerScoreboard(name);

    // Create the BLE Device
    BLEDevice::init("ESP32");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
    );
    pCharacteristic->setCallbacks(new MyCharacteristicCallbacks(scoreboard));

    // Creates BLE Descriptor 0x2902: Client Characteristic Configuration Descriptor (CCCD)
    pCharacteristic->addDescriptor(new BLE2902());
    // // Adds also the Characteristic User Description - 0x2901 descriptor
    // descriptor_2904 = new BLE2904();
    // descriptor_2904->setDescription("My own description for this characteristic.");
    // descriptor_2904->setAccessPermissions(ESP_GATT_PERM_READ);  // enforce read only - default is Read|Write
    // pCharacteristic->addDescriptor(descriptor_2904);

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");

    return scoreboard;
}

void TourneyMakerScoreboard::bumpScore() {
    this->setScore(this->score1 + 1, this->score2 + 1);
}

void TourneyMakerScoreboard::setScore(uint8_t score1, uint8_t score2) {
    this->score1 = score1;
    this->score2 = score2;

    uint8_t data[2];
    memcpy(data, &this->score1, 1);
    memcpy(data + 1, &this->score2, 1);

    pCharacteristic->setValue(data, sizeof(data)); 
    pCharacteristic->notify();
    Serial.println("new score sent: " + String(this->score1) + ":" + String(this->score2));
}

void TourneyMakerScoreboard::scoreReceived(uint8_t score1, uint8_t score2) {
    this->score1 = score1;
    this->score2 = score2;
    Serial.println("new score received: " + String(this->score1) + ":" + String(this->score2));
}

