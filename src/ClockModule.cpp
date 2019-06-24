#include <Arduino.h>
#include <stdio.h>
#include <DS1302.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include "ClockModule.h"

uint8_t clockModuleState = SHOWING_TIME;
uint8_t timeSetting = TIMESETTING_NONE;

Adafruit_7segment digitDisplay = Adafruit_7segment();

// CE (RST), IO (DATA), CLK
DS1302 RTC(5, 6, 7);

int hours = 0;
int minutes = 0;
int seconds = 0;
int totalSeconds = 0;

int hoursDigit1 = 0;
int hoursDigit2 = 0;
int minutesDigit1 = 0;
int minutesDigit2 = 0;
int secondsDigit1 = 0;
int secondsDigit2 = 0;

bool blink = false;
long lastBlinkAt = millis();

int alarmHours = 0;
int alarmMinutes = 0;
int alarmSeconds = 0;
long totalAlarmSeconds = 0;

GetSeconds alarmSettingSecondsChanged;
GetCurrentAndDuration alarmSecondsElapsed;
AlarmSettingStarted alarmSettingStarted;

void onAlarmSettingSecondsChanged(GetSeconds callback)
{
    alarmSettingSecondsChanged = callback;
}

void OnAlarmSecondsElapsed(GetCurrentAndDuration callback)
{
    alarmSecondsElapsed = callback;
}

void OnAlarmSettingStarted(AlarmSettingStarted callback)
{
    alarmSettingStarted = callback;
}

byte getclockModuleState()
{
    return clockModuleState;
}

byte getTimeSettingState()
{
    return timeSetting;
}

void resetAlarm()
{
    clockModuleState = SHOWING_TIME;
}

bool isNightTime()
{
    return hours >= 22 || hours < 7;
}

void initClock()
{
    digitDisplay.begin(0x70);
    RTC.writeProtect(false);
    RTC.halt(false);

    clockModuleState = SHOWING_TIME;

    hours = 4;
    minutes = 20;
    saveTime();
}

void updateClockModule()
{
    if (clockModuleState == SHOWING_TIME)
    {
        printTime();
    }
    else if (clockModuleState == SETTING_TIME)
    {
        printSettingTime();
    }
    else if (clockModuleState == SETTING_ALARM)
    {
        printAlarmSettingTime();
    }
    else if (clockModuleState == RUNNING_ALARM)
    {
        printAlarmTime();
    }
    else if (clockModuleState == FINISHED_ALARM)
    {
        printBlinkingTime();
    }
}

void rotateTimeSetting()
{
    if (clockModuleState == RUNNING_ALARM)
    {
        return;
    }

    if (clockModuleState != SETTING_TIME)
    {
        timeSetting = TIMESETTING_HOUR;
        clockModuleState = SETTING_TIME;
        startBlink();
        return;
    }

    switch (timeSetting)
    {
    case TIMESETTING_HOUR:
        timeSetting = TIMESETTING_MINUTE;
        startBlink();
        break;
    case TIMESETTING_MINUTE:
        clockModuleState = SHOWING_TIME;
        startBlink();
        saveTime();
        break;
    }
}

void executeAlarmAction()
{
    if (clockModuleState == SETTING_ALARM)
    {
        startAlarm();
    }
    else if (clockModuleState == FINISHED_ALARM)
    {
        clockModuleState = SHOWING_TIME;
    }
    else if (clockModuleState == RUNNING_ALARM)
    {
        clockModuleState = SHOWING_TIME;
    }
}

void changeTime(uint8_t value)
{
    if (value != 0 && clockModuleState == SHOWING_TIME)
    {
        alarmSettingStarted();
        clockModuleState = SETTING_ALARM;
        alarmHours = 0;
        alarmMinutes = 0;
        alarmSeconds = 0;
        totalAlarmSeconds = 0;
    }

    if (value == 255)
    {
        increaseTime();
    }
    else if (value == 1)
    {
        decreaseTime();
    }
}

