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
}

void VFS::init()
{
    // initialize stanby of the sd card
    pinMode(SD_STANDBY_PIN, OUTPUT);
    digitalWrite(SD_STANDBY_PIN, 0);

    Serial.print("\nInitializing SD card...");

    // we'll use the initialization code from the utility libraries
    // since we're just testing if the card is working!
    if (!card.init(SPI_HALF_SPEED, SD_CS_PIN))
    {
        Serial.println("\nSD card: initialization failed.");
#if ENABLE_DEBUG
        Serial.println("Things to check:");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chip? Select pin to match your shield or module?");
#endif

        // if not present return and go on
        return;
    }
#if ENABLE_DEBUG
    else
    {
        Serial.println("SD: Wiring is correct and a card is present.");
    }
#endif

#if ENABLE_DEBUG
    // print the type of card
    Serial.println();
    Serial.print("Card type:         ");
    switch (card.type())
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
    if (!volume.init(card))
    {
        Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
        return;
    }

#if ENABLE_DEBUG
    Serial.print("Clusters:          ");
    Serial.println(volume.clusterCount());
    Serial.print("Blocks x Cluster:  ");
    Serial.println(volume.blocksPerCluster());

    Serial.print("Total Blocks:      ");
    Serial.println(volume.blocksPerCluster() * volume.clusterCount());
    Serial.println();

    // print the type and size of the first FAT-type volume
    uint32_t volumesize;
    Serial.print("Volume type is:    FAT");
    Serial.println(volume.fatType(), DEC);

    volumesize = volume.blocksPerCluster(); // clusters are collections of blocks
    volumesize *= volume.clusterCount();    // we'll have a lot of clusters
    volumesize /= 2;                        // SD card blocks are always 512 bytes (2 blocks are 1KB)
    Serial.print("Volume size (Kb):  ");
    Serial.println(volumesize);
    Serial.print("Volume size (Mb):  ");
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.print("Volume size (Gb):  ");
    Serial.println((float)volumesize / 1024.0);
#endif

    root.openRoot(volume);

#if ENABLE_DEBUG
    Serial.println("\nFiles found on the card (name, date and size in bytes): ");
    // list all files in the card with date and size
    root.ls(LS_R | LS_DATE | LS_SIZE);
#endif

    // the card is ready to be used
    ready = true;
}

/* instantiate the class */
VFS vfs;
