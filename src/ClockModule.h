#include <Arduino.h>
#include <stdio.h>
#include "RtcModule.h"
#include <Time.h>
#include "Timer.h"
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

class ClockModule
{
public:
    uint8_t state;

    ClockModule();
    void update();
    void rotateTimeSetting();
    void executeAlarmAction();
    void changeTime(uint8_t value);

private:
    uint8_t timeSetting;
    Adafruit_7segment digitDisplay;
    RtcModule RTC;

    int hours;
    int minutes;
    uint8_t seconds;
    int totalSeconds;

    uint8_t hoursDigit1;
    uint8_t hoursDigit2;
    uint8_t minutesDigit1;
    uint8_t minutesDigit2;
    uint8_t secondsDigit1;
    uint8_t secondsDigit2;

    bool blink;
    int lastBlinkAt;

    uint8_t alarmHours;
    int alarmMinutes;
    uint8_t alarmSeconds;
    long totalAlarmSeconds;

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
};