#include "Arduino.h"
#ifndef __LED_CONTROLLER
#include "./LedController.h"
#endif

class LedActiveAnimator
{
public:
    void init(LedController controller);
    void rowAnim();
    void animateAlarmSetting(long seconds);
    void animateAlarmElapsed(long current, long total);
    void animate();

private:
    long lastRowLight;
    uint8_t rowA;
    LedController _controller;
    long _lastAlarmSettingSeconds;
    byte _alarmSettingAnimationRow;
};