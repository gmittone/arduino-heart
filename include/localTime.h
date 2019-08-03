/*
 * localTime.h - Library to manage time
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */


#ifndef _LOCAL_TIME_H
#define _LOCAL_TIME_H

#include <Arduino.h>

/* manage time with Arduino library */
#include <Time.h>

/* load the alarm library */
#include <TimeAlarms.h>

/* include rtc hardware device class */
#include <DS1307.h>

/* include timezone facility */
#include <Timezone.h>

/* include the udp socket library */
#include "ntpclient.h"

/* include the display library */
#include "display.h"

/* include the ntp local library */
#include "config.h"

// string format; used on getDOWStr and getMonthStr
#define FORMAT_SHORT	1
#define FORMAT_LONG		2

class LocalTime
{
public:
    LocalTime();
    // initialize with compile time and set rtc
    void init();
    void loop();
    // enable NTP sync
    void enableNTP();
    void setTime(time_t t);
    TimeElements getTime();
    time_t getUnixTimeStamp();
    String getFormattedTime();
    String getFormattedTime(TimeElements te);

    String getDOWStr(uint8_t format);
    String getMonthStr(uint8_t format);

private:
    time_t local, utc;
    TimeElements local_te, utc_te;
    //pointer to the time change rule, use to get the TZ abbrev
    TimeChangeRule *tcr;

    // use __DATE__, __TIME__ to set the first date
    uint8_t StringToUint8(const char* pString);
    void setRtcFromCopiledDate();
};

extern LocalTime localTime;
#endif // _LOCAL_TIME_H