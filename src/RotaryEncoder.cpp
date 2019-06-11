#include "RotaryEncoder.h"
#include <Arduino.h>

int CLOCKWISE = 1;
int COUNTERCLOCKWISE = -1;

int _pin1;
int _pin2;

int _lastStepAt;
int _lastQuadValue;

int _counterClockWiseCount;
int _clockWiseCount;

RotaryEncoder::RotaryEncoder(int pin1, int pin2)
{
    _pin1 = pin1;
    _pin2 = pin2;

    pinMode(_pin1, INPUT);
    digitalWrite(_pin1, HIGH);
    pinMode(_pin2, INPUT);
    digitalWrite(_pin2, HIGH);

    _lastQuadValue = readQuadValue();
    _lastStepAt = millis();
}

int RotaryEncoder::read()
{
    bool doReturnValue = false;
    int newQuadValue = readQuadValue();
    int quadDirection = getQuadDirection(_lastQuadValue, newQuadValue);

    int duration = millis() - _lastStepAt;
    if (duration <= 5)
    {
        return 0;
    }

    if (quadDirection == CLOCKWISE)
    {
        _counterClockWiseCount = 0;
        _clockWiseCount++;

        if (_clockWiseCount == 3)
        {
            doReturnValue = true;
            _clockWiseCount = 0;
        }
        _lastStepAt = millis();
    }

    if (quadDirection == COUNTERCLOCKWISE)
    {
        _clockWiseCount = 0;
        _counterClockWiseCount++;

        if (_counterClockWiseCount == 3)
        {
            doReturnValue = true;
            _counterClockWiseCount = 0;
        }
        _lastStepAt = millis();
    }

    if (quadDirection == 999)
    {
        _lastStepAt = millis();
    }

    _lastQuadValue = newQuadValue;

    if (doReturnValue)
    {
        return quadDirection;
    }

    return 0;
}

int RotaryEncoder::readQuadValue()
{
    int value = digitalRead(_pin1);
    value = value * 2 + digitalRead(_pin2);
    return value;
}

int RotaryEncoder::getQuadDirection(int previousValue, int newValue)
{
    if (newValue == 3)
    {
        newValue = 2;
    }
    else if (newValue == 2)
    {
        newValue = 3;
    }

    if (previousValue == 3)
    {
        previousValue = 2;
    }
    else if (previousValue == 2)
    {
        previousValue = 3;
    }

    int quadDirection = previousValue - newValue;

    if (abs(quadDirection) == 2)
    {
        quadDirection = 999;
    }

    return quadDirection;
}