#include <Arduino.h>
#include "Button.h"
#include <scoreboard.h>
#include "ScoreboardPrefs.h"
#include "BlinkingLED.h"
#include "DeepSleepTimer.h"

#include <ynvisible_scoreboard.h>
#include "Buzzer.h"
#include "BatteryManager.h"

BlinkingLED led(2, 500); // LED on GPIO 2, blink every 500ms
Buzzer buzzer(5, 100);
DeepSleepTimer sleepTimer(GPIO_NUM_25, 1000 * 60 * 60);
TourneyMakerScoreboard *scoreboard = NULL;

class MyScoreReceivedCallback : public ScoreboardChangedCallback
{
  void onScoreChanged(uint8_t score1, uint8_t score2)
  {
    Serial.println("score received in callback " + String(score1) + ":" + String(score2));
    setYnvisibleScore(score1, score2);
    buzzer.buzz();
    sleepTimer.reset();
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

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting up...");

  ScoreboardPrefs::begin();

  scoreboard = TourneyMakerScoreboard::setup();
  scoreboard->scoreboardChangedCallback = new MyScoreReceivedCallback();
  scoreboard->scoreboardStatusCallback = new MyScoreboardStatusCallback();
  scoreboard->startAdvertising();

  // right button on esp board
  Button *bootBtn = new Button(GPIO_NUM_0, false);
  bootBtn->attachLongPressStartEventCb([](void *button_handle, void *usr_data)
                                       { scoreboard->setScore(0, 0); }, NULL);
  bootBtn->attachSingleClickEventCb([](void *button_handle, void *usr_data)
                                    { scoreboard->setScore(1, 0); }, NULL);
  bootBtn->attachDoubleClickEventCb([](void *button_handle, void *usr_data)
                                    { scoreboard->setScore(0, 0); }, NULL);

  // reset hall sensor
  Button *resetBtn = new Button(GPIO_NUM_25, true);
  resetBtn->attachLongPressStartEventCb([](void *button_handle, void *usr_data)
                                        { scoreboard->setScore(0, 0); }, NULL);

  // left hall sensors
  Button *score1Up = new Button(GPIO_NUM_32, true);
  score1Up->attachPressDownEventCb([](void *button_handle, void *usr_data)
                                   { scoreboard->bumpScore(1, 0); }, NULL);

  Button *score1Down = new Button(GPIO_NUM_26, true);
  score1Down->attachPressDownEventCb([](void *button_handle, void *usr_data)
                                     { scoreboard->bumpScore(-1, 0); }, NULL);

  // right hall sensors
  Button *score2Up = new Button(GPIO_NUM_35, true);
  score2Up->attachPressDownEventCb([](void *button_handle, void *usr_data)
                                   { scoreboard->bumpScore(0, 1); }, NULL);

  Button *score2Down = new Button(GPIO_NUM_34, true);
  score2Down->attachPressDownEventCb([](void *button_handle, void *usr_data)
                                     { scoreboard->bumpScore(0, -1); }, NULL);
}

void loop()
{
  led.update();
  sleepTimer.update();
  Serial.println(String(analogRead(GPIO_NUM_33))); // Print the raw analog value from GPIO 33
}
