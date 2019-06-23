#include "Arduino.h"
#ifndef __LED_CONTROLLER
#include "./LedController.h"
#endif

class LedActiveAnimator
{
public:
    void init(LedController controller);
    void rowAnim();

private:
    long lastRowLight;
    uint8_t rowA;
    LedController _controller;
};