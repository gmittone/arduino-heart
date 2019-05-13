/*
 * utility.cpp - Library to collects many useful functions
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#include "utility.h"

// get rtc object from main.cpp
extern DS1307 rtc;

void serialWelcome()
{
  Serial.println("                                          ");
  Serial.println("                                          ");
  Serial.println("          ** Arduino Heart **             ");
  Serial.println("                                          ");
  Serial.println("        @@@@@@           @@@@@@           ");
  Serial.println("      @@@@@@@@@@       @@@@@@@@@@         ");
  Serial.println("    @@@@@@@@@@@@@@   @@@@@@@@@@@@@@       ");
  Serial.println("  @@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@     ");
  Serial.println(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ");
  Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   ");
  Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   ");
  Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   ");
  Serial.println(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ");
  Serial.println("  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     ");
  Serial.println("   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      ");
  Serial.println("    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@       ");
  Serial.println("      @@@@@@@@@@@@@@@@@@@@@@@@@@@         ");
  Serial.println("        @@@@@@@@@@@@@@@@@@@@@@@           ");
  Serial.println("          @@@@@@@@@@@@@@@@@@@             ");
  Serial.println("            @@@@@@@@@@@@@@@               ");
  Serial.println("              @@@@@@@@@@@                 ");
  Serial.println("                @@@@@@@                   ");
  Serial.println("                  @@@                     ");
  Serial.println("                   @                      ");
  Serial.println("                                          ");
  Serial.println("                  by                      ");
  Serial.println("       Alessandro `alez` Mauro            ");
  Serial.println("                  &&                      ");
  Serial.println("       Davide `rainbow` Gerhard           ");
  Serial.println("                                          ");
  Serial.println("         Copyright @ 2018/2019            ");
  Serial.println("                                          ");
  Serial.println("                                          ");
  Serial.print("Firmware version: ");
  Serial.println(VERSION);
  Serial.print("Firmware compiled on: ");
  Serial.print(__DATE__);
  Serial.print(" - ");
  Serial.println(__TIME__);
  Serial.println();
  Serial.println();
}

void printWifiData() {
  // print your WiFi module's IP address:
  IPAddress ip = WiFiSpi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFiSpi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFiSpi.SSID());

  // print the MAC address of the router you're attached to:
  byte *bssid = WiFiSpi.BSSID();
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFiSpi.RSSI();
  Serial.print("signal strength (RSSI): ");
  Serial.println(rssi);
}

uint8_t StringToUint8(const char* pString)
{
    uint8_t value = 0;

    // skip leading 0 and spaces
    while ('0' == *pString || *pString == ' ')
    {
        pString++;
    }

    // calculate number until we hit non-numeral char
    while ('0' <= *pString && *pString <= '9')
    {
        value *= 10;
        value += *pString - '0';
        pString++;
    }
    return value;
}

void setRtcFromCopiledDate()
{
  uint16_t year;
  uint8_t month, day, hour, minute, second;

    // sample input: date = "Dec 06 2009", time = "12:34:56"
    year = StringToUint8(__DATE__ + 9) + 2000;
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (__DATE__[0])
    {
    case 'J':
        if ( __DATE__[1] == 'a' )
            month = 1;
        else if ( __DATE__[2] == 'n' )
            month = 6;
        else
            month = 7;
        break;
    case 'F':
        month = 2;
        break;
    case 'A':
        month = __DATE__[1] == 'p' ? 4 : 8;
        break;
    case 'M':
        month = __DATE__[2] == 'r' ? 3 : 5;
        break;
    case 'S':
        month = 9;
        break;
    case 'O':
        month = 10;
        break;
    case 'N':
        month = 11;
        break;
    case 'D':
        month = 12;
        break;
    }
    day = StringToUint8(__DATE__ + 4);
    hour = StringToUint8(__TIME__);
    minute = StringToUint8(__TIME__ + 3);
    second = StringToUint8(__TIME__ + 6);

rtc.setDate(day, month, year);
rtc.setTime(hour, minute, second);
}
