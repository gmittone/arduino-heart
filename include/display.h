/*
 * display.h - Library to manage the display and the touch screen
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <Scheduler.h>
#include "config.h"

/*
 * include display and touchscreen libs
 * 
 * both from http://www.rinkydinkelectronics.com
 */
#include <UTFT.h>
#include <URTouch.h>

/*
 * output low ==> lcd on
 * output high ==> light off
 */
#define LCD_STANDBY_PIN 46
/*
 * output low ==> light on
 * output high ==> light off
 */
#define LCD_BACKLIGHT_PIN 43

class Display
{
public:
    Display();
    void init();
    SchedulerTask loop();

private:
    /* manage standby/backlight status */
    bool lcd_standby;
    bool lcd_backlight;

};

extern Display display;

#endif // _DISPLAY_H