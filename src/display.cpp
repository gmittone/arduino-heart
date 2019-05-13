/*
 * display.cpp - implement display and touch screen classes
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#include "display.h"

/* SSD1289 controller */
UTFT lcd(ITDB32S, 38, 39, 40, 41);
/* TSC2046 controller */
URTouch touch(5, 6, 7, 11, 12);

/*
 * import images converted with
 * http://rinkydinkelectronics.com/t_imageconverter565.php
 * 
 * TODO:
 *   - understand if they are freed after usage;
 *     ex. when the method return
 */
#include "display/heart_150x150.h"

// Declare which fonts we will be using
// TODO are you sure? nothing more clear?
extern uint8_t BigFont[];
extern uint8_t SmallFont[];

Display::Display()
{
  // initialize backlight and standby port
  pinMode(LCD_STANDBY_PIN, OUTPUT);
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);

  // landscape mode
  this->y_size = lcd.getDisplayXSize();
  this->x_size = lcd.getDisplayYSize();

  this->oldsec = 0;
}

/* initialize method */
void Display::init()
{
  // initialile LCD screen
  lcd.InitLCD(LANDSCAPE);
  lcd.clrScr();

  // initialize touch scree
  touch.InitTouch();
  // PREC_MEDIUM or PREC_HI
  touch.setPrecision(PREC_HI);

  // default behaviour
  lcd.setFont(BigFont);
  lcd.setBackColor(0, 0, 0);

  // enable display
  this->lcdOn();
}

/* loop method */
void Display::loop()
{
  delay(100);
}

void Display::lcdOn()
{
  // set backlight and standby to on
  digitalWrite(LCD_STANDBY_PIN, 0);
  digitalWrite(LCD_BACKLIGHT_PIN, 0);
}

void Display::lcdOff()
{
  // set backlight and standby to off
  digitalWrite(LCD_STANDBY_PIN, 1);
  digitalWrite(LCD_BACKLIGHT_PIN, 1);
}

void Display::welcomeScreen()
{
  uint8_t middle = this->x_size / 2;

  char line_1[] = "Arduino Heart";
  char line_2[] = "by";
  char line_3[] = "Alessandro =alez= Mauro";
  char line_4[] = "Davide =rainbow= Gerhard";

  lcd.setFont(BigFont);
  lcd.setBackColor(0, 0, 0);
  lcd.drawBitmap(middle - 75, 10, 150, 150, heart_150x150);
  lcd.print(line_1, middle_screen(this->x_size, sizeof(line_1) * lcd.getFontXsize()), 160);
  lcd.setFont(SmallFont);
  lcd.print(line_2, middle_screen(this->x_size, sizeof(line_2) * lcd.getFontXsize()), 180);
  lcd.print(line_3, middle_screen(this->x_size, sizeof(line_3) * lcd.getFontXsize()), 195);
  lcd.print(line_4, middle_screen(this->x_size, sizeof(line_4) * lcd.getFontXsize()), 210);

  // TODO check if necessary
  delay(2000);
}

/************ CLOCK *************/
void Display::drawClock()
{
  // Clear screen
  lcd.clrScr();

  // Draw Clockface
  lcd.setColor(0, 0, 255);
  lcd.setBackColor(0, 0, 0);
  for (int i = 0; i < 5; i++)
  {
    lcd.drawCircle(clockCenterX, clockCenterY, 119 - i);
  }
  for (int i = 0; i < 5; i++)
  {
    lcd.drawCircle(clockCenterX, clockCenterY, i);
  }

  lcd.setColor(192, 192, 255);
  lcd.print("3", clockCenterX + 92, clockCenterY - 8);
  lcd.print("6", clockCenterX - 8, clockCenterY + 95);
  lcd.print("9", clockCenterX - 109, clockCenterY - 8);
  lcd.print("12", clockCenterX - 16, clockCenterY - 109);
  for (int i = 0; i < 12; i++)
  {
    if ((i % 3) != 0)
      drawMark(i);
  }

  RtcDateTime dt;
  dt = heart_RTC.getDateTime();
  drawMin(dt.Minute);
  drawHour(dt.Hour, dt.Minute);
  drawSec(dt.Second);
  this->oldsec=dt.Second;

  // Draw calendar
  lcd.setColor(255, 255, 255);
  lcd.fillRoundRect(240, 0, 319, 85);
  lcd.setColor(0, 0, 0);
  for (int i = 0; i < 7; i++)
  {
    lcd.drawLine(249 + (i * 10), 0, 248 + (i * 10), 3);
    lcd.drawLine(250 + (i * 10), 0, 249 + (i * 10), 3);
    lcd.drawLine(251 + (i * 10), 0, 250 + (i * 10), 3);
  }

  // Draw photo button
  lcd.setColor(255, 0, 0);
  lcd.fillRoundRect(260, 150, 319, 189);
  lcd.setColor(255, 255, 255);
  lcd.drawRoundRect(260, 150, 319, 189);
  lcd.setBackColor(255, 0, 0);
  lcd.print("PHOTO", 266, 162);
  lcd.setBackColor(0, 0, 0);

  // Draw set button
  lcd.setColor(255, 0, 0);
  lcd.fillRoundRect(260, 200, 319, 239);
  lcd.setColor(255, 255, 255);
  lcd.drawRoundRect(260, 200, 319, 239);
  lcd.setBackColor(255, 0, 0);
  lcd.print("SET", 266, 212);
  lcd.setBackColor(0, 0, 0);
}

void Display::drawMark(int h)
{
  float x1, y1, x2, y2;

  h = h * 30;
  h = h + 270;

  x1 = 110 * cos(h * 0.0175);
  y1 = 110 * sin(h * 0.0175);
  x2 = 100 * cos(h * 0.0175);
  y2 = 100 * sin(h * 0.0175);

  lcd.drawLine(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY);
}

