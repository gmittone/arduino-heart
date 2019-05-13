/*
 * webserver.h - Library to manage the web server
 *
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _WEBSERVER_H
#define _WEBSERVER_H

#include <Scheduler.h>
#include <WiFiSpi.h>
#include "config.h"

// web server default port
#define WEB_SERVER_PORT 80

class WebServer
{
public:
    WebServer();
    void init();
    SchedulerTask loop();

private:

};

extern WebServer webserver;

#endif // _WEBSERVER_H