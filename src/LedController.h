#include "Arduino.h"

#ifndef __LED_CONTROLLER
#define __LED_CONTROLLER
#endif

class LedController
{
public:
    LedController();
    LedController(int latchPin, int clockPin, int dataPin);
    void turnOnColumn(int column);
    void turnOnRow(int row);
    void turnOnSingle(int row, int column);

private:
    int _latchPin;
    int _clockPin;
    int _dataPin;
    void shiftData(byte rowData, byte colData);
};