#include <scoreboard.h>
#include <Arduino.h>

TourneyMakerScoreboard *scoreboard = NULL;

void setup() {
  Serial.begin(9600);
  scoreboard = TourneyMakerScoreboard::setup("ESP32");
}

void loop() {
  Serial.println("loop");
  if (scoreboard->deviceConnected) {
    scoreboard->bumpScore();
  }
  delay(10000);
}
