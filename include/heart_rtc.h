/*
 * heart_rtc.h - library DS1307 RTC chip present on board
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _HEART_RTC_H
#define _HEART_RTC_H

#include <Wire.h>
#include <RtcDS1307.h>
#include <time.h>

/*
 * RTC DS1307 programmable alarm
 * digital pin; active HI
 */
#define RTC_ALARM_PIN 17

#define countof(a) (sizeof(a) / sizeof(a[0]))

class Heart_RTC
{
public:
    Heart_RTC();
    void init();
    void setDateTime(unsigned long unixtimestamp);
    RtcDateTime getDateTime();
    String getDateTimeString();
    unsigned long getDateTimeUnixTimeStamp();
    void printDateTime();
    void printDateTime(const RtcDateTime& dt);
};

extern Heart_RTC heart_RTC;

#endif // _HEART_RTC_H