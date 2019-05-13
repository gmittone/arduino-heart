/*
 * main.h - Probably nothing to say ;-)
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _MAIN_H
#define _MAIN_H

// web server port
#define WEB_SERVER_PORT 80

/* the Wifi radio's status */
int status = WL_IDLE_STATUS;

/*
 * Capacitive touch sensor
 * we use the same nomenclature of hardware/heart_chema.ps
 * and the pinout is relative to Arduino DUE
 * TRIG_G for Giovanni sign = ADCH_7 = AD6
 * TRIG_B for Beatrice sign = ADCL_7 = CANRX0
 */
#define ctsGiovanni 68
#define ctsBeatrice 60

// web server loop; used by Scheduler
void web_server_loop();

// capacitive touch sensors loop
void cts_loop();

#endif // _MAIN_H