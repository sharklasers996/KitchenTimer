#include <stdio.h>
#include "Timer.h"
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <SD.h>
#include <TMRpcm.h>
#include <SPI.h>

#include "./src/RotaryEncoder.h"
#include "./src/PushButton.h"
#include "./src/ClockModule.h"
#include "./src/LedController.h"
#include "./src/PhotoResistorPushButton.h"
#include "./src/LedActiveAnimator.h"
#include "./src/LedPassiveAnimator.h"
#include "./src/BigButtonLedController.h"
#include "./src/AudioPlayer.h"

#define ACTION_BUTTON_LED 8
#define ROTARY_PIN1 0
#define ROTARY_PIN2 A3
#define TIME_BUTTON A1
#define POWER_BUTTON A0
#define ACTION_BUTTON A2
#define LED_LATCH_PIN 3
#define LED_CLOCK_PIN 4
#define LED_DATA_PIN 2
#define SD_PIN 10
#define SPEAKER_PIN 9

LedController ledController(LED_LATCH_PIN, LED_CLOCK_PIN, LED_DATA_PIN);
RotaryEncoder rotaryEncoder(ROTARY_PIN1, ROTARY_PIN2);

PushButton timeButton(TIME_BUTTON);
PushButton actionButton(ACTION_BUTTON);
PushButton prPowerButton(POWER_BUTTON);

LedActiveAnimator activeAnimator;
LedPassiveAnimator passiveAnimator;

BigButtonLedController buttonLed;

AudioPlayer audio;

#ifndef __CLOCK_FUNCTIONS
#define __CLOCK_FUNCTIONS
typedef void (*GetSeconds)(long seconds);
typedef void (*GetCurrentAndDuration)(long current, long duration);
typedef void (*AlarmSettingStarted)();
#endif

#ifndef __CLOCK_STATES
#define __CLOCK_STATES

#define SETTING_TIME 0
#define SHOWING_TIME 3
#define SETTING_ALARM 4
#define RUNNING_ALARM 5
#define FINISHED_ALARM 6

#define TIMESETTING_HOUR 0
#define TIMESETTING_MINUTE 1
#define TIMESETTING_NONE 2

#endif

#define MACHINE_STATE_ON 1
#define MACHINE_STATE_OFF 0
byte machineState;
bool allowAutomaticPowerSwitch = true;

long lastPowerSwitchCheckAt = millis();
// speaker
// OUT = +
// GND = -

// big button
// 1 = GND
// 2 = A2
// 3 = D8

void setup()
{
    Serial.begin(9600);
    SD.begin(SD_PIN);

    audio.init(SPEAKER_PIN);

    initClock();

    buttonLed.init(ACTION_BUTTON_LED);

    activeAnimator.init(ledController);
    passiveAnimator.init(ledController);

    onAlarmSettingSecondsChanged(alarmSettingChanged);
    OnAlarmSecondsElapsed(alarmElapsed);
    OnAlarmSettingStarted(alarmSettingStarted);

    machineState = MACHINE_STATE_ON;
}

void loop()
{
    switch (machineState)
    {
    case MACHINE_STATE_ON:
        machineOnLoop();
        break;
    case MACHINE_STATE_OFF:
        machineOffLoop();
        break;
    }
}

void machineOffLoop()
{
    if (prPowerButton.isPushed())
    {
        machineState = MACHINE_STATE_ON;
        allowAutomaticPowerSwitch = true;
        return;
    }

    if (actionButton.isPushed())
    {
        byte timeSettingState = getTimeSettingState();
        if (timeSettingState == TIMESETTING_MINUTE)
        {
            rotateTimeSetting();
        }
        else
        {
            machineState = MACHINE_STATE_ON;
            allowAutomaticPowerSwitch = true;
            executeAlarmAction();
            return;
        }
    }

    int clockState = getclockModuleState();

    if (timeButton.isPushed())
    {
        rotateTimeSetting();
    }

    uint8_t value = rotaryEncoder.read();
    changeTime(value);
    updateClockModule();

    buttonLed.turnOff();
    ledController.turnOff();
}

void machineOnLoop()
{
    if (prPowerButton.isPushed())
    {
        machineState = MACHINE_STATE_OFF;
        allowAutomaticPowerSwitch = false;
        resetAlarm();
        return;
    }

    int clockState = getclockModuleState();

    if (timeButton.isPushed())
    {
        rotateTimeSetting();
    }

    if (actionButton.isPushed())
    {
        byte timeSettingState = getTimeSettingState();
        if (clockState == SETTING_TIME && timeSettingState == TIMESETTING_MINUTE)
        {
            rotateTimeSetting();
        }
        else
        {
            executeAlarmAction();
        }
    }

    uint8_t value = rotaryEncoder.read();
    changeTime(value);
    updateClockModule();

    if (clockState == RUNNING_ALARM)
    {
        activeAnimator.animateAlarmElapsed();
        buttonLed.animateTicking();
    }
    else if (clockState == SETTING_ALARM)
    {
        activeAnimator.animateAlarmSetting();
        buttonLed.turnOn();
    }
    else if (clockState == FINISHED_ALARM)
    {
        audio.startAlarm();
        activeAnimator.animateAlarmFinished();
        buttonLed.animateAlarm();
    }
    else if (clockState == SHOWING_TIME || clockState == SETTING_TIME)
    {
        audio.stopAlarm();
        passiveAnimator.animate();
        buttonLed.turnOff();
    }
}

void alarmSettingChanged(long s)
{
    activeAnimator.updateAlarmSetting(s);
}

void alarmElapsed(long current, long total)
{
    activeAnimator.updateAlarmElapsedSeconds(current, total);
}

void alarmSettingStarted()
{
    machineState = MACHINE_STATE_ON;
    allowAutomaticPowerSwitch = true;
    activeAnimator.reset();
}