#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <scoreboard.h>

BLEServer *pServer = NULL;
BLECharacteristic *scoreCharacteristic = NULL;
BLECharacteristic *colorCharacteristic = NULL;

#define SERVICE_UUID "621c7b43-a755-4456-b3e5-946a58bf20d9"
#define SCORE_CHARACTERISTIC_UUID "2a08da31-68a4-4047-92af-95145bb7bd07"
#define COLOR_CHARACTERISTIC_UUID "30087a03-fad4-4c3a-a4a8-56eabcbb8d79"

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
            scoreboard->connected();
        };

        void onDisconnect(BLEServer *pServer) {
            scoreboard->disconnected();
            delay(500);                   // give the bluetooth stack the chance to get things ready
            pServer->startAdvertising();  // restart advertising
            Serial.println("start advertising");
        }
};
  
  class ScoreCharacteristicCallbacks : public BLECharacteristicCallbacks {
    TourneyMakerScoreboard* scoreboard;
    
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        this->scoreboard->scoreReceived(value);
    }

    public:
    ScoreCharacteristicCallbacks(TourneyMakerScoreboard *scoreboard) {
            this->scoreboard = scoreboard;
        }

  };

  class ColorCharacteristicCallbacks : public BLECharacteristicCallbacks {
    TourneyMakerScoreboard* scoreboard;
    
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        this->scoreboard->colorReceived(value);
    }

    public:
    ColorCharacteristicCallbacks(TourneyMakerScoreboard *scoreboard) {
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
    scoreCharacteristic = pService->createCharacteristic(
        SCORE_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY
    );
    scoreCharacteristic->addDescriptor(new BLE2902());
    scoreCharacteristic->setCallbacks(new ScoreCharacteristicCallbacks(scoreboard));

    // todo: only read?
    colorCharacteristic = pService->createCharacteristic(
        COLOR_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    colorCharacteristic->addDescriptor(new BLE2902());
    colorCharacteristic->setCallbacks(new ColorCharacteristicCallbacks(scoreboard));

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

void TourneyMakerScoreboard::connected() {
    this->deviceConnected = true;
    Serial.println("connected");
    
    // // set initial score without notifying
    // uint8_t data[2];
    // memcpy(data, &this->score1, 1); 
    // memcpy(data + 1, &this->score2, 1);

    // scoreCharacteristic->setValue(data, sizeof(data)); 
}

void TourneyMakerScoreboard::disconnected() {
    this->deviceConnected = false;
    Serial.println("disconnected");
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

    scoreCharacteristic->setValue(data, sizeof(data));
    scoreCharacteristic->notify();
    Serial.println("new score sent: " + String(this->score1) + ":" + String(this->score2));
}

void TourneyMakerScoreboard::scoreReceived(std::string value) {
    if (value.length() < 2) {
        return;
    }

    uint8_t score1 = value[0];
    uint8_t score2 = value[1];

    this->score1 = score1;
    this->score2 = score2;
    Serial.println("new score received: " + String(this->score1) + ":" + String(this->score2));
    scoreboardChangedCallback->onScoreReceived(score1, score2);
}

void TourneyMakerScoreboard::colorReceived(std::string value) {
    uint8_t r1 = value[0];
    uint8_t g1 = value[1];
    uint8_t b1 = value[2];
    uint8_t r2 = value[3];
    uint8_t g2 = value[4];
    uint8_t b2 = value[5];
    uint32_t color1 = (r1 << 16) | (g1 << 8) | b1;
    uint32_t color2 = (r2 << 16) | (g2 << 8) | b2;
    Serial.println("new color received: " + String(color1) + " - " + String(color2));
    scoreboardChangedCallback->onColorReceived(color1, color2);

}
