/*
 * localTime.cpp - Library to manage time
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#include <Arduino.h>
#include "localTime.h"

// instantiate the RTC hardware
DS1307 rtc(SDA, SCL);

#if ENABLE_NETWORK
WiFiSpiUdp ntpUDP;
NTPClient ntpClient(ntpUDP, NTP_SERVER);
#endif

LocalTime::LocalTime()
{
}

void LocalTime::init()
{
    // start the rtc chip
    rtc.begin();

    // check if rtc has good time
    // initial startup 01/01/00 01 00:00:00
    // (MM/DD/YY DOW HH:MM:SS)
    Time t = rtc.getTime();
    if(t.mon == 01 && t.date == 01 && t.year == 2000)
    {
        this->setRtcFromCopiledDate();
    }
  
    // get the time from rtc
    this->local = rtc.getUnixTime(rtc.getTime());
    breakTime(this->local, this->local_te);

    Serial.println("Local time is: " + this->getFormattedTime());
}

void LocalTime::loop()
{
    timeStatus();
    this->local = rtc.getUnixTime(rtc.getTime());
    breakTime(this->local, this->local_te);
#if ENABLE_TIME_DEBUG
    Serial.println("Local unixtimestamp: " + String(this->local) + " Local time: " + this->getFormattedTime());
#endif
    delay(1000);
}

/*
 * force the NTP query and return unixtime stamp
 * used by setSyncProvider() of Time.h
 * 
 * @return time in seconds since Jan. 1, 1970
 */
time_t runSyncTime()
{
#if ENABLE_NETWORK    
    if (ntpClient.update())
    {
        time_t t = ntpClient.getEpochTime();
        TimeElements te;
        breakTime(t, te);
        rtc.setDate(te.Day, te.Month, te.Year + 1970);
        rtc.setTime(te.Hour, te.Minute, te.Second); 
        return t;
    }
    else
    {
        return 0;
    }
#endif    
}

void LocalTime::enableNTP()
{
#if ENABLE_NETWORK    
    // synchronize with ntp every NTP_UPDATE_INTERVAL
    // Write the NTP time to the local RTC chip
    // TODO evaluate latency from NTP function to set on RTC
    Serial.print("Start ntp client to server: ");
    Serial.println(NTP_SERVER);
    ntpClient.begin();
    ntpClient.setUpdateInterval(NTP_UPDATE_INTERVAL);

    setSyncProvider(runSyncTime);

    uint8_t status = timeStatus();
    if (status == timeSet)
    {
        String status_string = (status == timeNotSet ? "timeNotSet" : (status == timeSet ? "timeSet" : "timeNeedsSync" ));
#if ENABLE_TIME_DEBUG
        Serial.println("Time sync status: " + status_string);
        Serial.println("NTP time is: " + ntpClient.getFormattedTime());
#endif        
    }
#endif // ENABLE_NETWORK    
#if ENABLE_TIME_DEBUG    
    Serial.println("Now local time is: " + this->getFormattedTime());
#endif
}

void LocalTime::setTime(time_t t)
{
    this->local = t;
    breakTime(this->local, this->local_te);

    // TODO may move to a RTC DS1307 library that support time_t
    rtc.setTime(this->local_te.Hour, this->local_te.Minute, this->local_te.Second);
    rtc.setDate(this->local_te.Day, this->local_te.Month, this->local_te.Year);
}

TimeElements LocalTime::getTime()
{
    return this->local_te;
}

time_t LocalTime::getUnixTimeStamp()
{
    return this->local;
}

String LocalTime::getFormattedTime()
{
    return String(this->local_te.Year + 1970) + "/" + String(this->local_te.Month) + "/" +
           String(this->local_te.Day) + " " + String(this->local_te.Hour) + ":" +
           String(this->local_te.Minute) + ":" + String(this->local_te.Second);
}

String LocalTime::getFormattedTime(TimeElements te)
{
    return String(te.Year + 1970) + "/" + String(te.Month) + "/" +
           String(te.Day) + " " + String(te.Hour) + ":" +
           String(te.Minute) + ":" + String(te.Second);
}

String LocalTime::getDOWStr(uint8_t format)
{
    char *output = "xxxxxxxxxx";
    char *daysLong[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    char *daysShort[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    if (format == FORMAT_SHORT)
        output = daysShort[this->local_te.Wday - 1];
    else
        output = daysLong[this->local_te.Wday - 1];
    return output;
}

String LocalTime::getMonthStr(uint8_t format)
{
    char *output = "xxxxxxxxx";
    char *monthLong[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    char *monthShort[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    if (format == FORMAT_SHORT)
        output = monthShort[this->local_te.Month - 1];
    else
        output = monthLong[this->local_te.Month - 1];
    return output;
}

uint8_t LocalTime::StringToUint8(const char *pString)
{
    uint8_t value = 0;

    // skip leading 0 and spaces
    while ('0' == *pString || *pString == ' ')
    {
        pString++;
    }

    // calculate number until we hit non-numeral char
    while ('0' <= *pString && *pString <= '9')
    {
        value *= 10;
        value += *pString - '0';
        pString++;
    }
    return value;
}

// use __DATE__, __TIME__ to set the first date
void LocalTime::setRtcFromCopiledDate()
{
    uint16_t year;
    uint8_t month, day, hour, minute, second;

    // sample input: date = "Dec 06 2009", time = "12:34:56"
    year = this->StringToUint8(__DATE__ + 9) + 2000;
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (__DATE__[0])
    {
    case 'J':
        if (__DATE__[1] == 'a')
            month = 1;
        else if (__DATE__[2] == 'n')
            month = 6;
        else
            month = 7;
        break;
    case 'F':
        month = 2;
        break;
    case 'A':
        month = __DATE__[1] == 'p' ? 4 : 8;
        break;
    case 'M':
        month = __DATE__[2] == 'r' ? 3 : 5;
        break;
    case 'S':
        month = 9;
        break;
    case 'O':
        month = 10;
        break;
    case 'N':
        month = 11;
        break;
    case 'D':
        month = 12;
        break;
    }
    day = StringToUint8(__DATE__ + 4);
    hour = StringToUint8(__TIME__);
    minute = StringToUint8(__TIME__ + 3);
    second = StringToUint8(__TIME__ + 6);

    rtc.setDate(day, month, year);
    rtc.setTime(hour, minute, second);
}

/* instantiate the main object */
LocalTime localTime;