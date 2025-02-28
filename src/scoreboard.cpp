#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <scoreboard.h>

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;

#define SERVICE_UUID "621c7b43-a755-4456-b3e5-946a58bf20d9"
#define CHARACTERISTIC_UUID "2a08da31-68a4-4047-92af-95145bb7bd07"

TourneyMakerScoreboard::TourneyMakerScoreboard(std::string name) {
    this->name = name;
}

class MyServerCallbacks : public BLEServerCallbacks {
    TourneyMakerScoreboard* scoreboard;

    public:
        MyServerCallbacks(TourneyMakerScoreboard *scoreboard) {
            this->scoreboard = scoreboard;
        }
        void onConnect(BLEServer *pServer) {
            scoreboard->deviceConnected = true;
            Serial.println("connected");
        };

        void onDisconnect(BLEServer *pServer) {
            scoreboard->deviceConnected = false;
            Serial.println("disconnected");
            delay(500);                   // give the bluetooth stack the chance to get things ready
            pServer->startAdvertising();  // restart advertising
            Serial.println("start advertising");
        }
};
  
  class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
    TourneyMakerScoreboard* scoreboard;
    
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
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

TourneyMakerScoreboard* TourneyMakerScoreboard::setup(std::string name) {
    Serial.println(("Initializing scoreboard " + name + "...").c_str());
    TourneyMakerScoreboard *scoreboard = new TourneyMakerScoreboard(name);

    // Create the BLE Device
    BLEDevice::init(name);

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(scoreboard));

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

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);

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
    scoreReceivedCallback->onScoreReceived(score1, score2);
}
