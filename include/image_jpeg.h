/*
 * image_jpeg.h - Library to render jpeg file with UTFT
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _IMAGE_JPEG_H
#define _IMAGE_JPEG_H

#include "config.h"
/*
 * include display and touchscreen libs
 * 
 * both from http://www.rinkydinkelectronics.com
 */
#include <UTFT.h>

/*
 * JPEG decoder library
 * from https://github.com/Bodmer/JPEGDecoder
 */
#include <JPEGDecoder.h>

/* Return the minimum of two values a and b */
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))

class ImageJpeg
{
    public:
        ImageJpeg();
        void init(UTFT *ptrUTFT, uint32_t tft_cs);
        // display info about jpeg file
        void jpegInfo();
        // decode jpeg file
        int decodeSdFile(const char* file);
        // render file to UTFT
        void renderJPEG(int xpos, int ypos);

    private:
        UTFT *_UTFT;
        uint32_t _tft_cs;
};

extern ImageJpeg image_jpeg;

#endif // _IMAGE_JPEG_H