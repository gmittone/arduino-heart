/*
 * main.cpp - Probably nothing to say ;-)
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 */

/* don't forget, we are using the arduino framework */
#include <Arduino.h>

/* Scheduler to manage multiple tasks */
#include <Scheduler.h>

/*
 * WiFiSpi - https://github.com/JiriBilek/WiFiSpi
 * include library that permit to use the esp8266
 * as a "bridge" between wifi connection and arduino.
 * this facility permit to manage all connections
 * parameters from arduino code (this one) and
 * to instantiate tcp/udp/ssl sockets.
*/
#include <WiFiSpi.h>

/* include the capactivice touch sensors */
#include "cts.h"

/* include rtc class */
#include "heart_rtc.h"

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

/* general configure header */
#include "config.h"

#if ENABLE_NETWORK
/* the Wifi radio's status */
int status = WL_IDLE_STATUS;

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
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // welcome screen on Serial
  serialWelcome();

  // Initialize the RTC chip
  heart_RTC.init();

#if ENABLE_SD
  // initialize vfs/sd card
  vfs.init();
#endif

  // Initialize the display
  display.init();

#if ENABLE_NETWORK
  // Initialize the WifiSpi library
  WiFiSpi.init();

  // WiFiSpi versions
  Serial.print("WiFiSpi ESP8266 firmware version: ");
  Serial.println(WiFiSpi.firmwareVersion());
  Serial.print("WiFiSpi ESP8266 protocol version: ");
  Serial.println(WiFiSpi.protocolVersion());
  Serial.print("WiFiSpi master protocol version: ");
  Serial.println(WiFiSpi.masterProtocolVersion());

  // check for the presence of the ESP module:
  if (WiFiSpi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi module not present");
    // don't continue:
    while (true)
      ;
  }

  if (!WiFiSpi.checkProtocolVersion())
  {
    Serial.println("Protocol version mismatch. Please upgrade the firmware");
    // don't continue:
    while (true)
      ;
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network:
    status = WiFiSpi.begin(WIFI_SSID, WIFI_PASS);

    // wait 5 seconds for connection:
    delay(5000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
  Serial.println("");

  // Write the NTP time to the local RTC chip
  // TODO evaluate latency from NTP function to set on RTC
  Serial.print("Start ntp client to host: ");
  Serial.println(NTP_SERVER);
  timeClient.begin();
  heart_RTC.setDateTime(timeClient.getEpochTime());

  // start web server
  webserver.init();
  Scheduler.startLoop(webserver.loop());
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
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
#endif

  delay(100);
}
