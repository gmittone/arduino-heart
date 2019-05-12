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

#define countof(a) (sizeof(a) / sizeof(a[0]))

class Heart_RTCClass
{
public:
    Heart_RTCClass();
    void init();
    void setDateTime(unsigned long unixtimestamp);
    RtcDateTime getDateTime();
    String getDateTimeString();
    unsigned long getDateTimeUnixTimeStamp();
    void printDateTime();
    void printDateTime(const RtcDateTime& dt);
};

extern Heart_RTCClass Heart_RTC;

#endif // _HEART_RTC_H