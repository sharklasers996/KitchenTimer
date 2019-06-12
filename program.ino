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
PushButton powerButton(POWER_BUTTON);
PushButton actionButton(ACTION_BUTTON);

TMRpcm tmrpcm;

void setup()
{
    Serial.begin(9600);

    tmrpcm.speakerPin = 9;
    tmrpcm.setVolume(4);

    SD.begin(SD_PIN);
    initClock();
}

void loop()
{
    testingTimerMethod();
}

void testingTimerMethod()
{
    if (timeButton.isPushed())
    {
        rotateTimeSetting();
    }

    if (powerButton.isPushed())
    {
        Serial.println("power!");
    }

    if (actionButton.isPushed())
    {
        executeAlarmAction();
    }

    uint8_t value = rotaryEncoder.read();
    changeTime(value);
    update();

    rowAnim();
}

uint8_t rowA = 1;
int lastRowLight = millis();

void rowAnim()
{
    int duration = millis() - lastRowLight;
    if (duration < 500)
    {
        return;
    }

    lastRowLight = millis();
    ledController.turnOnRow(rowA);
    rowA++;

    if (rowA > 10)
    {
        rowA = 0;
    }
}