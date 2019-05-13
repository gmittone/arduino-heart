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

/* include rtc class */
#include "heart_rtc.h"

/* include the ntp client class */
#include "ntpclient.h"

/* include display and touch class */
#include "display.h"

/* include some useful functions */
#include "utility.h"

/* main header */
#include "main.h"

/* general configure header */
#include "config.h"

#if ENABLE_NETWORK
/* instantiate web server on port 80 */
WiFiSpiServer web_server(WEB_SERVER_PORT);

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
  pinMode(ctsGiovanni, INPUT);
  pinMode(ctsBeatrice, INPUT);

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
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFiSpi.begin(ssid, pass);

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
  Serial.print("Start Webserver on host: ");
  Serial.print(WiFiSpi.localIP());
  Serial.print(" and port: ");
  Serial.println(WEB_SERVER_PORT);
  Serial.println("");
  web_server.begin();
  Scheduler.startLoop(web_server_loop);
#endif // ENABLE_NETWORK

// loop on capacitive pads
Scheduler.startLoop(cts_loop);
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

void cts_loop()
{
  if(digitalRead(ctsGiovanni))
  {
    #if ENABLE_DEBUG
    Serial.println("Giovanni TOUCHED");
    #endif
  }
  if(digitalRead(ctsBeatrice))
  {
    #if ENABLE_DEBUG
    Serial.println("Beatrice TOUCHED");
    #endif
  }

  // 100ms seems good
  delay(100);
  yield();
}


#if ENABLE_NETWORK
void web_server_loop()
{
  // listen for incoming clients
  WiFiSpiClient client = web_server.available();
  if (client) {
    #if ENABLE_DEBUG
    Serial.println("new client");
    #endif
    // an http request ends with a blank line
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        #if ENABLE_DEBUG
        Serial.write(c);
        #endif
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    #if ENABLE_DEBUG
    Serial.println("client disonnected");
    #endif
  }
  yield();
}
#endif // ENABLE_NETWORK