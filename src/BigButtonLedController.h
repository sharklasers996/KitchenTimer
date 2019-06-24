#include "Arduino.h"

class BigButtonLedController
{
public:
    void init(int bigButtonLedPin);
    void turnOn();
    void turnOff();
    void animateTicking();
    void animateAlarm();

private:
    int _bigButtonLedPin;
    long _lastBigButtonAnimationAt;
};