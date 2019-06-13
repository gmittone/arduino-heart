/*
 * vfs.cpp - implement interface to sd card
 *
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#include "vfs.h"

VFS::VFS()
{
    this->ready = false;
    this->photos_number = 0;
}

void VFS::init()
{
    // initialize stanby of the sd card
    pinMode(SD_STANDBY_PIN, OUTPUT);
    digitalWrite(SD_STANDBY_PIN, 0);

    Serial.println("\nInitializing SD card...");

    // we'll use the initialization code from the utility libraries
    // since we're just testing if the card is working!
    if (!this->card.init(SPI_FULL_SPEED, SD_CS_PIN))
    {
        Serial.println("\nSD card: initialization failed.");
#if ENABLE_VFS_DEBUG
        Serial.println("Things to check:");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chip? Select pin to match your shield or module?");
#endif
        // if not present return and go on
        return;
    }
#if ENABLE_VFS_DEBUG
    else
    {
        Serial.println("SD: Wiring is correct and a card is present.");
    }
#endif

#if ENABLE_VFS_DEBUG
    // print the type of card
    Serial.println();
    Serial.print("Card type:         ");
    switch (this->card.type())
    {
    case SD_CARD_TYPE_SD1:
        Serial.println("SD1");
        break;
    case SD_CARD_TYPE_SD2:
        Serial.println("SD2");
        break;
    case SD_CARD_TYPE_SDHC:
        Serial.println("SDHC");
        break;
    default:
        Serial.println("Unknown");
    }
#endif

    // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
    if (!this->volume.init(this->card))
    {
        Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
        return;
    }

#if ENABLE_VFS_DEBUG
    Serial.print("Clusters:          ");
    Serial.println(this->volume.clusterCount());
    Serial.print("Blocks x Cluster:  ");
    Serial.println(this->volume.blocksPerCluster());

    Serial.print("Total Blocks:      ");
    Serial.println(this->volume.blocksPerCluster() * this->volume.clusterCount());
    Serial.println();

    // print the type and size of the first FAT-type volume
    uint32_t volumesize;
    Serial.print("Volume type is:    FAT");
    Serial.println(this->volume.fatType(), DEC);

    volumesize = this->volume.blocksPerCluster(); // clusters are collections of blocks
    volumesize *= this->volume.clusterCount();    // we'll have a lot of clusters
    volumesize /= 2;                              // SD card blocks are always 512 bytes (2 blocks are 1KB)
    Serial.print("Volume size (Kb):  ");
    Serial.println(volumesize);
    Serial.print("Volume size (Mb):  ");
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.print("Volume size (Gb):  ");
    Serial.println((float)volumesize / 1024.0);
#endif

    this->root.openRoot(this->volume);

#if ENABLE_VFS_DEBUG
    Serial.println("\nFiles found on the card (name, date and size in bytes): ");
    // list all files in the card with date and size
    this->root.ls(LS_R | LS_DATE | LS_SIZE);
#endif

    // now initialize the main object
    if (SD.begin(SD_CS_PIN))
    {
        this->ready = true;
        this->getPhotos();
        Serial.println("SD card initialized correctly.");
    }
    else
    {
        Serial.println("SD card initialization failed.");
    }
}

void VFS::getPhotos()
{
    if (this->ready)
    {
        File photos = SD.open(SD_PHOTOS_FOLDER);
        File entry;
        char name[SD_MAXIMUM_PHOTO_FILE_LENGTH];

        while (true)
        {
            entry = photos.openNextFile();
            // exit if no more file exists
            if (!entry)
            {
                break;
            }

            // if it is not a directory
            if (!entry.isDirectory())
            {
                memset(name, '\0', sizeof(name));
                strcpy(name, SD_PHOTOS_FOLDER "/");
                strcat(name, entry.name());

                // to lower case
                for (int i = 0; i < SD_MAXIMUM_PHOTO_FILE_LENGTH; i++)
                {
                    name[i] = tolower(name[i]);
                }
                if (this->photos_number < SD_MAXIMUM_PHOTO_FILES)
                {
#if ENABLE_VFS_DEBUG
                    Serial.println("Found photo file: " + String(name));
#endif
                    if (strstr(name, "jpg") != NULL || strstr(name, "jpeg") != NULL)
                    {
                        this->photosList[this->photos_number] = strndup(name, strlen(name));
                        this->photosType[this->photos_number] = jpeg;
                        this->photos_number++;
                    }
                    else if (strstr(name, "raw") != NULL)
                    {
                        this->photosList[this->photos_number] = strndup(name, strlen(name));
                        this->photosType[this->photos_number] = raw;
                        this->photos_number++;
                    }
                }
                else
                {
                    Serial.println("Too many photos, the maximum is " + SD_MAXIMUM_PHOTO_FILES);
                }
            }
        }
    }
}

/* instantiate the class */
VFS vfs;
