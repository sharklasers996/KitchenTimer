#include "Arduino.h"
#include "LedPassiveAnimator.h"

LedController _ledControllerPassive;

#define ANIMATION_COUNT 2
#define ANIMATION_DELAY 200

byte animationIndex;
bool animationFinished = false;

long lastAnimationAt = millis();

void LedPassiveAnimator::init(LedController controller)
{
    _ledControllerPassive = controller;
    animationIndex = 0;
}

void LedPassiveAnimator::animate()
{
    if (animationFinished)
    {
        animationIndex++;
        if (animationIndex > ANIMATION_COUNT)
        {
            animationIndex = 0;
        }

        animationFinished = false;
    }

    switch (animationIndex)
    {
    case 0:
        animation0();
        break;
    case 1:
        animation1();
        break;
    case 2:
        animation2();
        break;
    }
}

long LedPassiveAnimator::durationSinceLastAnimation()
{
    return millis() - lastAnimationAt;
}

void LedPassiveAnimator::setLastAnimationAtNow()
{
    lastAnimationAt = millis();
}

void LedPassiveAnimator::setAnimationAsFinished()
{
    animationFinished = true;
    _ledControllerPassive.turnOff();
    lastAnimationAt = lastAnimationAt - ANIMATION_DELAY;
}

byte animationLedIndex = 0;
void LedPassiveAnimator::animation0()
{
    long duration = durationSinceLastAnimation();
    if (duration < ANIMATION_DELAY)
    {
        return;
    }

    setLastAnimationAtNow();

    animationLedIndex++;
    if (animationLedIndex > 60)
    {
        animationLedIndex = 0;
        setAnimationAsFinished();
        return;
    }

    int ledIndex = 0;
    for (int row = 1; row <= 10; row++)
    {
        for (int col = 1; col <= 6; col++)
        {
            ledIndex++;

            if (ledIndex >= animationLedIndex)
            {
                _ledControllerPassive.turnOnSingle(row, col);
                return;
            }
        }
    }
}

void LedPassiveAnimator::animation1()
{
    long duration = durationSinceLastAnimation();
    if (duration > ANIMATION_DELAY)
    {
        setLastAnimationAtNow();

        animationLedIndex++;
        if (animationLedIndex > 60)
        {
            animationLedIndex = 0;
            setAnimationAsFinished();
            return;
        }
    }

    int ledsOn = 0;
    for (int row = 1; row <= 10; row++)
    {
        for (int col = 1; col <= 6; col++)
        {
            _ledControllerPassive.turnOnSingle(row, col);
            ledsOn++;

            if (ledsOn >= animationLedIndex)
            {
                return;
            }
        }
    }
}

int animation2LedIndex = 60;
void LedPassiveAnimator::animation2()
{
    long duration = durationSinceLastAnimation();
    if (duration > ANIMATION_DELAY)
    {
        setLastAnimationAtNow();

        animation2LedIndex--;
        if (animation2LedIndex <= 0)
        {
            animation2LedIndex = 60;
            setAnimationAsFinished();
            return;
        }
    }

    byte turnedOn = 0;

    byte fullRowsToTurnOn = animation2LedIndex / 6;
    byte add = 0;
    if (fullRowsToTurnOn > 0)
    {
        add = 1;
    }

    for (int row = 10; row > 10 - fullRowsToTurnOn + add; row--)
    {
        turnedOn += 6;
        _ledControllerPassive.turnOnRow(row);
    }

    if (turnedOn >= animation2LedIndex)
    {
        return;
    }

    for (int row = 10 - fullRowsToTurnOn + add; row > 0; row--)
    {
        for (int col = 6; col > 0; col--)
        {
            _ledControllerPassive.turnOnSingle(row, col);

            turnedOn++;
            if (turnedOn >= animation2LedIndex)
            {
                return;
            }
        }
    }
}