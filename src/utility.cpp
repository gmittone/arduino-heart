/*
 * utility.cpp - Library to collects many useful functions
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 */


#include <Arduino.h>
#include <WiFiSpi.h>
#include <Wire.h>
#include <RtcDS1307.h>

#include "utility.h"

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
  Serial.println("          Copyright @ 2018                ");
  Serial.println("                                          ");
  Serial.println("                                          ");
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
