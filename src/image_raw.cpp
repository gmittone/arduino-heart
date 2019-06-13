/*
 * image_raw.cpp - implement raw rendering routing with UTFT
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#include "image_raw.h"

ImageRaw::ImageRaw()
{
}

void ImageRaw::init(UTFT *ptrUTFT)
{
    _UTFT = ptrUTFT;
}

int ImageRaw::load(int x, int y, int sx, int sy, char *filename, int bufmult, bool iswap)
{
    char buf[2 * bufmult * sx];
    int cx, cy, cp;
    word result;
    dataFile = SD.open(filename);
    if (dataFile)
    {
        cbi(_UTFT->P_CS, _UTFT->B_CS);
        cx = 0;
        cy = 0;
        result = bufmult * sx;
        if (_UTFT->orient == PORTRAIT)
        {
            _UTFT->setXY(x, y, x + sx - 1, y + sy - 1);
        }
        for (int n = 0; n < sy; n += bufmult)
        {
            result = dataFile.read(&buf, (2 * bufmult) * sx);
            if (_UTFT->orient == PORTRAIT)
            {
                for (int i = 0; i < result; i += 2)
                {
                    if (iswap == 1)
                    {
                        _UTFT->LCD_Write_DATA(buf[i + 1], buf[i]);
                    }
                    else
                    {
                        _UTFT->LCD_Write_DATA(buf[i], buf[i + 1]);
                    }
                }
            }
            else
            {
                cp = 0;
                while (cp < result)
                {
                    if (((result - cp) / 2) < (sx - cx))
                    {
                        _UTFT->setXY(x + cx, y + cy, x + cx + ((result - cp) / 2) - 1, y + cy);
                        for (int i = (result - cp) - 2; i >= 0; i -= 2)
                        {
                            if (iswap == 1)
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + i + 1], buf[cp + i]);
                            }
                            else
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + i], buf[cp + i + 1]);
                            }
                        }
                        cx += ((result - cp) / 2);
                        cp = result;
                    }
                    else
                    {
                        _UTFT->setXY(x + cx, y + cy, x + sx - 1, y + cy);
                        for (int i = sx - cx - 1; i >= 0; i--)
                        {
                            if (iswap)
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + (i * 2) + 1], buf[cp + (i * 2)]);
                            }
                            else
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + (i * 2)], buf[cp + (i * 2) + 1]);
                            }
                        }
                        cp += (sx - cx) * 2;
                        cx = 0;
                        cy++;
                    }
                }
            }
        }
        dataFile.close();
        _UTFT->setXY(0, 0, _UTFT->getDisplayXSize() - 1, _UTFT->getDisplayYSize() - 1);
        sbi(_UTFT->P_CS, _UTFT->B_CS);
        return 0;
    }
    else
    {
        return 99;
    }
}

#if defined(__AVR__)
int ImageRaw::loadS(int x, int y, int sx, int sy, int bufmult, bool iswap)
{
    while (!Serial.find("IMAGE"))
        ; // Wait for IMAGE command from host
    byte buf[2 * bufmult * sx];
    int cx, cy, cp;
    word result;
    while (!Serial.available())
        ;
    if (Serial.available() > 0)
    {
        cbi(_UTFT->P_CS, _UTFT->B_CS);
        cx = 0;
        cy = 0;
        result = bufmult * sx;
        if (_UTFT->orient == PORTRAIT)
        {
            _UTFT->setXY(x, y, x + sx - 1, y + sy - 1);
        }
        for (int n = 0; n < sy; n += bufmult)
        {
            result = Serial.readBytes(buf, ((2 * bufmult) * sx));
            if (_UTFT->orient == PORTRAIT)
            {
                for (int i = 0; i < result; i += 2)
                {
                    if (iswap == 1)
                    {
                        _UTFT->LCD_Write_DATA(buf[i + 1], buf[i]);
                    }
                    else
                    {
                        _UTFT->LCD_Write_DATA(buf[i], buf[i + 1]);
                    }
                }
            }
            else
            {
                cp = 0;
                while (cp < result)
                {
                    if (((result - cp) / 2) < (sx - cx))
                    {
                        _UTFT->setXY(x + cx, y + cy, x + cx + ((result - cp) / 2) - 1, y + cy);
                        for (int i = (result - cp) - 2; i >= 0; i -= 2)
                        {
                            if (iswap == 1)
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + i + 1], buf[cp + i]);
                            }
                            else
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + i], buf[cp + i + 1]);
                            }
                        }
                        cx += ((result - cp) / 2);
                        cp = result;
                    }
                    else
                    {
                        _UTFT->setXY(x + cx, y + cy, x + sx - 1, y + cy);
                        for (int i = sx - cx - 1; i >= 0; i--)
                        {
                            if (iswap)
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + (i * 2) + 1], buf[cp + (i * 2)]);
                            }
                            else
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + (i * 2)], buf[cp + (i * 2) + 1]);
                            }
                        }
                        cp += (sx - cx) * 2;
                        cx = 0;
                        cy++;
                    }
                }
            }
        }
        _UTFT->setXY(0, 0, _UTFT->getDisplayXSize() - 1, _UTFT->getDisplayYSize() - 1);
        sbi(_UTFT->P_CS, _UTFT->B_CS);
        return 0;
    }
}
#elif defined(__arm__)
int ImageRaw::loadS(UARTClass port, int x, int y, int sx, int sy, int bufmult, bool iswap)
{
    while (!port.find("IMAGE"))
        ; // Wait for IMAGE command from host
    byte buf[2 * bufmult * sx];
    int cx, cy, cp;
    word result;
    while ((cy = port.available()) == 0)
        ;
    if (cy = (port.available()) > 0)
    {
        cbi(_UTFT->P_CS, _UTFT->B_CS);
        cx = 0;
        cy = 0;
        result = bufmult * sx;
        if (_UTFT->orient == PORTRAIT)
        {
            _UTFT->setXY(x, y, x + sx - 1, y + sy - 1);
        }
        for (int n = 0; n < sy; n += bufmult)
        {
            result = port.readBytes(buf, ((2 * bufmult) * sx));
            if (_UTFT->orient == PORTRAIT)
            {
                for (int i = 0; i < result; i += 2)
                {
                    if (iswap == 1)
                    {
                        _UTFT->LCD_Write_DATA(buf[i + 1], buf[i]);
                    }
                    else
                    {
                        _UTFT->LCD_Write_DATA(buf[i], buf[i + 1]);
                    }
                }
            }
            else
            {
                cp = 0;
                while (cp < result)
                {
                    if (((result - cp) / 2) < (sx - cx))
                    {
                        _UTFT->setXY(x + cx, y + cy, x + cx + ((result - cp) / 2) - 1, y + cy);
                        for (int i = (result - cp) - 2; i >= 0; i -= 2)
                        {
                            if (iswap == 1)
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + i + 1], buf[cp + i]);
                            }
                            else
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + i], buf[cp + i + 1]);
                            }
                        }
                        cx += ((result - cp) / 2);
                        cp = result;
                    }
                    else
                    {
                        _UTFT->setXY(x + cx, y + cy, x + sx - 1, y + cy);
                        for (int i = sx - cx - 1; i >= 0; i--)
                        {
                            if (iswap)
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + (i * 2) + 1], buf[cp + (i * 2)]);
                            }
                            else
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + (i * 2)], buf[cp + (i * 2) + 1]);
                            }
                        }
                        cp += (sx - cx) * 2;
                        cx = 0;
                        cy++;
                    }
                }
            }
        }
        _UTFT->setXY(0, 0, _UTFT->getDisplayXSize() - 1, _UTFT->getDisplayYSize() - 1);
        sbi(_UTFT->P_CS, _UTFT->B_CS);
        return 0;
    }
    else
    {
        return 99;
    }
}

int ImageRaw::loadS(Serial_ port, int x, int y, int sx, int sy, int bufmult, bool iswap)
{
    while (!port.find("IMAGE"))
        ; // Wait for IMAGE command from host
    byte buf[2 * bufmult * sx];
    int cx, cy, cp;
    word result;
    while ((cy = port.available()) == 0)
        ;
    if (cy = (port.available()) > 0)
    {
        cbi(_UTFT->P_CS, _UTFT->B_CS);
        cx = 0;
        cy = 0;
        result = bufmult * sx;
        if (_UTFT->orient == PORTRAIT)
        {
            _UTFT->setXY(x, y, x + sx - 1, y + sy - 1);
        }
        for (int n = 0; n < sy; n += bufmult)
        {
            result = port.readBytes(buf, ((2 * bufmult) * sx));
            if (_UTFT->orient == PORTRAIT)
            {
                for (int i = 0; i < result; i += 2)
                {
                    if (iswap == 1)
                    {
                        _UTFT->LCD_Write_DATA(buf[i + 1], buf[i]);
                    }
                    else
                    {
                        _UTFT->LCD_Write_DATA(buf[i], buf[i + 1]);
                    }
                }
            }
            else
            {
                cp = 0;
                while (cp < result)
                {
                    if (((result - cp) / 2) < (sx - cx))
                    {
                        _UTFT->setXY(x + cx, y + cy, x + cx + ((result - cp) / 2) - 1, y + cy);
                        for (int i = (result - cp) - 2; i >= 0; i -= 2)
                        {
                            if (iswap == 1)
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + i + 1], buf[cp + i]);
                            }
                            else
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + i], buf[cp + i + 1]);
                            }
                        }
                        cx += ((result - cp) / 2);
                        cp = result;
                    }
                    else
                    {
                        _UTFT->setXY(x + cx, y + cy, x + sx - 1, y + cy);
                        for (int i = sx - cx - 1; i >= 0; i--)
                        {
                            if (iswap)
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + (i * 2) + 1], buf[cp + (i * 2)]);
                            }
                            else
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + (i * 2)], buf[cp + (i * 2) + 1]);
                            }
                        }
                        cp += (sx - cx) * 2;
                        cx = 0;
                        cy++;
                    }
                }
            }
        }
        _UTFT->setXY(0, 0, _UTFT->getDisplayXSize() - 1, _UTFT->getDisplayYSize() - 1);
        sbi(_UTFT->P_CS, _UTFT->B_CS);
        return 0;
    }
    else
    {
        return 99;
    }
}
#endif

int ImageRaw::loadcpld(int x, int y, int sx, int sy, char *filename, int writepage, int bufmult, bool iswap)
{
    char buf[(sx * bufmult * 2)];
    _UTFT->setWritePage(writepage);
    int cx, cy, cp;
    word result;
    dataFile = SD.open(filename);
    if (dataFile)
    {
        cbi(_UTFT->P_CS, _UTFT->B_CS);
        cx = 0;
        cy = 0;
        result = bufmult * sx;
        if (_UTFT->orient == PORTRAIT)
        {
            _UTFT->setXY(x, y, x + sx - 1, y + sy - 1);
        }
        for (int n = 0; n < sy; n += bufmult)
        {
            result = dataFile.read(&buf, (sx * 2 * bufmult));
            if (_UTFT->orient == PORTRAIT)
            {
                for (int i = 0; i < result; i += 2)
                {
                    if (iswap)
                    {
                        _UTFT->LCD_Write_DATA(buf[i + 1], buf[i]);
                    }
                    else
                    {
                        _UTFT->LCD_Write_DATA(buf[i], buf[i + 1]);
                    }
                }
            }
            else
            {
                cp = 0;
                while (cp < result)
                {
                    if (((result - cp) / 2) < (sx - cx))
                    {
                        _UTFT->setXY(x + cx, y + cy, x + cx + ((result - cp) / 2) - 1, y + cy);
                        for (int i = (result - cp) - 2; i >= 0; i -= 2)
                        {
                            if (iswap)
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + i + 1], buf[cp + i]);
                            }
                            else
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + i], buf[cp + i + 1]);
                            }
                        }
                        cx += ((result - cp) / 2);
                        cp = result;
                    }
                    else
                    {
                        _UTFT->setXY(x + cx, y + cy, x + sx - 1, y + cy);
                        for (int i = sx - cx - 1; i >= 0; i--)
                        {
                            if (iswap)
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + (i * 2) + 1], buf[cp + (i * 2)]);
                            }
                            else
                            {
                                _UTFT->LCD_Write_DATA(buf[cp + (i * 2)], buf[cp + (i * 2) + 1]);
                            }
                        }
                        cp += (sx - cx) * 2;
                        cx = 0;
                        cy++;
                    }
                }
            }
        }
        dataFile.close();
        _UTFT->setXY(0, 0, _UTFT->getDisplayXSize() - 1, _UTFT->getDisplayYSize() - 1);
        sbi(_UTFT->P_CS, _UTFT->B_CS);
        return 0;
    }
    else
    {
        return 99;
    }
}

int ImageRaw::pan(int dispx, int dispy, int sx, int sy, unsigned long offsetx, unsigned long offsety, unsigned long sizex, unsigned long sizey, char *filename, bool iswap)
{
    char buffer[2 * sx];
    int cx, cy, cp;
    word temp, result;
    unsigned long temp1, temp2;
    temp1 = sizex * 2;
    temp2 = offsetx * 2;
    dataFile = SD.open(filename);
    if (dataFile)
    {
        cbi(_UTFT->P_CS, _UTFT->B_CS);
        cx = 0;
        cy = 0;
        result = sx;
        if (_UTFT->orient == PORTRAIT)
        {
            _UTFT->setXY(dispx, dispy, dispx + sx - 1, dispy + sy - 1);
        }
        for (int n = 0; n < sy; n++)
        {
            dataFile.seek(((n + offsety) * temp1) + temp2);
            result = dataFile.read(&buffer, 2 * sx);
            if (_UTFT->orient == PORTRAIT)
            {
                for (int i = 0; i < result; i += 2)
                {
                    if (iswap == 1)
                    {
                        _UTFT->LCD_Write_DATA(buffer[i + 1], buffer[i]);
                    }
                    else
                    {
                        _UTFT->LCD_Write_DATA(buffer[i], buffer[i + 1]);
                    }
                }
            }
            else
            {
                cp = 0;
                while (cp < result)
                {
                    if (((result - cp) / 2) < (sx - cx))
                    {
                        _UTFT->setXY(dispx + cx, dispy + cy, dispx + cx + ((result - cp) / 2) - 1, dispy + cy);
                        for (int i = (result - cp) - 2; i >= 0; i -= 2)
                        {
                            if (iswap == 1)
                            {
                                _UTFT->LCD_Write_DATA(buffer[cp + i + 1], buffer[cp + i]);
                            }
                            else
                            {
                                _UTFT->LCD_Write_DATA(buffer[cp + i], buffer[cp + i + 1]);
                            }
                        }
                        cx += ((result - cp) / 2);
                        cp = result;
                    }
                    else
                    {
                        _UTFT->setXY(dispx + cx, dispy + cy, dispx + sx - 1, dispy + cy);
                        for (int i = sx - cx - 1; i >= 0; i--)
                        {
                            if (iswap == 1)
                            {
                                _UTFT->LCD_Write_DATA(buffer[cp + (i * 2) + 1], buffer[cp + (i * 2)]);
                            }
                            else
                            {
                                _UTFT->LCD_Write_DATA(buffer[cp + (i * 2)], buffer[cp + (i * 2) + 1]);
                            }
                        }
                        cp += (sx - cx) * 2;
                        cx = 0;
                        cy++;
                    }
                }
            }
        }
        dataFile.close();
        _UTFT->setXY(0, 0, _UTFT->getDisplayXSize() - 1, _UTFT->getDisplayYSize() - 1);
        sbi(_UTFT->P_CS, _UTFT->B_CS);
        return 0;
    }
    else
    {
        return 99;
    }
}

// instantiate the class
ImageRaw image_raw;