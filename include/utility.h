/*
 * utility.h - Library to collects many useful functions
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef _UTILITY_H
#define _UTILITY_H

#include <Arduino.h>
#include <WiFiSpi.h>
#include <Wire.h>
#include <DS1307.h>
#include "version.h"

// welcome to serial
void serialWelcome();

// print information about wifi status
void printWifiData();
void printCurrentNet();

#endif // _UTILITY_H
