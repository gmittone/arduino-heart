/*
 * main.h - Probably nothing to say ;-)
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _MAIN_H
#define _MAIN_H

// enable or disable
#define ENABLE_DEBUG 0

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

/* 
 * define marriage time in unix time
 * 06/16/2018 @ 11:00pm (UTC)
 */
#define MARIAGE_UNIXTIMESTAMP = 1529150340

/*
 * Capacitive touch sensor
 * we use the same nomenclature of hardware/heart_chema.ps
 * and the pinout is relative to Arduino DUE
 * TRIG_G for Giovanni sign = ADCH_7 = AD6
 * TRIG_B for Beatrice sign = ADCL_7 = CANRX0
 */
#define ctsGiovanni 68
#define ctsBeatrice 60

/* Arduino DUO serial output speed */
#define ARDUINO_SERIAL_SPEED 9600

// web server loop; used by Scheduler
void web_server_loop();

// capacitive touch sensors loop
void cts_loop();

#endif // _MAIN_H