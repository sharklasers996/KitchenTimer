#include "Arduino.h"

class RotaryEncoder
{
public:
  RotaryEncoder(int pin1, int pin2);
  int read();

private:
  int _pin1;
  int _pin2;
  int _lastStepAt;
  int _lastQuadValue;
  int readQuadValue();
  int getQuadDirection(int prevVal, int newVal);
};