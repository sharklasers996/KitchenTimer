#include <stdio.h>
#include "Timer.h"
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#include "./src/RotaryEncoder.h"
#include "./src/RtcModule.h"
#include "./src/PushButton.h"
#include "./src/ClockModule.h"
void setup() {}
void loop() {}
// #define ROTARY_PIN1 0
// #define ROTARY_PIN2 A3
// #define TIME_BUTTON A1
// #define POWER_BUTTON A0
// #define ACTION_BUTTON A2

// RotaryEncoder rotaryEncoder(ROTARY_PIN1, ROTARY_PIN2);

// PushButton timeButton(TIME_BUTTON);
// PushButton powerButton(POWER_BUTTON);
// PushButton actionButton(ACTION_BUTTON);
// //--------------------------

// #define SETTING_TIME 0
// #define SHOWING_TIME 3
// #define SETTING_ALARM 4
// #define RUNNING_ALARM 5
// #define FINISHED_ALARM 6

// #define TIMESETTING_HOUR 0
// #define TIMESETTING_MINUTE 1
// #define TIMESETTING_NONE 2

// uint8_t state = SHOWING_TIME;
// uint8_t timeSetting = TIMESETTING_NONE;

// Adafruit_7segment digitDisplay = Adafruit_7segment();

// // CE (RST), IO (DATA), CLK (5, 6, 7);
// RtcModule RTC = RtcModule();

// int hours = 0;
// int minutes = 0;
// uint8_t seconds = 0;
// int totalSeconds = 0;

// uint8_t hoursDigit1 = 0;
// uint8_t hoursDigit2 = 0;
// uint8_t minutesDigit1 = 0;
// uint8_t minutesDigit2 = 0;
// uint8_t secondsDigit1 = 0;
// uint8_t secondsDigit2 = 0;

// bool blink = false;
// int lastBlinkAt = millis();

// uint8_t alarmHours = 0;
// int alarmMinutes = 0;
// uint8_t alarmSeconds = 0;
// long totalAlarmSeconds = 0;

// Timer testingTimer;
// Timer timer;

// void setup()
// {
//     Serial.begin(9600);

//     digitDisplay.begin(0x70);

//     RTC.init(5, 6, 7);
//     RTC.writeProtect(false);
//     RTC.halt(false);

//     hours = 4;
//     minutes = 20;
//     saveTime();

//     testingTimer.every(1, testingTimerMethod);
//     timer.every(1, update);
// }

// void loop()
// {
//     testingTimer.update();
//     timer.update();
// }

// void testingTimerMethod()
// {
//     if (timeButton.isPushed())
//     {
//         rotateTimeSetting();
//     }

//     if (powerButton.isPushed())
//     {
//         Serial.println("power!");
//     }

//     if (actionButton.isPushed())
//     {
//         if (state == SETTING_ALARM)
//         {
//             startAlarm();
//         }
//         else if (state == FINISHED_ALARM)
//         {
//             state = SHOWING_TIME;
//         }
//     }

//     uint8_t value = rotaryEncoder.read();

//     if (value != 0 && state == SHOWING_TIME)
//     {
//         state = SETTING_ALARM;
//         alarmHours = 0;
//         alarmMinutes = 0;
//         alarmSeconds = 0;
//         totalAlarmSeconds = 0;
//     }

//     if (value == 255)
//     {
//         increaseTime();
//     }
//     else if (value == 1)
//     {
//         decreaseTime();
//     }
// }

// void update()
// {
//     if (state == SHOWING_TIME)
//     {
//         printTime();
//     }
//     else if (state == SETTING_TIME)
//     {
//         printSettingTime();
//     }
//     else if (state == SETTING_ALARM)
//     {
//         printAlarmSettingTime();
//     }
//     else if (state == RUNNING_ALARM)
//     {
//         printAlarmTime();
//     }
//     else if (state == FINISHED_ALARM)
//     {
//         printBlinkingTime();
//     }
// }

// void rotateTimeSetting()
// {
//     if (state != SETTING_TIME)
//     {
//         timeSetting = TIMESETTING_HOUR;
//         state = SETTING_TIME;
//         startBlink();
//         return;
//     }

//     switch (timeSetting)
//     {
//     case TIMESETTING_HOUR:
//         timeSetting = TIMESETTING_MINUTE;
//         startBlink();
//         break;
//     case TIMESETTING_MINUTE:
//         state = SHOWING_TIME;
//         startBlink();
//         saveTime();
//         break;
//     }
// }

// void increaseTime()
// {
//     if (state == SETTING_TIME)
//     {
//         if (timeSetting == TIMESETTING_HOUR)
//         {
//             hours++;
//             if (hours >= 24)
//             {
//                 hours = 0;
//             }
//         }
//         else if (timeSetting == TIMESETTING_MINUTE)
//         {
//             minutes++;
//             if (minutes >= 60)
//             {
//                 minutes = 0;
//             }
//         }

