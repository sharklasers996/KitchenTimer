#include <Arduino.h>
#include <stdio.h>
#include "RtcModule.h"
#include <Time.h>
#include "Timer.h"
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

void initClock();
void update();
void rotateTimeSetting();
void executeAlarmAction();
void changeTime(uint8_t value);

void increaseTime();
void decreaseTime();
void printTime();
void printSettingTime();
void printAlarmSettingTime();
void printAlarmTime();
void printBlinkingTime();
void startBlink();
void blinkHours();
void blinkMinutes();
void blinkAll();
void startAlarm();
void updateAlarm();
void saveTime();
void updateTimeVariables();
void updateAlarmVariables();
void getDigits(uint8_t number, uint8_t &digit1, uint8_t &digit2);