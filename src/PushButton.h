#include "Arduino.h"

class PushButton
{
public:
    PushButton(int pin);
    bool isPushed();

private:
    int _pin;
    long _lastReadAt;
};