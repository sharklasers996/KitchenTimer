#include "Arduino.h"
#include "LedActiveAnimator.h"

LedController _controller;
long lastRowLight;
uint8_t rowA;

void LedActiveAnimator::init(LedController controller)
{
    _controller = controller;
}

void LedActiveAnimator::rowAnim()
{
    int duration = millis() - lastRowLight;
    if (duration < 500)
    {
        return;
    }

    lastRowLight = millis();
    _controller.turnOnRow(rowA);
    rowA++;

    if (rowA > 10)
    {
        rowA = 0;
    }
}