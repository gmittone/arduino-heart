/*
 * config.h - Probably nothing to say ;-)
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _CONFIG_H
#define _CONFIG_H

/* WiFi credentials */
#define WIFI_SSID "ArduinoHeart"
#define WIFI_PASS "VtG5PpD8jfufym7fa77apTtjdpF"

/* enable or disable debug messages
 * TODO:
 *   - move to macro INFO()/WARN()/ERR()
 */
#define ENABLE_DEBUG 0

/* overwrite WiFiSpi configuration
 *
 * TODO:
 * seems doesn't work rewriting _CONFIG_H_INCLUDED
 * of lib/WiFiSpi/src/utility/debug.h and
 * other definitions
 */
#define ENABLE_WIFI_DEBUG 0

/* enable or disable the network code */
#define ENABLE_NETWORK 1

/* enable SD card support */
#define ENABLE_SD 1

/* enable display */
#define ENABLE_DISPLAY 1

/* 
 * define marriage time in unix time
 * 06/16/2018 @ 11:00pm (UTC)
 */
#define MARIAGE_UNIXTIMESTAMP 1529146800

/* Arduino DUO serial output speed */
#define ARDUINO_SERIAL_SPEED 115200

/* define the ntp server used */
#define NTP_SERVER "europe.pool.ntp.org"

#endif
