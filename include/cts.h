/*
 * cts.h - Library to manage the pads like, giovanni/beatrice signature
 *         cts = Capactive Touch Sensor
 *
 * chip: TTP223 https://infusionsystems.com/support/TTP223.pdf
 *
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _CTS_H
#define _CTS_H

#include <Arduino.h>
#include <Scheduler.h>
#include "config.h"
#include "display.h"

/*
 * Capacitive touch sensor
 * we use the same nomenclature of hardware/heart_chema.ps
 * and the pinout is relative to Arduino DUE
 * TRIG_G for Giovanni sign = ADCH_7 = AD6
 * TRIG_B for Beatrice sign = ADCL_7 = CANRX0
 */
#define ctsGiovanni 68
#define ctsBeatrice 60

/*
 * how many samples needed to activate the button
 */
#define SAMPLES_FILTER 3

/*
 * show message per N cycle
 * ATTENTION: this value is dependend on the loop delay()
 */
#define MARIAGE_SHOW_TIME 500000

class Cts
{
public:
    Cts();
    void init();
    void loop();

private:
    // pad status
    bool cts_giovanni;
    bool cts_beatrice;

    // low pass filter variables
    uint8_t samples;
    bool pressing_giovanni;
    bool pressing_beatrice;

    // count the time to display the mariage message
    // -1  disabled
    // 0   remove
    // N   count-down
    int32_t mariage_message_time;

};

extern Cts cts;

#endif // _CTS_H
