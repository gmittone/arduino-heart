/*
 * main.cpp - Probably nothing to say ;-)
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 */

/* don't forget, we are using the arduino framework */
#include <Arduino.h>

/*
 * WiFiSpi - https://github.com/JiriBilek/WiFiSpi
 * include library that permit to use the esp8266
 * as a "bridge" between wifi connection and arduino.
 * this facility permit to manage all connections
 * parameters from arduino code (this one) and
 * to instantiate tcp/udp/ssl sockets.
*/
#include "WiFiSpi.h"

/* include some useful functions */
#include "utility.h"

/* include the ntp client class */
#include "ntpclient.h"

/* WiFi credentials */
char ssid[] = "ArduinoHeart";
char pass[] = "VtG5PpD8jfufym7fa77apTtjdpF";

/* the Wifi radio's status */
int status = WL_IDLE_STATUS;

/*
 * INITIALIZE the board
 */
void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Initialize the WifiSpi library
  WiFiSpi.init();

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
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFiSpi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

  /* get the time from ntp server */
  NTPClient();
}

/*
 * Keep alive the board ;-)
 */
void loop()
{
}