//         getDigits(hours, hoursDigit1, hoursDigit2);
//         getDigits(minutes, minutesDigit1, minutesDigit2);
//     }
//     else if (state == SETTING_ALARM)
//     {
//         alarmMinutes++;
//         if (alarmMinutes >= 60)
//         {
//             alarmHours++;
//             if (alarmHours >= 23)
//             {
//                 alarmHours = 23;
//             }
//             alarmMinutes = 0;
//         }

//         totalAlarmSeconds = alarmHours * 60 * 60;
//         totalAlarmSeconds += alarmMinutes * 60;
//         Serial.print("total ");
//         Serial.println(totalAlarmSeconds);
//     }
// }

// void decreaseTime()
// {
//     if (state == SETTING_TIME)
//     {
//         if (timeSetting == TIMESETTING_HOUR)
//         {
//             hours--;
//             if (hours < 0)
//             {
//                 hours = 23;
//             }
//         }
//         else if (timeSetting == TIMESETTING_MINUTE)
//         {
//             minutes--;
//             if (minutes < 0)
//             {
//                 minutes = 59;
//             }
//         }

//         getDigits(hours, hoursDigit1, hoursDigit2);
//         getDigits(minutes, minutesDigit1, minutesDigit2);
//     }
//     else if (state == SETTING_ALARM)
//     {
//         alarmMinutes--;
//         if (alarmMinutes < 0)
//         {
//             alarmHours--;
//             if (alarmHours <= 0)
//             {
//                 alarmHours = 23;
//             }
//             alarmMinutes = 59;
//         }

//         totalAlarmSeconds = alarmHours * 60 * 60;
//         totalAlarmSeconds += alarmMinutes * 60;

//         Serial.print("total ");
//         Serial.println(totalAlarmSeconds);
//     }
// }

// void printTime()
// {
//     if (blink)
//     {
//         blinkAll();
//     }
//     else
//     {
//         updateTimeVariables();

//         digitDisplay.writeDigitNum(0, hoursDigit1, false);
//         digitDisplay.writeDigitNum(1, hoursDigit2, false);

//         bool drawColon = true;
//         if (seconds % 2 == 0)
//         {
//             drawColon = false;
//         }

//         digitDisplay.drawColon(drawColon);
//         digitDisplay.writeDigitNum(3, minutesDigit1, false);
//         digitDisplay.writeDigitNum(4, minutesDigit2, false);
//         digitDisplay.writeDisplay();
//     }
// }

// void printSettingTime()
// {
//     if (blink)
//     {
//         if (timeSetting == TIMESETTING_HOUR)
//         {
//             blinkHours();
//         }
//         else if (timeSetting == TIMESETTING_MINUTE)
//         {
//             blinkMinutes();
//         }
//     }
//     else
//     {
//         digitDisplay.writeDigitNum(0, hoursDigit1, false);
//         digitDisplay.writeDigitNum(1, hoursDigit2, false);
//         digitDisplay.drawColon(true);
//         digitDisplay.writeDigitNum(3, minutesDigit1, false);
//         digitDisplay.writeDigitNum(4, minutesDigit2, false);
//         digitDisplay.writeDisplay();
//     }
// }

// void printAlarmSettingTime()
// {
//     updateAlarmVariables();

//     digitDisplay.writeDigitNum(0, hoursDigit1, false);
//     digitDisplay.writeDigitNum(1, hoursDigit2, false);
//     digitDisplay.writeDigitNum(3, minutesDigit1, false);
//     digitDisplay.writeDigitNum(4, minutesDigit2, false);

//     digitDisplay.drawColon(true);
//     digitDisplay.writeDisplay();
// }

// void printAlarmTime()
// {
//     updateAlarm();
//     updateAlarmVariables();

//     if (alarmHours == 0)
//     {
//         digitDisplay.writeDigitNum(0, minutesDigit1, false);
//         digitDisplay.writeDigitNum(1, minutesDigit2, false);
//         digitDisplay.writeDigitNum(3, secondsDigit1, false);
//         digitDisplay.writeDigitNum(4, secondsDigit2, false);
//     }
//     else
//     {
//         digitDisplay.writeDigitNum(0, hoursDigit1, false);
//         digitDisplay.writeDigitNum(1, hoursDigit2, false);
//         digitDisplay.writeDigitNum(3, minutesDigit1, false);
//         digitDisplay.writeDigitNum(4, minutesDigit2, false);
//     }

//     bool drawColon = true;
//     if (alarmSeconds % 2 == 0)
//     {
//         drawColon = false;
//     }
//     digitDisplay.drawColon(drawColon);
//     digitDisplay.writeDisplay();
// }

// void printBlinkingTime()
// {
//     int blinkTime = millis() - lastBlinkAt;
//     if (blinkTime < 500)
//     {
//         return;
//     }

