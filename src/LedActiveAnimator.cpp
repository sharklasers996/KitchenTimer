#include "Arduino.h"
#include "LedActiveAnimator.h"

#define ROW_COUNT 10
#define COLUMN_COUNT 6
#define LED_COUNT 60

LedController _controller;

long _lastAlarmSettingSeconds = 0;
byte _alarmSettingAnimationRow;

void LedActiveAnimator::init(LedController controller)
{
    _controller = controller;
    _alarmSettingAnimationRow = 1;
}

void LedActiveAnimator::animateAlarmSetting()
{
    _controller.turnOnRow(_alarmSettingAnimationRow);
}

void LedActiveAnimator::updateAlarmSetting(long seconds)
{
    long secondsDifference = seconds - _lastAlarmSettingSeconds;
    _lastAlarmSettingSeconds = seconds;

    if (secondsDifference < 0)
    {
        _alarmSettingAnimationRow++;
    }
    else
    {
        _alarmSettingAnimationRow--;
    }

    if (_alarmSettingAnimationRow < 1)
    {
        _alarmSettingAnimationRow = 10;
    }

    if (_alarmSettingAnimationRow > 10)
    {
        _alarmSettingAnimationRow = 1;
    }
}

byte ledCountToTurnOn = 0;

void LedActiveAnimator::animateAlarmElapsed()
{
    if (ledCountToTurnOn == 0)
    {
        return;
    }

    byte turnedOn = 0;

    byte fullRowsToTurnOn = ledCountToTurnOn / 6;
    byte add = 0;
    if (fullRowsToTurnOn > 0)
    {
        add = 1;
    }

    for (int row = 10; row > 10 - fullRowsToTurnOn + add; row--)
    {
        turnedOn += 6;
        _controller.turnOnRow(row);
    }

    if (turnedOn >= ledCountToTurnOn)
    {
        return;
    }

    for (int row = 10 - fullRowsToTurnOn + add; row > 0; row--)
    {
        for (int col = 6; col > 0; col--)
        {
            _controller.turnOnSingle(row, col);

            turnedOn++;
            if (turnedOn >= ledCountToTurnOn)
            {
                return;
            }
        }
    }
}

void LedActiveAnimator::updateAlarmElapsedSeconds(long current, long total)
{
    double ledsPerSecond = 60 / (double)total;
    double secondsPassed = total - current;
    ledCountToTurnOn = 60 - (secondsPassed * ledsPerSecond);
}

long _lastAlarmFinishedAnimationAt = millis();
byte _alarmFinishedAnimationRow = 1;
bool _alarmFinishedDirectionUp = false;

void LedActiveAnimator::animateAlarmFinished()
{
    long durationSinceAnimation = millis() - _lastAlarmFinishedAnimationAt;
    if (durationSinceAnimation > _alarmFinishedAnimationRow * 5)
    {
        if (!_alarmFinishedDirectionUp)
        {
            _alarmFinishedAnimationRow++;
            if (_alarmFinishedAnimationRow > 9)
            {
                _alarmFinishedDirectionUp = true;
            }
        }
        else
        {
            _alarmFinishedAnimationRow--;
            if (_alarmFinishedAnimationRow <= 1)
            {
                _alarmFinishedDirectionUp = false;
            }
        }

        _lastAlarmFinishedAnimationAt = millis();
    }

    _controller.turnOnRow(_alarmFinishedAnimationRow);
}

void LedActiveAnimator::reset()
{
    _alarmSettingAnimationRow = 1;
}