void increaseTime()
{
    if (clockModuleState == SETTING_TIME)
    {
        if (timeSetting == TIMESETTING_HOUR)
        {
            hours++;
            if (hours >= 24)
            {
                hours = 0;
            }
        }
        else if (timeSetting == TIMESETTING_MINUTE)
        {
            minutes++;
            if (minutes >= 60)
            {
                minutes = 0;
            }
        }

        getDigits(hours, hoursDigit1, hoursDigit2);
        getDigits(minutes, minutesDigit1, minutesDigit2);
    }
    else if (clockModuleState == SETTING_ALARM)
    {
        alarmMinutes++;
        if (alarmMinutes >= 60)
        {
            alarmHours++;
            if (alarmHours >= 23)
            {
                alarmHours = 23;
            }
            alarmMinutes = 0;
        }

        totalAlarmSeconds = alarmHours * 60 * 60;
        totalAlarmSeconds += alarmMinutes * 60;

        alarmSettingSecondsChanged(totalAlarmSeconds);
    }
}

void decreaseTime()
{
    if (clockModuleState == SETTING_TIME)
    {
        if (timeSetting == TIMESETTING_HOUR)
        {
            hours--;
            if (hours < 0)
            {
                hours = 23;
            }
        }
        else if (timeSetting == TIMESETTING_MINUTE)
        {
            minutes--;
            if (minutes < 0)
            {
                minutes = 59;
            }
        }

        getDigits(hours, hoursDigit1, hoursDigit2);
        getDigits(minutes, minutesDigit1, minutesDigit2);
    }
    else if (clockModuleState == SETTING_ALARM)
    {
        alarmMinutes--;
        if (alarmMinutes < 0)
        {
            alarmHours--;
            if (alarmHours <= 0)
            {
                alarmHours = 23;
            }
            alarmMinutes = 59;
        }

        totalAlarmSeconds = alarmHours * 60 * 60;
        totalAlarmSeconds += alarmMinutes * 60;

        alarmSettingSecondsChanged(totalAlarmSeconds);
    }
}

void printTime()
{
    if (blink)
    {
        blinkAll();
    }
    else
    {
        updateTimeVariables();

        digitDisplay.writeDigitNum(0, hoursDigit1, false);
        digitDisplay.writeDigitNum(1, hoursDigit2, false);

        bool drawColon = true;
        if (seconds % 2 == 0)
        {
            drawColon = false;
        }

        digitDisplay.drawColon(drawColon);
        digitDisplay.writeDigitNum(3, minutesDigit1, false);
        digitDisplay.writeDigitNum(4, minutesDigit2, false);
        digitDisplay.writeDisplay();
    }
}

void printSettingTime()
{
    if (blink)
    {
        if (timeSetting == TIMESETTING_HOUR)
        {
            blinkHours();
        }
        else if (timeSetting == TIMESETTING_MINUTE)
        {
            blinkMinutes();
        }
    }
    else
    {
        digitDisplay.writeDigitNum(0, hoursDigit1, false);
        digitDisplay.writeDigitNum(1, hoursDigit2, false);
        digitDisplay.drawColon(true);
        digitDisplay.writeDigitNum(3, minutesDigit1, false);
        digitDisplay.writeDigitNum(4, minutesDigit2, false);
        digitDisplay.writeDisplay();
    }
}

void printAlarmSettingTime()
{
    updateAlarmVariables();

    digitDisplay.writeDigitNum(0, hoursDigit1, false);
    digitDisplay.writeDigitNum(1, hoursDigit2, false);
    digitDisplay.writeDigitNum(3, minutesDigit1, false);
    digitDisplay.writeDigitNum(4, minutesDigit2, false);

    digitDisplay.drawColon(true);
    digitDisplay.writeDisplay();
}

void printAlarmTime()
{
    updateAlarm();
    updateAlarmVariables();

    if (alarmHours == 0)
    {
        digitDisplay.writeDigitNum(0, minutesDigit1, false);
        digitDisplay.writeDigitNum(1, minutesDigit2, false);
        digitDisplay.writeDigitNum(3, secondsDigit1, false);
        digitDisplay.writeDigitNum(4, secondsDigit2, false);
    }
    else
    {
        digitDisplay.writeDigitNum(0, hoursDigit1, false);
        digitDisplay.writeDigitNum(1, hoursDigit2, false);
        digitDisplay.writeDigitNum(3, minutesDigit1, false);
        digitDisplay.writeDigitNum(4, minutesDigit2, false);
    }

    bool drawColon = true;
    if (alarmSeconds % 2 == 0)
    {
        drawColon = false;
    }
    digitDisplay.drawColon(drawColon);
    digitDisplay.writeDisplay();
}

