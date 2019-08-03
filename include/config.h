/*
 * config.h - Probably nothing to say ;-)
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _CONFIG_H
#define _CONFIG_H

/* WiFi credentials */
#define WIFI_SSID "nuje_home"
#define WIFI_PASS "homepassword"

/* enable or disable debug messages
 * TODO:
 *   - move to macro INFO()/WARN()/ERR()
 */
#define ENABLE_DEBUG 0
#if ENABLE_DEBUG
#define DEBUG
#endif

/* enable ntp debug */
#define ENABLE_NTP_DEBUG 0

/* overwrite WiFiSpi configuration
 *
 * TODO:
 * seems it doesn't work rewriting _CONFIG_H_INCLUDED
 * of lib/WiFiSpi/src/utility/debug.h and
 * other definitions
 */
#define ENABLE_WIFI_DEBUG 0

/* enable or disable localTime debug messages
 */
#define ENABLE_TIME_DEBUG 0

/* enable or disable CTS debug messages
 */
#define ENABLE_CTS_DEBUG 0

/* enable or disable display debug messages
 */
#define ENABLE_DISPLAY_DEBUG 0

/* enable or disable the vfs debug messages */
#define ENABLE_VFS_DEBUG 0

/* enable or disable the network code */
#define ENABLE_NETWORK 1

/* enable SD card support */
#define ENABLE_SD 1

/* enable display */
#define ENABLE_DISPLAY 1

/* enable web server
 * TODO: manage multiple socket
 *       see https://github.com/JiriBilek/WiFiSpi/issues/14
 */
#define ENABLE_WEB_SERVER 0

/* 
 * define marriage time in unix timestamp
 * 06/16/2018 @ 11:00pm (UTC)
 */
#define MARIAGE_UNIXTIMESTAMP 1529146800

/* Arduino DUO serial output speed */
#define ARDUINO_SERIAL_SPEED 115200

/* ESP8266 serial port debuging */
#define ESP8266_SERIAL_SPEED 115200

/* define the ntp server used */
#define NTP_SERVER "pool.ntp.org"

/* define local time zone */
#define TIMEZONE "GMT"

/*
 * update interval of ntp
 * every 1h in ms
 */
#define NTP_UPDATE_INTERVAL 3600000

/*
 * folder on the sd that contains photos
 * on JPEG format (*.jpg) at size 480x320 pixels
 */
#define SD_PHOTOS_FOLDER "/PHOTOS"

/*
 * how many ms to wait on welcome screen
 * 
 * NOTE: with networking we could problably avoid it
 */
#define WELCOME_SCREEN_WAIT 2000

#endif
