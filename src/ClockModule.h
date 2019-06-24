#include <Arduino.h>
#include <stdio.h>
#include <DS1302.h>
#include <Time.h>
#include "Timer.h"
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#ifndef __CLOCK_FUNCTIONS
typedef void (*GetSeconds)(long seconds);
typedef void (*GetCurrentAndDuration)(long current, long duration);
typedef void (*AlarmSettingStarted)();
#endif

#ifndef __CLOCK_STATES
#define SETTING_TIME 0
#define SHOWING_TIME 3
#define SETTING_ALARM 4
#define RUNNING_ALARM 5
#define FINISHED_ALARM 6

#define TIMESETTING_HOUR 0
#define TIMESETTING_MINUTE 1
#define TIMESETTING_NONE 2
#endif

void onAlarmSettingSecondsChanged(GetSeconds callback);
void OnAlarmSecondsElapsed(GetCurrentAndDuration callback);
void OnAlarmSettingStarted(AlarmSettingStarted callback);

byte getclockModuleState();
byte getTimeSettingState();
void resetAlarm();

void initClock();
void updateClockModule();
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
void getDigits(int number, int &digit1, int &digit2);