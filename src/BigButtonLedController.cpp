#include "Arduino.h"
#include "BigButtonLedController.h"

int _bigButtonLedPin;
long _lastBigButtonAnimationAt;

bool isOn = false;

void BigButtonLedController::init(int bigButtonLedPin)
{
    _bigButtonLedPin = bigButtonLedPin;
    pinMode(_bigButtonLedPin, OUTPUT);
    _lastBigButtonAnimationAt = millis();

    turnOff();
}

void BigButtonLedController::turnOff()
{
    digitalWrite(_bigButtonLedPin, LOW);
}

void BigButtonLedController::turnOn()
{
    digitalWrite(_bigButtonLedPin, HIGH);
}

void BigButtonLedController::animateTicking()
{
    long duration = millis() - _lastBigButtonAnimationAt;
    if (duration < 1000)
    {
        return;
    }

    _lastBigButtonAnimationAt = millis();

    isOn = !isOn;
    if (isOn)
    {
        turnOn();
    }
    else
    {
        turnOff();
    }
}

void BigButtonLedController::animateAlarm()
{
    long duration = millis() - _lastBigButtonAnimationAt;
    if (duration < 200)
    {
        return;
    }

    _lastBigButtonAnimationAt = millis();

    isOn = !isOn;
    if (isOn)
    {
        turnOn();
    }
    else
    {
        turnOff();
    }
}
