#include <Arduino.h>
#include "Button.h"
#include <scoreboard.h>
#include "ScoreboardPrefs.h"
#include "BlinkingLED.h"
#include "DeepSleepTimer.h"

BlinkingLED led(2, 500); // LED on GPIO 2, blink every 500ms
DeepSleepTimer sleepTimer(GPIO_NUM_0, 1000 * 10);
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
  sleepTimer.start();
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

  Button *btn = new Button(GPIO_NUM_0, false);
  btn->attachSingleClickEventCb(&onSingleClick, NULL);
  btn->attachDoubleClickEventCb(&onDoubleClick, NULL);
  btn->attachLongPressStartEventCb(&onLongPressStart, NULL);
}

void loop()
{
  led.update();
  sleepTimer.update();
}
