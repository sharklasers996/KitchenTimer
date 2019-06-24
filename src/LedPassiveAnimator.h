#include "Arduino.h"
#ifndef __LED_CONTROLLER
#include "./LedController.h"
#endif

class LedPassiveAnimator
{
public:
    void init(LedController controller);
    void animate();

private:
    byte animationIndex;
    bool animationFinished;
    long lastAnimationAt;
    LedController _ledControllerPassive;
    long durationSinceLastAnimation();
    void setLastAnimationAtNow();
    void setAnimationAsFinished();
    void animation0();
    void animation1();
    void animation2();
};