/*
 * wifi.h - Library to manage the wifi device
 *
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _WIFI_H
#define _WIFI_H

/*
 * WiFiSpi - https://github.com/JiriBilek/WiFiSpi
 * include library that permit to use the esp8266
 * as a "bridge" between wifi connection and arduino.
 * this facility permit to manage all connections
 * parameters from arduino code (this one) and
 * to instantiate tcp/udp/ssl sockets.
*/
#include <WiFiSpi.h>
#include "config.h"
#include "utility.h"

/* pin that control the esp8266 enable circuit */
#define ESP8266_ENABLE_PIN 16

/* how many milliseconds we need to wait for wifi connection */
#define WIFI_WAIT_CONNECTION 3000

#define WIFI_RETRY 3

class Wifi
{
public:
    Wifi();
    void init();
    bool isReady();
    void readSerial();

private:
    // simplify wifi checks
    bool ready;
    // the Wifi radio's status
    int status;
};

extern Wifi wifi;

#endif // _WIFI_H
