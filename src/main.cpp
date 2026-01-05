#include <Arduino.h>
#include "Button.h"
#include <scoreboard.h>
#include "ScoreboardPrefs.h"
#include "BlinkingLED.h"
#include "DeepSleepTimer.h"

BlinkingLED led(2, 500); // LED on GPIO 2, blink every 500ms
DeepSleepTimer sleepTimer(GPIO_NUM_25, 1000 * 30);
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

class MyScoreboardStatusCallback : public ScoreboardStatusCallback
{
  void onConnected()
  {
    led.stop(false);
    sleepTimer.stop();
  }

  void onDisconnected()
  {
    led.stop(false);
    sleepTimer.start();
  }

  void onStartAdvertisement()
  {
    led.start();
    sleepTimer.start();
  }
};

static void onSingleClick(void *button_handle, void *usr_data)
{
  Serial.println("onSingleClick");
  sleepTimer.reset();
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

  ScoreboardPrefs::begin();

  scoreboard = TourneyMakerScoreboard::setup();
  scoreboard->scoreboardChangedCallback = new MyScoreReceivedCallback();
  scoreboard->scoreboardStatusCallback = new MyScoreboardStatusCallback();
  scoreboard->startAdvertising();

  Button *btn = new Button(GPIO_NUM_25, true);
  btn->attachSingleClickEventCb(&onSingleClick, NULL);
  btn->attachDoubleClickEventCb(&onDoubleClick, NULL);
  btn->attachLongPressStartEventCb(&onLongPressStart, NULL);

  Button *score2Up = new Button(GPIO_NUM_35, true);
  score2Up->attachSingleClickEventCb([](void *button_handle, void *usr_data)
                                     { scoreboard->bumpScore(0, 1); }, NULL);

  Button *score2Down = new Button(GPIO_NUM_34, true);
  score2Down->attachSingleClickEventCb([](void *button_handle, void *usr_data)
                                       { scoreboard->bumpScore(0, -1); }, NULL);

  Button *score1Up = new Button(GPIO_NUM_32, true);
  score1Up->attachSingleClickEventCb([](void *button_handle, void *usr_data)
                                     { scoreboard->bumpScore(1, 0); }, NULL);

  Button *score1Down = new Button(GPIO_NUM_26, true);
  score1Down->attachSingleClickEventCb([](void *button_handle, void *usr_data)
                                       { scoreboard->bumpScore(-1, 0); }, NULL);

  pinMode(5, OUTPUT);
  // digitalWrite(5, HIGH);
}

void loop()
{
  led.update();
  sleepTimer.update();
}
