#include <Arduino.h>
#include "scoreboard.h"

TourneyMakerScoreboard *scoreboard = NULL;

void setup() {
  Serial.begin(9600);
  scoreboard = TourneyMakerScoreboard::setup("Dev Scoreboard Tim");
}

void loop() {
  Serial.println("loop");
  scoreboard->bumpScore();
  delay(10000);
}
