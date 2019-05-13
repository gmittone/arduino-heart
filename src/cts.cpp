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
    while (1)
    {
        this->pressing_giovanni = false;
        this->pressing_beatrice = false;
        // simple filter
        for (this->samples = 0; SAMPLES_FILTER > this->samples; this->samples++)
        {
            if (digitalRead(ctsGiovanni))
                this->pressing_giovanni = true;
            else
                this->pressing_giovanni = false;

            if (digitalRead(ctsBeatrice))
                this->pressing_beatrice = true;
            else
                this->pressing_beatrice = false;

            if (!this->pressing_giovanni || !this->pressing_beatrice)
                continue;

            // we need to adjust based on respond that we want
            delay(20);
        }

        if (this->pressing_giovanni && !this->cts_giovanni)
        {
            this->cts_giovanni = true;
#if ENABLE_DEBUG
            Serial.println("Giovanni TOUCHED");
#endif
        }
        else if (!this->pressing_giovanni && this->cts_giovanni)
        {
            this->cts_giovanni = false;
        }

        if (this->pressing_beatrice && !this->cts_beatrice)
        {
            this->cts_beatrice = true;
#if ENABLE_DEBUG
            Serial.println("Beatrice TOUCHED");
#endif
        }
        else if (!this->pressing_beatrice && this->cts_beatrice)
        {
            this->cts_beatrice = false;
        }

        yield();
    }

    // avoid warning
    return nullptr;
}

/* instantiate the class */
Cts cts;
