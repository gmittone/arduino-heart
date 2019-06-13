/*
 * display.h - Library to manage the display and the touch screen
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <Scheduler.h>
#include <math.h>
#include "config.h"
#include "localTime.h"
#include "vfs.h"

/*
 * include display and touchscreen libs
 * 
 * both from http://www.rinkydinkelectronics.com
 */
#include <UTFT.h>
#include <URTouch.h>

/* jpeg image facility */
#include "image_jpeg.h"

/* raw image facility */
#include "image_raw.h"

/*
 * display SSD1289 pin
 * 
 * Size: 320x240
 */
#define TFT_CHIP SSD1289
#define TFT_RS  38
#define TFT_WR  39
#define TFT_CS  40
#define TFT_RST 41

/*
 * touch pin
 */
#define TOUCH_TCLK  5
#define TOUCH_TCS   6
#define TOUCH_TDIN  7
#define TOUCH_DOUT  11
#define TOUCH_IRQ   12

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
 * how many ms of delay in the display loop
 */
#define DISPLAY_LOOP_TIME 50

/*
 * how many loops before change the image 5000
 * 
 * ATTENTION: this value is dependend on the loop delay()
 */
#define PHOTO_LOOP_TIME 500

/* 
 * usage
 * middle_screen(this->x_size, sizeof(line)*lcd.getFontXsize())
 */
#define middle_screen(x, y) (x - y)/2

/* where the clock is centered on the screen */
#define clockCenterX 119
#define clockCenterY 119

/*
 * enum to define which window are available
 * needed to check easily where we are
 */
enum window {
    welcome = 0,
    clock = 1,
    photo = 2
};

class Display
{
public:
    Display();
    void init();
    void drawWelcome();
    void loop();

    // in which window we are
    uint8_t window_status;

    // power on or off the display
    void lcdOn();
    void lcdOff();

    void drawClock();
    void printDate();

    // print the phrase "Married from"
    void printMariageMessage();
    // clear message
    void clearMariageMessage();

    // print seconds from mariage day
    void printMariageSeconds(TimeElements t);
    // print days from mariage day
    void printMariageDays(TimeElements t);    
    // clear mariage message and value
    void clearMariage();

    // status of mariage message
    // not very clean to expose these; should be better
    // to create a signal that alert other functions when
    // we change window
    bool mariage_seconds_enabled;
    bool mariage_days_enabled;

    // redraw hand
    void DrawHand();

    // draw photos from sd on the screen
    void DrawPhoto();

private:
    // screen size
    uint16_t x_size, y_size;

    // used in the loop
    uint16_t x, y;

    // time used on the clock
    TimeElements t;

    // manage standby/backlight status
    bool lcd_standby;
    bool lcd_backlight;

    /* clock */
    time_t oldsec;
    void drawMark(int h);
    void drawSec(int s);
    void drawMin(int m);
    void drawHour(int h, int m);
    void clearDate();

    // photo used position on the array
    uint16_t photo_position;
    File photoFile;

    // print error
    void DrawError(uint8_t e, char* err);

    // variable to count cycle before photo change
    uint32_t photo_time;

    // jpeg status error on decoding
    int8_t resJpegDecode;
};

extern Display display;

#endif // _DISPLAY_H
