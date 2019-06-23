#include <Arduino.h>
#include "LedController.h"

int _latchPin;
int _clockPin;
int _dataPin;

LedController::LedController()
{
}

LedController::LedController(int latchPin, int clockPin, int dataPin)
{
    _latchPin = latchPin;
    _clockPin = clockPin;
    _dataPin = dataPin;

    pinMode(_latchPin, OUTPUT);
    pinMode(_dataPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);

    digitalWrite(dataPin, 0);
    digitalWrite(clockPin, 0);
}

void LedController::turnOnColumn(int column)
{
    byte rowData = B00000000;
    byte colData = B00000000;

    for (int i = 1; i < 7; i++)
    {
        if (column == i)
        {
            colData = colData | B1 << (6 - i);
            continue;
        }

        colData = colData | B0 << (6 - i);
    }

    shiftData(rowData, colData);
}

void LedController::turnOnRow(int row)
{
    byte rowData = B00000000;
    byte colData = B00111111;

    if (row <= 8)
    {
        for (int i = 1; i < 9; i++)
        {
            if (row == i)
            {
                rowData = rowData | B0 << (8 - i);
                continue;
            }

            rowData = rowData | B1 << (8 - i);
        }

        colData = B11111111;
    }
    else
    {
        rowData = B11111111;
        if (row == 9)
        {
            colData = colData | B01 << 6;
        }
        else if (row == 10)
        {
            colData = colData | B10 << 6;
        }
    }
    shiftData(rowData, colData);
}

void LedController::turnOnSingle(int row, int column)
{
    byte rowData = B00000000;
    byte colData = B00000000;

    if (row <= 8)
    {
        for (int i = 1; i < 9; i++)
        {
            if (row == i)
            {
                rowData = rowData | B0 << (8 - i);
                continue;
            }

            rowData = rowData | B1 << (8 - i);
        }

        for (int i = 1; i < 7; i++)
        {
            if (column == i)
            {
                colData = colData | B1 << (6 - i);
                continue;
            }

            colData = colData | B0 << (6 - i);
        }

        colData = B11 << 6 | colData;
    }
    else
    {
        rowData = B11111111;

        for (int i = 1; i < 7; i++)
        {
            if (column == i)
            {
                colData = colData | B1 << (6 - i);
                continue;
            }

            colData = colData | B0 << (6 - i);
        }

        if (row == 9)
        {
            colData = colData | B01 << 6;
        }
        else if (row == 10)
        {
            colData = colData | B10 << 6;
        }
    }
    shiftData(rowData, colData);
}

void LedController::shiftData(byte rowData, byte colData)
{
    digitalWrite(_latchPin, LOW);
    shiftOut(_dataPin, _clockPin, MSBFIRST, rowData);
    shiftOut(_dataPin, _clockPin, MSBFIRST, colData);
    digitalWrite(_latchPin, HIGH);
}