void printBlinkingTime()
{
    long blinkTime = millis() - lastBlinkAt;
    if (blinkTime < 500)
    {
        return;
    }

    if (blink)
    {
        digitDisplay.clear();
        blink = false;
    }
    else
    {
        digitDisplay.drawColon(true);
        digitDisplay.writeDigitNum(0, 0, false);
        digitDisplay.writeDigitNum(1, 0, false);
        digitDisplay.writeDigitNum(3, 0, false);
        digitDisplay.writeDigitNum(4, 0, false);
        blink = true;
    }

    digitDisplay.writeDisplay();

    lastBlinkAt = millis();
}

void startBlink()
{
    lastBlinkAt = millis();
    blink = true;
}

void blinkHours()
{
    long blinkTime = millis() - lastBlinkAt;
    if (blinkTime > 500)
    {
        blink = false;
    }

    digitDisplay.clear();
    digitDisplay.drawColon(true);
    digitDisplay.writeDigitNum(3, minutesDigit1, false);
    digitDisplay.writeDigitNum(4, minutesDigit2, false);
    digitDisplay.writeDisplay();
}

void blinkMinutes()
{
    long blinkTime = millis() - lastBlinkAt;
    if (blinkTime > 500)
    {
        blink = false;
    }

    digitDisplay.clear();
    digitDisplay.drawColon(true);
    digitDisplay.writeDigitNum(0, hoursDigit1, false);
    digitDisplay.writeDigitNum(1, hoursDigit2, false);
    digitDisplay.writeDisplay();
}

void blinkAll()
{
    long blinkTime = millis() - lastBlinkAt;
    if (blinkTime > 500)
    {
        blink = false;
    }

    digitDisplay.clear();
    digitDisplay.writeDisplay();
}

long alarmDuration = 0;
long lastAlarmUpdateSeconds = 0;
bool firstAlarm = false;
void startAlarm()
{
    lastAlarmUpdateSeconds = totalSeconds;
    clockModuleState = RUNNING_ALARM;
    alarmDuration = totalAlarmSeconds;

    firstAlarm = true;
    int secondsDifference = totalSeconds - lastAlarmUpdateSeconds;
}

void updateAlarm()
{
    updateTimeVariables();

    if (firstAlarm)
    {
        lastAlarmUpdateSeconds = totalSeconds + 1;
        firstAlarm = false;
        alarmSecondsElapsed(alarmDuration, alarmDuration);
    }

    int secondsDifference = totalSeconds - lastAlarmUpdateSeconds;
    if (secondsDifference <= 0)
    {
        return;
    }

    totalAlarmSeconds -= secondsDifference;

    alarmHours = (totalAlarmSeconds / 60) / 60;
    alarmMinutes = totalAlarmSeconds / 60;
    alarmSeconds = totalAlarmSeconds % 60;

    lastAlarmUpdateSeconds = totalSeconds;

    alarmSecondsElapsed(totalAlarmSeconds, alarmDuration);

    if (totalAlarmSeconds <= 0)
    {
        lastBlinkAt = millis() + 1000;
        blink = true;
        clockModuleState = FINISHED_ALARM;
    }
}

void saveTime()
{
    // Sunday, September 25, 2018 at 13:30:50.
    Time t(2018, 9, 25, hours, minutes, 00, Time::kTuesday);
    RTC.time(t);
}

void updateTimeVariables()
{
    Time t = RTC.time();

    hours = t.hr;
    minutes = t.min;
    seconds = t.sec;

    totalSeconds = hours * 60;
    totalSeconds += minutes * 60;
    totalSeconds += seconds;

    getDigits(t.hr, hoursDigit1, hoursDigit2);
    getDigits(t.min, minutesDigit1, minutesDigit2);
    getDigits(t.sec, secondsDigit1, secondsDigit2);
}

void updateAlarmVariables()
{
    getDigits(alarmHours, hoursDigit1, hoursDigit2);
    getDigits(alarmMinutes, minutesDigit1, minutesDigit2);
    getDigits(alarmSeconds, secondsDigit1, secondsDigit2);
}

void getDigits(int number, int &digit1, int &digit2)
{
    digit2 = number % 10;

    if (number < 10)
    {
        digit1 = 0;
    }
    else
    {
        digit1 = number;
        if (digit1 >= 100000000)
            digit1 /= 100000000;
        if (digit1 >= 10000)
            digit1 /= 10000;
        if (digit1 >= 100)
            digit1 /= 100;
        if (digit1 >= 10)
            digit1 /= 10;
    }
}