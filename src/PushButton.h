#include "Arduino.h"

class PushButton
{
public:
    PushButton(int pin);
    bool isPushed();

private:
    int _pin;
    int _lastReadAt;
};