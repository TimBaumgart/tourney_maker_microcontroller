#include <scoreboard.h>
#include <Arduino.h>
#include "Button.h"
#include "prefs.h"

TourneyMakerScoreboard *scoreboard = NULL;

class MyScoreReceivedCallback : public ScoreboardChangedCallback
{
  void onScoreReceived(uint8_t score1, uint8_t score2)
  {
    Serial.println("score received in callback " + String(score1) + ":" + String(score2));
  }

  void onColorReceived(uint32_t color1, uint32_t color2)
  {
    Serial.println("color received in callback " + String(color1) + ":" + String(color2));
  }
};

static void onSingleClick(void *button_handle, void *usr_data)
{
  Serial.println("onSingleClick");
  scoreboard->bumpScore(1, 0);
}

static void onDoubleClick(void *button_handle, void *usr_data)
{
  Serial.println("onDoubleClick");
  scoreboard->bumpScore(0, 1);
}

static void onLongPressStart(void *button_handle, void *usr_data)
{
  Serial.println("onLongPressStart");
  scoreboard->setScore(0, 0);
}

void setup()
{
  Serial.begin(9600);
  scoreboard = TourneyMakerScoreboard::setup("Tourney Maker Dev");
  scoreboard->scoreboardChangedCallback = new MyScoreReceivedCallback();

  Button *btn = new Button(GPIO_NUM_0, false);
  btn->attachSingleClickEventCb(&onSingleClick, NULL);
  btn->attachDoubleClickEventCb(&onDoubleClick, NULL);
  btn->attachLongPressStartEventCb(&onLongPressStart, NULL);

  ScoreboardPrefs::begin();
  std::string globalFieldId = ScoreboardPrefs::getGlobalFieldId();
}

void loop()
{
}
