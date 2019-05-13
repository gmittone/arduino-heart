/*
 * display.h - Library to manage the display and the touch screen
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <Scheduler.h>
#include <DS1307.h>
#include <math.h>
#include "config.h"

/*
 * include display and touchscreen libs
 * 
 * both from http://www.rinkydinkelectronics.com
 */
#include <UTFT.h>
#include <URTouch.h>

/*
 * include images converted with
 * http://rinkydinkelectronics.com/t_imageconverter565.php
 */
//#include "display/heart_150x150.h"

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

/* 
 * use as
 * middle_screen(this->x_size, sizeof(line)*lcd.getFontXsize())
 */
#define middle_screen(x, y) (x - y)/2

/* where the clock is centered on the screen */
#define clockCenterX 119
#define clockCenterY 119

class Display
{
public:
    Display();
    void init();
    void welcomeScreen();
    void loop();

    // power on or off the display
    void lcdOn();
    void lcdOff();

    void drawClock();
    void printDate();

    // print seconds from mariage day
    void printMariageSeconds(Time t);

private:
    // screen size
    int x_size, y_size;

    // used in the loop
    int x, y;

    // time structure
    Time t;

    // manage standby/backlight status
    bool lcd_standby;
    bool lcd_backlight;

    /* clock */
    int oldsec;
    void drawMark(int h);
    void drawSec(int s);
    void drawMin(int m);
    void drawHour(int h, int m);
    void clearDate();

    void printMariage();
};

extern Display display;

#endif // _DISPLAY_H
