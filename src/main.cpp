/*
 * main.cpp - Probably nothing to say ;-)
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

/* don't forget, we are using the arduino framework */
#include <Arduino.h>

/* Scheduler to manage multiple tasks */
#include <Scheduler.h>

/* general configure header */
#include "config.h"

/* include the capactivice touch sensors */
#include "cts.h"

/* include rtc class */
#include "heart_rtc.h"

/* include wifi management class */
#include "wifi.h"

/* include the ntp client class */
#include "ntpclient.h"

/* include web server class */
#include "webserver.h"

/* include display and touch class */
#include "display.h"

/* include file system */
#include "vfs.h"

/* include some useful functions */
#include "utility.h"

#if ENABLE_NETWORK
/* instantiate ntp client */
WiFiSpiUdp ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER);
#endif

/*
 * INITIALIZE the board
 */
void setup()
{
  // before everything configure the pads buttons;
  // they can be used as function trigger
  cts.init();

  // Initialize serial and wait for port to open:
  Serial.begin(ARDUINO_SERIAL_SPEED);

  // welcome screen on Serial
  serialWelcome();

  // Initialize the RTC chip
  heart_RTC.init();

#if ENABLE_SD
  // initialize vfs/sd card
  vfs.init();
#endif

#if ENABLE_DISPLAY
  // Initialize the display
  display.init();
  // loop on display
  Scheduler.startLoop(display.loop());
#endif

#if ENABLE_NETWORK
  // initialize the wifi with config.h data
  wifi.init();

  // if the wifi is ready we can enable other network services
  if (wifi.isReady())
  {
    // Write the NTP time to the local RTC chip
    // TODO evaluate latency from NTP function to set on RTC
    Serial.print("Start ntp client to server: ");
    Serial.println(NTP_SERVER);
    timeClient.begin();
    heart_RTC.setDateTime(timeClient.getEpochTime());

    // start web server
    webserver.init();
    Scheduler.startLoop(webserver.loop());
  }
#endif // ENABLE_NETWORK

  // loop on capacitive pads
  Scheduler.startLoop(cts.loop());
}

/*
 * Keep alive the board ;-)
 */
void loop()
{
#if ENABLE_NETWORK
  if (wifi.isReady())
  {
    timeClient.update();
    Serial.println(timeClient.getFormattedTime());
  }
#endif

  delay(100);
}
