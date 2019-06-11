#include <Arduino.h>
#include "PushButton.h"

int _pin;
int _lastReadAt;

PushButton::PushButton(int pin)
{
    _pin = pin;

    pinMode(_pin, INPUT_PULLUP);
    _lastReadAt = millis();
}

bool PushButton::isPushed()
{
    int readTime = millis() - _lastReadAt;
    if (readTime < 250)
    {
        return false;
    }

    if (digitalRead(_pin) == LOW)
    {
        _lastReadAt = millis();
        return true;
    }

    return false;
}