void Display::drawSec(int s)
{
  float x1, y1, x2, y2;
  int ps = s - 1;

  lcd.setColor(0, 0, 0);
  if (ps == -1)
    ps = 59;
  ps = ps * 6;
  ps = ps + 270;

  x1 = 95 * cos(ps * 0.0175);
  y1 = 95 * sin(ps * 0.0175);
  x2 = 80 * cos(ps * 0.0175);
  y2 = 80 * sin(ps * 0.0175);

  lcd.drawLine(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY);

  lcd.setColor(255, 0, 0);
  s = s * 6;
  s = s + 270;

  x1 = 95 * cos(s * 0.0175);
  y1 = 95 * sin(s * 0.0175);
  x2 = 80 * cos(s * 0.0175);
  y2 = 80 * sin(s * 0.0175);

  lcd.drawLine(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY);
}

void Display::drawMin(int m)
{
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int pm = m - 1;

  lcd.setColor(0, 0, 0);
  if (pm == -1)
    pm = 59;
  pm = pm * 6;
  pm = pm + 270;

  x1 = 80 * cos(pm * 0.0175);
  y1 = 80 * sin(pm * 0.0175);
  x2 = 5 * cos(pm * 0.0175);
  y2 = 5 * sin(pm * 0.0175);
  x3 = 30 * cos((pm + 4) * 0.0175);
  y3 = 30 * sin((pm + 4) * 0.0175);
  x4 = 30 * cos((pm - 4) * 0.0175);
  y4 = 30 * sin((pm - 4) * 0.0175);

  lcd.drawLine(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY);
  lcd.drawLine(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY);
  lcd.drawLine(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY);
  lcd.drawLine(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY);

  lcd.setColor(0, 255, 0);
  m = m * 6;
  m = m + 270;

  x1 = 80 * cos(m * 0.0175);
  y1 = 80 * sin(m * 0.0175);
  x2 = 5 * cos(m * 0.0175);
  y2 = 5 * sin(m * 0.0175);
  x3 = 30 * cos((m + 4) * 0.0175);
  y3 = 30 * sin((m + 4) * 0.0175);
  x4 = 30 * cos((m - 4) * 0.0175);
  y4 = 30 * sin((m - 4) * 0.0175);

  lcd.drawLine(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY);
  lcd.drawLine(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY);
  lcd.drawLine(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY);
  lcd.drawLine(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY);
}

void Display::drawHour(int h, int m)
{
  float x1, y1, x2, y2, x3, y3, x4, y4;
  int ph = h;

  lcd.setColor(0, 0, 0);
  if (m == 0)
  {
    ph = ((ph - 1) * 30) + ((m + 59) / 2);
  }
  else
  {
    ph = (ph * 30) + ((m - 1) / 2);
  }
  ph = ph + 270;

  x1 = 60 * cos(ph * 0.0175);
  y1 = 60 * sin(ph * 0.0175);
  x2 = 5 * cos(ph * 0.0175);
  y2 = 5 * sin(ph * 0.0175);
  x3 = 20 * cos((ph + 5) * 0.0175);
  y3 = 20 * sin((ph + 5) * 0.0175);
  x4 = 20 * cos((ph - 5) * 0.0175);
  y4 = 20 * sin((ph - 5) * 0.0175);

  lcd.drawLine(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY);
  lcd.drawLine(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY);
  lcd.drawLine(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY);
  lcd.drawLine(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY);

  lcd.setColor(255, 255, 0);
  h = (h * 30) + (m / 2);
  h = h + 270;

  x1 = 60 * cos(h * 0.0175);
  y1 = 60 * sin(h * 0.0175);
  x2 = 5 * cos(h * 0.0175);
  y2 = 5 * sin(h * 0.0175);
  x3 = 20 * cos((h + 5) * 0.0175);
  y3 = 20 * sin((h + 5) * 0.0175);
  x4 = 20 * cos((h - 5) * 0.0175);
  y4 = 20 * sin((h - 5) * 0.0175);

  lcd.drawLine(x1 + clockCenterX, y1 + clockCenterY, x3 + clockCenterX, y3 + clockCenterY);
  lcd.drawLine(x3 + clockCenterX, y3 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY);
  lcd.drawLine(x2 + clockCenterX, y2 + clockCenterY, x4 + clockCenterX, y4 + clockCenterY);
  lcd.drawLine(x4 + clockCenterX, y4 + clockCenterY, x1 + clockCenterX, y1 + clockCenterY);
}


void Display::printDate()
{
  RtcDateTime dt;
  dt = heart_RTC.getDateTime();


  drawMin(dt.Minute);
  drawHour(dt.Hour, dt.Minute);
  drawSec(dt.Second);
  this->oldsec=dt.Second;

  lcd.setFont(BigFont);
  lcd.setColor(0, 0, 0);
  lcd.setBackColor(255, 255, 255);
  lcd.print(rtc.getDOWStr(FORMAT_SHORT), 256, 8);
  if (t_temp.date<10)
    lcd.printNumI(t_temp.date, 272, 28);
  else
    lcd.printNumI(t_temp.date, 264, 28);
  lcd.print(rtc.getMonthStr(FORMAT_SHORT), 256, 48);
  lcd.printNumI(t_temp.year, 248, 65);
}

void Display::clearDate()
{
  lcd.setColor(255, 255, 255);
  lcd.fillRect(248, 8, 312, 81);
}
/********************************/

/* instantiate the display */
Display display;
