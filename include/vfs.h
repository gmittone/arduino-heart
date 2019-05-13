/*
 * vfs.h - Library to manage the sd card
 *
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _VFS_H
#define _VFS_H

#include <SPI.h>
#include <SD.h>
#include "config.h"

/*
 * Connected to main SPI bus
 * chip select (CS) = digital pin 3
 */
#define SD_CS_PIN 3

/*
 * output low ==> SD on
 * input hi-z (out hi?) ==> SD off
 */
#define SD_STANDBY_PIN 48

class VFS
{
public:
    VFS();
    void init();

private:
    Sd2Card card;
    SdVolume volume;
    SdFile root;
    // simplify the check using a boolean variable
    bool sd_ready;
};

extern VFS vfs;

#endif // _VFS_H