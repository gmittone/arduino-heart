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
    Serial.println("\nInitializing capacitive sensors (pads)...");

    // configure port mode
    pinMode(ctsGiovanni, INPUT);
    pinMode(ctsBeatrice, INPUT);

    this->cts_giovanni = false;
    this->cts_beatrice = false;

    this->mariage_message_time = -1;
}

/* manage loop */
void Cts::loop()
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
#if ENABLE_CTS_DEBUG
        Serial.println("Pad Giovanni touched");
#endif

        // action based on window (display) status
        switch (display.window_status)
        {
        case clock:
            // print the mariage value
            // we always print without checking the mariage status
            // in this way we can pass between cts
            if (this->mariage_message_time > -1 && (display.mariage_seconds_enabled || display.mariage_days_enabled))
            {
                display.clearMariage();
            }
            else
            {
                display.printMariageMessage();
            }
            display.printMariageSeconds(localTime.getTime());
            this->mariage_message_time = MARIAGE_SHOW_TIME;
            break;
        default:
            break;
        }
    }
    else if (!this->pressing_giovanni && this->cts_giovanni)
    {
        this->cts_giovanni = false;
    }

    if (this->pressing_beatrice && !this->cts_beatrice)
    {
        this->cts_beatrice = true;
#if ENABLE_CTS_DEBUG
        Serial.println("Pad Beatrice touched");
#endif

        // action based on window (display) status
        switch (display.window_status)
        {
        case clock:
            if (this->mariage_message_time > -1 && (display.mariage_seconds_enabled || display.mariage_days_enabled))
            {
                display.clearMariage();
            }
            else
            {
                display.printMariageMessage();
            }
            display.printMariageDays(localTime.getTime());
            this->mariage_message_time = MARIAGE_SHOW_TIME;
            break;
        default:
            break;
        }
    }
    else if (!this->pressing_beatrice && this->cts_beatrice)
    {
        this->cts_beatrice = false;
    }

    if (this->mariage_message_time != -1)
    {
        // decrement mariate counter
        if (this->mariage_message_time > 0)
        {
            this->mariage_message_time--;
        }
        else
        {
            this->mariage_message_time = -1;
            // I prefer to redraw the window to
            // be sure every hand is right
            display.drawClock();
        }
    }
}

/* instantiate the class */
Cts cts;
