/*
 * image_jpeg.cpp - implement jpeg rendering routing with UTFT
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#include "image_jpeg.h"

ImageJpeg::ImageJpeg()
{
}

void ImageJpeg::init(UTFT *ptrUTFT, uint32_t tft_cs)
{
  _UTFT = ptrUTFT;
  _tft_cs = tft_cs;
}

void ImageJpeg::jpegInfo()
{
  Serial.println(F("==============="));
  Serial.println(F("JPEG image info"));
  Serial.println(F("==============="));
  Serial.print(F("Width      :"));
  Serial.println(JpegDec.width);
  Serial.print(F("Height     :"));
  Serial.println(JpegDec.height);
  Serial.print(F("Components :"));
  Serial.println(JpegDec.comps);
  Serial.print(F("MCU / row  :"));
  Serial.println(JpegDec.MCUSPerRow);
  Serial.print(F("MCU / col  :"));
  Serial.println(JpegDec.MCUSPerCol);
  Serial.print(F("Scan type  :"));
  Serial.println(JpegDec.scanType);
  Serial.print(F("MCU width  :"));
  Serial.println(JpegDec.MCUWidth);
  Serial.print(F("MCU height :"));
  Serial.println(JpegDec.MCUHeight);
  Serial.println(F("==============="));
}

int ImageJpeg::decodeSdFile(const char *file)
{
  return JpegDec.decodeSdFile(file);
}

void ImageJpeg::renderJPEG(int xpos, int ypos)
{

  // retrieve infomration about the image
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint16_t max_x = JpegDec.width;
  uint16_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint16_t min_w = minimum(mcu_w, max_x % mcu_w);
  uint16_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint16_t win_w = mcu_w;
  uint16_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // dd
  uint16_t tc, tr;
  uint16_t *tp ;

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while (JpegDec.read())
  {

    // save a pointer to the image block
    pImg = JpegDec.pImage;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right and bottom edges
    if (mcu_x + mcu_w <= max_x)
      win_w = mcu_w;
    else
      win_w = min_w;
    if (mcu_y + mcu_h <= max_y)
      win_h = mcu_h;
    else
      win_h = min_h;

    // calculate how many pixels must be drawn
    uint16_t mcu_pixels = win_w * win_h;

    // draw image MCU block only if it will fit on the screen
    if ((mcu_x + win_w) <= _UTFT->getDisplayXSize() && (mcu_y + win_h) <= _UTFT->getDisplayYSize())
    {
      digitalWrite(_tft_cs, LOW); // Set chip select low so we can take control of display

// Write all MCU pixels to the TFT window
#if LCD_ORIENTATION == PORTAINT
      // Now set a MCU bounding window on the TFT to push pixels into (x, y, x + width - 1, y + height - 1)
      _UTFT->setXY(mcu_x, mcu_y, mcu_x + win_w - 1, mcu_y + win_h - 1);
      while (mcu_pixels--)
      {
        // Push each pixel to the TFT MCU area
        uint8_t col_h = (*pImg) >> 8;        // High byte
        uint8_t col_l = (*pImg) & 0xFF;      // Low byte
        _UTFT->LCD_Write_DATA(col_h, col_l); // Send a pixel colour to window
        pImg++;                              // Increment pointer
      }
#else // LANDSCAPE
      _UTFT->setXY(mcu_x, mcu_y, mcu_x + win_w - 1, mcu_y + win_h - 1);
      for (tc = 1; tc <= win_w; tc++)
      {
        for (tr = 0; tr < win_h; tr++)
        {
          tp = pImg + tr * win_w + win_w - tc;
          _UTFT->LCD_Write_DATA((*tp) >> 8, (*tp) & 0xFF); // Send a pixel colour to window
        }
      }
#endif
    }

    // Stop drawing blocks if the bottom of the screen has been reached,
    // the abort function will close the file
    else if ((mcu_y + win_h) >= _UTFT->getDisplayYSize())
      JpegDec.abort();
  }

  // calculate how long it took to draw the image
  drawTime = millis() - drawTime;

  // print the results to the serial port
#if ENABLE_DISPLAY && ENABLE_DEBUG
  Serial.print(F("Total render time was    : "));
  Serial.print(drawTime);
  Serial.println(F(" ms"));
  Serial.println(F(""));
#endif  
}

// instantiate the class
ImageJpeg image_jpeg;