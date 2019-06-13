/*
 * display.cpp - implement display and touch screen classes
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#include "display.h"

/* SSD1289 controller */
UTFT lcd(TFT_CHIP, TFT_RS, TFT_WR, TFT_CS, TFT_RST);
/* TSC2046 controller */
URTouch touch(TOUCH_TCLK, TOUCH_TCS, TOUCH_TDIN, TOUCH_DOUT, TOUCH_IRQ);

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
  this->x_size = lcd.getDisplayYSize();
  this->y_size = lcd.getDisplayXSize();

  this->oldsec = 0;

  this->mariage_seconds_enabled = false;
  this->mariage_days_enabled = false;

  this->photo_position = 0;

  this->photo_time = 0;
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

  // initialize the jpeg image object
  image_jpeg.init(&lcd, TFT_CS);

  // initialize the raw image object
  image_raw.init(&lcd);

  // enable display
  this->lcdOn();
}

/* loop method */
void Display::loop()
{
  switch (this->window_status)
  {
  // clock window
  case clock:
    this->t = localTime.getTime();
    if (this->oldsec != this->t.Second)
    {
      if ((this->t.Second == 0) and (this->t.Minute == 0) and (this->t.Hour == 0))
      {
        this->clearDate();
        this->printDate();
      }
      if (this->t.Second == 0)
      {
        this->drawMin(this->t.Minute);
        this->drawHour(this->t.Hour, this->t.Minute);

        // update mariage days
        if (this->mariage_days_enabled)
        {
          this->printMariageDays(this->t);
        }
      }
      this->drawSec(this->t.Second);

      // update mariage unixtimestamp
      if (this->mariage_seconds_enabled)
      {
        this->printMariageSeconds(this->t);
      }

      // update the mariage value if enabled
      if (this->mariage_seconds_enabled)
      {
        this->printMariageSeconds(this->t);
      }
      if (this->mariage_days_enabled)
      {
        this->printMariageDays(this->t);
      }

      this->oldsec = this->t.Second;
    }
    if (touch.dataAvailable())
    {
      Serial.println("Touch data available");
      touch.read();
      this->x = touch.getX();
      this->y = touch.getY();

      // Setting button
      /*
      if (((this->y >= 150) && (this->y <= 189)) && ((this->x >= 260) && (this->x <= 319)))
      {
#if ENABLE_DISPLAY_DEBUG
        Serial.println("Touched the setting button. Move to Setting screen.");
#endif
        lcd.setColor(255, 0, 0);
        lcd.drawRoundRect(260, 150, 319, 189);
      }
      */

      // Photo button
      if (((this->y >= 200) && (this->y <= 239)) && ((this->x >= 260) && (this->x <= 319)))
      {
#if ENABLE_DISPLAY_DEBUG
        Serial.println("Touched the photo button. Move to Photo screen.");
#endif
        this->photo_position = 0;
        this->DrawPhoto();
        this->photo_time = PHOTO_LOOP_TIME;
      }
    }
    break;
  // photo window
  case photo:
    this->photo_time--;
    if (this->photo_time == 0)
    {
      this->photo_position = (this->photo_position + 1) % vfs.photos_number;
      this->DrawPhoto();
      this->photo_time = PHOTO_LOOP_TIME;
    }
    if (touch.dataAvailable())
    {
      Serial.println("Touch data available");

      touch.read();
      this->x = touch.getX();
      this->y = touch.getY();

      // exit button on the lower part of the screen
      if (((this->y >= 200) && (this->y <= 239)) && ((this->x >= 0) && (this->x <= 319)))
      {
#if ENABLE_DISPLAY_DEBUG
        Serial.println("Touched the lower part of the screen. Move to Clock screen.");
#endif
        this->drawClock();
      }
      // next photo in the right end of the screen
      else if (((this->y >= 0) && (this->y <= 239)) && ((this->x >= 260) && (this->x <= 319)))
      {
#if ENABLE_DISPLAY_DEBUG
        Serial.println("Touched the right end of the screen. Move to the next photo.");
#endif

        this->photo_position = (this->photo_position + 1) % vfs.photos_number;
        this->DrawPhoto();
        this->photo_time = PHOTO_LOOP_TIME;
      }
      // previous photo in the left end of the screen
      else if (((this->y >= 0) && (this->y <= 239)) && ((this->x >= 0) && (this->x <= 60)))
      {
#if ENABLE_DISPLAY_DEBUG
        Serial.println("Touched the left end of the screen. Move to the previous photo.");
#endif
        if (this->photo_position > 0)
        {
          this->photo_position--;
          this->DrawPhoto();
          this->photo_time = PHOTO_LOOP_TIME;
        }
      }
    }
    break;
  default:
    break;
  }
  delay(DISPLAY_LOOP_TIME);
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

void Display::drawWelcome()
{
  // set the status
  this->window_status = welcome;

  // Clear screen
  lcd.clrScr();

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

  delay(WELCOME_SCREEN_WAIT);
}

void Display::drawClock()
{
  // set the status
  this->window_status = clock;

  // be sure that we draw a clear screen
  this->mariage_days_enabled = false;
  this->mariage_seconds_enabled = false;

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

  TimeElements t = localTime.getTime();
  drawMin(t.Minute);
  drawHour(t.Hour, t.Minute);
  drawSec(t.Second);
  oldsec = t.Second;

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

  // print the date in the calendar
  this->printDate();

  // Draw set button
  /*
  lcd.setColor(255, 0, 0);
  lcd.fillRoundRect(260, 150, 319, 189);
  lcd.setColor(255, 255, 255);
  lcd.drawRoundRect(260, 150, 319, 189);
  lcd.setBackColor(255, 0, 0);
  lcd.setFont(SmallFont);
  lcd.print("SET", 266, 162);
  lcd.setBackColor(0, 0, 0);
  */

  // Draw photo button
  lcd.setColor(255, 0, 0);
  lcd.fillRoundRect(260, 200, 319, 239);
  lcd.setColor(255, 255, 255);
  lcd.drawRoundRect(260, 200, 319, 239);
  lcd.setBackColor(255, 0, 0);
  lcd.setFont(SmallFont);
  lcd.print("PHOTOS", 266, 212);
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
  TimeElements t_temp;
  t_temp = localTime.getTime();

  lcd.setFont(BigFont);
  lcd.setColor(0, 0, 0);
  lcd.setBackColor(255, 255, 255);
  lcd.print(localTime.getDOWStr(FORMAT_SHORT), 256, 8);
  if (t_temp.Day < 10)
    lcd.printNumI(t_temp.Day, 272, 28);
  else
    lcd.printNumI(t_temp.Day, 264, 28);
  lcd.print(localTime.getMonthStr(FORMAT_SHORT), 256, 48);
  lcd.printNumI(t_temp.Year + 1970, 248, 65);
}

void Display::clearDate()
{
  lcd.setColor(255, 255, 255);
  lcd.fillRect(248, 8, 312, 81);
}

void Display::printMariageMessage()
{
  lcd.setColor(255, 255, 255);
  lcd.setBackColor(0, 0, 0);
  lcd.setFont(SmallFont);
  lcd.print("Married from", middle_screen(clockCenterX * 2, 13 * lcd.getFontXsize()), 50);
}

void Display::clearMariageMessage()
{
  // clear mariage message
  lcd.setColor(255, 255, 255);
  lcd.setBackColor(0, 0, 0);
  lcd.setFont(SmallFont);
  lcd.print("            ", middle_screen(clockCenterX * 2, 13 * lcd.getFontXsize()), 50);
}

void Display::printMariageSeconds(TimeElements t)
{
  lcd.setColor(255, 255, 255);
  lcd.setBackColor(0, 0, 0);
  lcd.setFont(BigFont);

  time_t ut = makeTime(t);

  lcd.printNumI(ut - MARIAGE_UNIXTIMESTAMP, middle_screen(clockCenterX * 2, (log10(ut - MARIAGE_UNIXTIMESTAMP) + 1) * lcd.getFontXsize()), 67);

  this->mariage_seconds_enabled = true;
  this->mariage_days_enabled = false;
}

void Display::printMariageDays(TimeElements t)
{
  lcd.setColor(255, 255, 255);
  lcd.setBackColor(0, 0, 0);
  lcd.setFont(BigFont);

  time_t ut = makeTime(t);
  long int days = (ut - MARIAGE_UNIXTIMESTAMP) / 86400;

  lcd.printNumI(days, middle_screen(clockCenterX * 2, (log10(days) + 1) * lcd.getFontXsize()), 67);

  this->mariage_days_enabled = true;
  this->mariage_seconds_enabled = false;
}

void Display::clearMariage()
{
  // clear mariage value
  lcd.setColor(255, 255, 255);
  lcd.setBackColor(0, 0, 0);
  lcd.setFont(BigFont);
  // quite dirty but ok
  lcd.print("          ", middle_screen(clockCenterX * 2, 11 * lcd.getFontXsize()), 67);

  this->mariage_seconds_enabled = this->mariage_seconds_enabled == true ? false : false;
  this->mariage_days_enabled = this->mariage_days_enabled == true ? false : false;
}

void Display::DrawHand()
{
  drawMin(this->t.Minute);
  drawHour(this->t.Hour, this->t.Minute);
}

void Display::DrawPhoto()
{
  // set the status
  this->window_status = photo;

  // Clear screen
  lcd.clrScr();

  if (vfs.ready)
  {
#if ENABLE_DISPLAY_DEBUG
    photoFile = SD.open(vfs.photosList[this->photo_position], O_READ);
    if (!photoFile)
    {
      this->DrawError(3774, "SD card open failed.");
    }
    else
    {
      Serial.println("File opened sucessfully");
    }
#endif

    switch (vfs.photosType[this->photo_position])
    {
    case jpeg:
      // try to decode
      this->resJpegDecode = image_jpeg.decodeSdFile(vfs.photosList[this->photo_position]);
      if (this->resJpegDecode)
      {
        // now we can draw
#if ENABLE_DISPLAY_DEBUG
        image_jpeg.jpegInfo();
#endif
        // render the image onto the screen at given coordinates (x, y)
        image_jpeg.renderJPEG(0, 0);
      }
      else if (this->resJpegDecode == 0)
      {
        Serial.println("Progressive JPEG files are not supported.");
      }
      else
      {
        // decode failure
        this->DrawError(3775, "Failed to decode jpeg image.");
        return;
      }
      break;
    case raw:
        // draw raw image on the screen
        image_raw.load(0, 0, y_size, x_size, vfs.photosList[this->photo_position], 1, 1);
      break;
    default:
      break;
    }
  }
  else
  {
    this->DrawError(3773, "SD card read failed.");
    return;
  }
}

// Windows like ;-)
// Parameters:
//   e   : error number
//   err : error string
void Display::DrawError(uint8_t e, char *err)
{
  lcd.fillScr(15, 48, 178);
  lcd.setColor(255, 255, 255);
  lcd.setBackColor(15, 48, 178);
  lcd.setFont(BigFont);
  lcd.print("Error: " + String(e, DEC), middle_screen(y_size, 12 * lcd.getFontXsize()), 80);
  lcd.setFont(SmallFont);
  lcd.print(err, middle_screen(y_size, sizeof(*err) * lcd.getFontXsize()), 100);
}

/* instantiate the display */
Display display;
