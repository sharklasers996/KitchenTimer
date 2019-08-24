#include <Arduino.h>
#include "PhotoResistorPushButton.h"

int _prPin;
long _prLastReadAt;
long _lastLightCheckAt;
bool _lastLighValue = false;

PhotoResistorPushButton::PhotoResistorPushButton(int pin)
{
    _prPin = pin;

    _prLastReadAt = millis();
    _lastLightCheckAt = millis();
}

bool PhotoResistorPushButton::isPushed()
{
    int value = digitalRead(_prPin);
    return value == 1;

    // long readTime = millis() - _prLastReadAt;
    // if (readTime < 250)
    // {
    //     return false;
    // }

    // int value = analogRead(_prPin);
    // if (value < 1)
    // {
    //     _prLastReadAt = millis();
    //     return true;
    // }

    // return false;
}

bool PhotoResistorPushButton::isDark()
{
    int value = analogRead(_prPin);

    long time = millis() - _lastLightCheckAt;
    if (time > 5000)
    {
        if (value < 100 && value > 1)
        {
            _lastLighValue = true;
        }
        else
        {
            _lastLighValue = false;
        }

        _lastLightCheckAt = millis();
    }

    return _lastLighValue;
}

// bool PhotoResistorPushButton::isDark()
// {
//     int value = analogRead(_prPin);

//     long time = millis() - _lastLightCheckAt;
//     if (time > 100)
//     {
//         Serial.println(value);
//         _lastLightCheckAt = millis();
//     }

//     if (value < 100 && value > 1)
//     {
//         Serial.print(value);
//         Serial.println(" returning");
//         return true;
//     }

//     return false;
// }