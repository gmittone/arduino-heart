/*
 * cts.cpp - Implementation of Capacitive Touch Sensor (pad)
 *           like, giovanni/beatrice signature
 *           cts = Capactive Touch Sensor
 *
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#include "cts.h"

Cts::Cts()
{
}

/* initialize the capactive sensors */
void Cts::init()
{
    Serial.print("\nInitializing capacitive sensors (pads)...");

    // configure port mode
    pinMode(ctsGiovanni, INPUT);
    pinMode(ctsBeatrice, INPUT);

    this->cts_giovanni = false;
    this->cts_beatrice = false;
}

/* manage loop */
SchedulerTask Cts::loop()
{
    // TODO: implement filters to get only one touch
    while (1)
    {
        if (digitalRead(ctsGiovanni) && !this->cts_giovanni)
        {
            this->cts_giovanni = true;
#if ENABLE_DEBUG
            Serial.println("Giovanni TOUCHED");
#endif
        }
        else if (this->cts_giovanni)
        {
            this->cts_giovanni = false;
        }

        if (digitalRead(ctsBeatrice) && !this->cts_beatrice)
        {
            this->cts_beatrice = true;
#if ENABLE_DEBUG
            Serial.println("Beatrice TOUCHED");
#endif
        }
        else if (this->cts_beatrice)
        {
            this->cts_beatrice = false;
        }

        // 100ms seems good
        delay(100);
        yield();
    }

    // avoid warning
    return nullptr;
}

/* instantiate the class */
Cts cts;
