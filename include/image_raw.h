/*
 * image_raw.h - Library to render raw file with UTFT
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _IMAGE_RAW_H
#define _IMAGE_RAW_H

#include "config.h"
#include "vfs.h"

/*
 * include display and touchscreen libs
 * 
 * both from http://www.rinkydinkelectronics.com
 */
#include <UTFT.h>

class ImageRaw
{
public:
    ImageRaw();
    void init(UTFT *ptrUTFT);

    // from https://github.com/ghlawrence2000/UTFT_SdRaw/blob/master/src/UTFT_SdRaw.cpp
    // CC BY-NC-SA 3.0 license
    int load(int x, int y, int sx, int sy, char *filename, int bufmult, bool iswap);
#if defined(__AVR__)
    int loadS(int x, int y, int sx, int sy, int bufmult, bool iswap);
#elif defined(__arm__)
    int loadS(Serial_ port, int x, int y, int sx, int sy, int bufmult, bool iswap);
    int loadS(UARTClass port, int x, int y, int sx, int sy, int bufmult, bool iswap);
#endif
    int loadcpld(int x, int y, int sx, int sy, char *filename, int writepage = 0, int bufmult = 1, bool iswap = 0);
    int pan(int x, int y, int sx, int sy, unsigned long offsetx, unsigned long offsety, unsigned long sizex, unsigned long sizey, char *filename, bool iswap = 0);

private:
    UTFT *_UTFT;
    File dataFile;
};

extern ImageRaw image_raw;

#endif _IMAGE_RAW_H