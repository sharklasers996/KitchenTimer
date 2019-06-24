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
    int readTime = millis() - _prLastReadAt;
    if (readTime < 250)
    {
        return false;
    }

    int value = analogRead(_prPin);
    if (value < 30)
    {
        _prLastReadAt = millis();
        return true;
    }

    return false;
}

bool PhotoResistorPushButton::isDark()
{
    int value = analogRead(_prPin);
    if (value < 100 && value > 50)
    {
        Serial.println(value);
        return true;
    }

    return false;
}