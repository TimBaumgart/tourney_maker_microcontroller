#include <scoreboard.h>
#include <Arduino.h>

TourneyMakerScoreboard *scoreboard = NULL;

class MyScoreReceivedCallback : public ScoreReceivedCallback {
    void onScoreReceived(uint8_t score1, uint8_t score2) {
      Serial.println("score received in callback " + String(score1) + ":" + String(score2));
    }
};

void setup() {
  Serial.begin(9600);
  scoreboard = TourneyMakerScoreboard::setup("Tourney Maker Dev");
  scoreboard->scoreReceivedCallback = new MyScoreReceivedCallback();
}

void loop() {
  Serial.println("loop");
  if (scoreboard->deviceConnected) {
    scoreboard->bumpScore();
  }
  delay(10000);
}
