/*
 * main.h - Probably nothing to say ;-)
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _MAIN_H
#define _MAIN_H

// enable or disable
#define DEBUG 1

// enable or disable the network code
#define ENABLE_NETWORK 1

// web server port
#define WEB_SERVER_PORT 80

/* WiFi credentials */
char ssid[] = "ArduinoHeart";
char pass[] = "VtG5PpD8jfufym7fa77apTtjdpF";
/* the Wifi radio's status */
int status = WL_IDLE_STATUS;

/* define the ntp server used */
#define NTP_SERVER "europe.pool.ntp.org"

// web server loop; used by Scheduler
void web_server_loop();

#endif // _MAIN_H