/*
 * heart_rtc.cpp - implement DS1307 RTC chip present on board
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 * 
 * TODO:
 *   - manage local Timezone; now UTC
 */

#include "heart_rtc.h"

RtcDS1307<TwoWire> Rtc_DS1307(Wire);

Heart_RTC::Heart_RTC()
{
}

void Heart_RTC::init()
{
    // Initialize RTC chip
    Rtc_DS1307.Begin();

    // set date based on compile time
    // we can also use memory to store different value
    // check https://github.com/Makuna/Rtc/blob/master/examples/DS1307_Memory/DS1307_Memory.ino
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    if (!Rtc_DS1307.IsDateTimeValid())
    {
        if (Rtc_DS1307.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc_DS1307.LastError());
        }
        else
        {
            Serial.println("RTC lost confidence in the DateTime!");
            Rtc_DS1307.SetDateTime(compiled);
        }
    }

    if (!Rtc_DS1307.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc_DS1307.SetIsRunning(true);
    }

    RtcDateTime now = Rtc_DS1307.GetDateTime();
    if (now < compiled)
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc_DS1307.SetDateTime(compiled);
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc_DS1307.SetSquareWavePin(DS1307SquareWaveOut_Low);
}

void Heart_RTC::setDateTime(unsigned long unixtimestamp)
{
    // receive an error in input; like, from ntp
    if(unixtimestamp <= 0)
        return;
        
    // use secondsFrom2000 from 1970 (30 years = 946728000 seconds)
    Rtc_DS1307.SetDateTime(unixtimestamp - 946728000UL);
}

String Heart_RTC::getDateTimeString()
{
    char datestring[20];
    RtcDateTime dt = Rtc_DS1307.GetDateTime();

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    return datestring;
}

RtcDateTime Heart_RTC::getDateTime()
{
    return Rtc_DS1307.GetDateTime();
}

unsigned long Heart_RTC::getDateTimeUnixTimeStamp()
{
    RtcDateTime dt;
    dt = Rtc_DS1307.GetDateTime();
    //return(dt.Second + dt.Minute + dt.Hour + dt.Day + dt.Month + dt.Year - 62168470000UL);
    return 0UL;
}

void Heart_RTC::printDateTime()
{
    printDateTime(Rtc_DS1307.GetDateTime());
}

void Heart_RTC::printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}

Heart_RTC heart_RTC;
