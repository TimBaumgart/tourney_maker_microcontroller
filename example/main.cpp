#include <scoreboard.h>
#include <Arduino.h>

TourneyMakerScoreboard *scoreboard = NULL;

class MyScoreReceivedCallback : public ScoreboardChangedCallback {
    void onScoreReceived(uint8_t score1, uint8_t score2) {
      Serial.println("score received in callback " + String(score1) + ":" + String(score2));
    }

    void onColorReceived(uint32_t color1, uint32_t color2) {
      Serial.println("color received in callback " + String(color1) + ":" + String(color2));
    }
};

void setup() {
  Serial.begin(9600);
  scoreboard = TourneyMakerScoreboard::setup("Tourney Maker Dev");
  scoreboard->scoreboardChangedCallback = new MyScoreReceivedCallback();
}

void loop() {
  Serial.println("loop");
  if (scoreboard->deviceConnected) {
    scoreboard->bumpScore();
  }
  delay(10000);
}
