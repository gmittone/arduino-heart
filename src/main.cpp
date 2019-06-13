/*
 * main.cpp - Probably nothing to say ;-)
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

/* general configure header */
#include "config.h"

/* don't forget, we are using the arduino framework */
#include <Arduino.h>

/* Scheduler to manage multiple tasks */
#include <Scheduler.h>

/* include the capactivice touch sensors */
#include "cts.h"

/* include time management class */
#include "localTime.h"

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

/* "thread" functions */
void cts_loop();
void display_loop();
void webserver_loop();
void esp8266serial_loop();
void localtime_loop();

/*
 * INITIALIZE the board
 */
void setup()
{
#if ENABLE_DISPLAY
  // Initialize the display
  display.init();
#endif

  // Initialize serial and wait for port to open:
  Serial.begin(ARDUINO_SERIAL_SPEED);

#if ENABLE_NETWORK && ENABLE_DEBUG
  Serial.println("Start listening on Serial1 for ESP8266 debug messages");
  // start listening on serial of ESP8266 for debug messages
  Scheduler.startLoop(esp8266serial_loop);
#endif

  // before everything configure the pads buttons;
  // they can be used as function trigger
  cts.init();

  // start clock
  localTime.init();

  // welcome screen on Serial
  serialWelcome();

#if ENABLE_SD
  // initialize vfs/sd card
  vfs.init();
#endif

#if ENABLE_DISPLAY
  // Welcome message
  display.drawWelcome();
#endif

#if ENABLE_NETWORK
  // initialize the wifi with config.h data
  wifi.init();

  // if the wifi is ready we can enable other network services
  if (wifi.isReady())
  {
    localTime.enableNTP();

#if ENABLE_WEB_SERVER
    // start web server
    webserver.init();
    Scheduler.startLoop(webserver_loop);
#endif
  }
#endif // ENABLE_NETWORK

  // start time loop
  Scheduler.startLoop(localtime_loop);

#if ENABLE_DISPLAY
  display.drawClock();
#endif

  // start the loop on pads
  Scheduler.startLoop(cts_loop);

#if ENABLE_DISPLAY
  // loop on display
  Scheduler.startLoop(display_loop);
#endif
}

/*
 * Keep alive the board ;-)
 */
void loop()
{
  delay(1000);
  yield();
}

/* "thread" loops */
void cts_loop()
{
  cts.loop();
  yield();
}
void display_loop()
{
  display.loop();
  yield();
}
void webserver_loop()
{
  webserver.loop();
  yield();
}

void esp8266serial_loop()
{
  wifi.readSerial();
  yield();
}

void localtime_loop()
{
  localTime.loop();
  yield();
}