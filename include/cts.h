/*
 * cts.h - Library to manage the pads like, giovanni/beatrice signature
 *         cts = Capactive Touch Sensor
 *
 * chip: TTP223 https://infusionsystems.com/support/TTP223.pdf
 *
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _CTS_H
#define _CTS_H

#include <Arduino.h>
#include <Scheduler.h>
#include "config.h"

/*
 * Capacitive touch sensor
 * we use the same nomenclature of hardware/heart_chema.ps
 * and the pinout is relative to Arduino DUE
 * TRIG_G for Giovanni sign = ADCH_7 = AD6
 * TRIG_B for Beatrice sign = ADCL_7 = CANRX0
 */
#define ctsGiovanni 68
#define ctsBeatrice 60

class Cts
{
public:
    Cts();
    void init();
    SchedulerTask loop();

private:
    // pad status
    bool cts_giovanni;
    bool cts_beatrice;
};

extern Cts cts;

#endif // _CTS_H