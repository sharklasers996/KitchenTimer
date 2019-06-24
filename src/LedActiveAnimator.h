#include "Arduino.h"
#ifndef __LED_CONTROLLER
#include "./LedController.h"
#endif

class LedActiveAnimator
{
public:
    void init(LedController controller);
    void animateAlarmSetting();
    void animateAlarmElapsed();
    void updateAlarmSetting(long seconds);
    void updateAlarmElapsedSeconds(long current, long total);
    void animateAlarmFinished();

private:
    LedController _controller;
    long _lastAlarmSettingSeconds;
    byte _alarmSettingAnimationRow;
};