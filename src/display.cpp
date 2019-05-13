/*
 * display.cpp - implement display and touch screen classes
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#include "display.h"

/* TODO move to private */
UTFT lcd(ITDB32S, 38, 39, 40, 41);
URTouch touch(5, 6, 7, 11, 12);

// Declare which fonts we will be using
// TODO change
extern uint8_t BigFont[];

Display::Display()
{
}

/* initialize method */
void Display::init()
{
  // initialize backlight and standby to on
  pinMode(LCD_STANDBY_PIN, OUTPUT);
  digitalWrite(LCD_STANDBY_PIN, 0);
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
  digitalWrite(LCD_BACKLIGHT_PIN, 0);

  // initialile LCD screen
  lcd.InitLCD();
  lcd.clrScr();

  // initialize touch scree
  touch.InitTouch();
  // PREC_MEDIUM or PREC_HI
  touch.setPrecision(PREC_HI);

  // default behaviour
  lcd.setFont(BigFont);
  lcd.setBackColor(0, 0, 255);
}

/* loop method */
SchedulerTask Display::loop()
{
  while (1)
  {
    yield();
  }

  // avoid warning
  return nullptr;
}

/* instantiate the display */
Display display;
