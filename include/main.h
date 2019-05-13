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

// web server loop; used by Scheduler
void web_server_loop();

#endif // _MAIN_H