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

#define ROTARY_PIN1 0
#define ROTARY_PIN2 A3
#define TIME_BUTTON A1
#define POWER_BUTTON A0
#define ACTION_BUTTON A2
#define LED_LATCH_PIN 3
#define LED_CLOCK_PIN 4
#define LED_DATA_PIN 2
#define SD_PIN 10

LedController ledController(LED_LATCH_PIN, LED_CLOCK_PIN, LED_DATA_PIN);
RotaryEncoder rotaryEncoder(ROTARY_PIN1, ROTARY_PIN2);

PushButton timeButton(TIME_BUTTON);
PushButton actionButton(ACTION_BUTTON);
PhotoResistorPushButton prPowerButton(POWER_BUTTON);

LedActiveAnimator activeAnimator;

TMRpcm tmrpcm;

#ifndef __CLOCK_FUNCTIONS
#define __CLOCK_FUNCTIONS
typedef void (*GetSeconds)(long seconds);
typedef void (*GetCurrentAndDuration)(long current, long duration);
#endif

void setup()
{
    Serial.begin(9600);

    tmrpcm.speakerPin = 9;
    tmrpcm.setVolume(4);

    SD.begin(SD_PIN);
    initClock();

    pinMode(8, OUTPUT);
    digitalWrite(8, HIGH);

    activeAnimator.init(ledController);

    onAlarmSettingSecondsChanged(alarmSChanged);
    OnAlarmSecondsElapsed(alarmElapsed);
}

void loop()
{
    if (timeButton.isPushed())
    {
        Serial.println("time!");
        rotateTimeSetting();
    }

    if (prPowerButton.isPushed())
    {
        Serial.println("power!");
    }

    if (prPowerButton.isDark())
    {
        Serial.println("dark!");
    }

    if (actionButton.isPushed())
    {
        Serial.print("action at ");
        Serial.println(millis());

        executeAlarmAction();
    }

    uint8_t value = rotaryEncoder.read();
    changeTime(value);
    updateClockModule();

    activeAnimator.animate();
}

void alarmSChanged(long s)
{
    activeAnimator.animateAlarmSetting(s);
}

void alarmElapsed(long current, long total)
{
    activeAnimator.animateAlarmElapsed(current, total);
}