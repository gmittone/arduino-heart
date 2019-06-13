/*
 * vfs.h - Library to manage the sd card
 *
 * Created by Davide `rainbow` Gerhard
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

/*
 * how many photos I can index
 */
#define SD_MAXIMUM_PHOTO_FILES 256

/*
 * maximum length of the file with path
 */
#define SD_MAXIMUM_PHOTO_FILE_LENGTH 256

/*
 * identify file type, used on photo for example
 */
enum file_type {
    notype = 0,
    raw = 1,
    jpeg = 2
};

class VFS
{
public:
    VFS();
    void init();
    // return the photos file string
    void getPhotos();

    // photos filename array
    char* photosList[SD_MAXIMUM_PHOTO_FILES];
    // list that contains the file type of photosList
    // not as structure to minimize memory
    enum file_type photosType[SD_MAXIMUM_PHOTO_FILES];
    uint16_t photos_number;

    // simplify the check using a boolean variable
    bool ready;

private:
    Sd2Card card;
    SdVolume volume;
    SdFile root;
};

extern VFS vfs;

#endif // _VFS_H
