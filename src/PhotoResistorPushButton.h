#include "Arduino.h"

class PhotoResistorPushButton
{
public:
    PhotoResistorPushButton(int pin);
    bool isPushed();
    bool isDark();

private:
    int _prPin;
    long _prLastReadAt;
};