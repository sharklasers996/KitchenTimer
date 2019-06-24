#include <Arduino.h>
#include "PhotoResistorPushButton.h"

int _prPin;
long _prLastReadAt;

PhotoResistorPushButton::PhotoResistorPushButton(int pin)
{
    _prPin = pin;

    _prLastReadAt = millis();
}

bool PhotoResistorPushButton::isPushed()
{
    long readTime = millis() - _prLastReadAt;
    if (readTime < 250)
    {
        return false;
    }

    int value = analogRead(_prPin);
    if (value < 1)
    {
        _prLastReadAt = millis();
        return true;
    }

    return false;
}

bool PhotoResistorPushButton::isDark()
{
    int value = analogRead(_prPin);
    if (value < 100 && value > 1)
    {
        return true;
    }

    return false;
}