//     if (blink)
//     {
//         digitDisplay.clear();
//         blink = false;
//     }
//     else
//     {
//         digitDisplay.drawColon(true);
//         digitDisplay.writeDigitNum(0, 0, false);
//         digitDisplay.writeDigitNum(1, 0, false);
//         digitDisplay.writeDigitNum(3, 0, false);
//         digitDisplay.writeDigitNum(4, 0, false);
//         blink = true;
//     }

//     digitDisplay.writeDisplay();

//     lastBlinkAt = millis();
// }

// void startBlink()
// {
//     lastBlinkAt = millis();
//     blink = true;
// }

// void blinkHours()
// {
//     int blinkTime = millis() - lastBlinkAt;
//     if (blinkTime > 500)
//     {
//         blink = false;
//     }

//     digitDisplay.clear();
//     digitDisplay.drawColon(true);
//     digitDisplay.writeDigitNum(3, minutesDigit1, false);
//     digitDisplay.writeDigitNum(4, minutesDigit2, false);
//     digitDisplay.writeDisplay();
// }

// void blinkMinutes()
// {
//     int blinkTime = millis() - lastBlinkAt;
//     if (blinkTime > 500)
//     {
//         blink = false;
//     }

//     digitDisplay.clear();
//     digitDisplay.drawColon(true);
//     digitDisplay.writeDigitNum(0, hoursDigit1, false);
//     digitDisplay.writeDigitNum(1, hoursDigit2, false);
//     digitDisplay.writeDisplay();
// }

// void blinkAll()
// {
//     int blinkTime = millis() - lastBlinkAt;
//     if (blinkTime > 500)
//     {
//         blink = false;
//     }

//     digitDisplay.clear();
//     digitDisplay.writeDisplay();
// }

// int lastAlarmUpdateSeconds = 0;
// bool firstAlarm = false;
// void startAlarm()
// {
//     lastAlarmUpdateSeconds = totalSeconds;
//     state = RUNNING_ALARM;

//     firstAlarm = true;
//     int secondsDifference = totalSeconds - lastAlarmUpdateSeconds;

//     Serial.print("diff1 ");
//     Serial.println(secondsDifference);

//     Serial.print("total ");
//     Serial.println(totalAlarmSeconds);
// }

// void updateAlarm()
// {
//     updateTimeVariables();

//     if (firstAlarm)
//     {
//         lastAlarmUpdateSeconds = totalSeconds;
//         firstAlarm = false;
//     }

//     int secondsDifference = totalSeconds - lastAlarmUpdateSeconds;
//     if (secondsDifference == 0)
//     {
//         return;
//     }

//     Serial.print("diff ");
//     Serial.println(secondsDifference);

//     totalAlarmSeconds -= secondsDifference;
//     Serial.println(totalAlarmSeconds);

//     alarmHours = (totalAlarmSeconds / 60) / 60;
//     alarmMinutes = totalAlarmSeconds / 60;
//     alarmSeconds = totalAlarmSeconds % 60;

//     Serial.print(alarmHours);
//     Serial.print(":");
//     Serial.print(alarmMinutes);
//     Serial.print(":");
//     Serial.println(alarmSeconds);
//     lastAlarmUpdateSeconds = totalSeconds;

//     if (totalAlarmSeconds <= 0)
//     {
//         lastBlinkAt = millis() + 1000;
//         blink = true;
//         state = FINISHED_ALARM;
//     }
// }

// void saveTime()
// {
//     // Sunday, September 25, 2018 at 13:30:50.
//     RTCTime t(2018, 9, 25, hours, minutes, 00, RTCTime::kTuesday);
//     RTC.time(t);
// }

// void updateTimeVariables()
// {
//     RTCTime t = RTC.time();

//     hours = t.hr;
//     minutes = t.min;
//     seconds = t.sec;

//     totalSeconds = hours * 60;
//     totalSeconds += minutes * 60;
//     totalSeconds += seconds;

//     getDigits(t.hr, hoursDigit1, hoursDigit2);
//     getDigits(t.min, minutesDigit1, minutesDigit2);
//     getDigits(t.sec, secondsDigit1, secondsDigit2);
// }

// void updateAlarmVariables()
// {
//     getDigits(alarmHours, hoursDigit1, hoursDigit2);
//     getDigits(alarmMinutes, minutesDigit1, minutesDigit2);
//     getDigits(alarmSeconds, secondsDigit1, secondsDigit2);
// }

// void getDigits(uint8_t number, uint8_t &digit1, uint8_t &digit2)
// {
//     digit2 = number % 10;

//     if (number < 10)
//     {
//         digit1 = 0;
//     }
//     else
//     {
//         digit1 = number;
//         if (digit1 >= 100000000)
//             digit1 /= 100000000;
//         if (digit1 >= 10000)
//             digit1 /= 10000;
//         if (digit1 >= 100)
//             digit1 /= 100;
//         if (digit1 >= 10)
//             digit1 /= 10;
//     }
// }
