/*
 * wifi.h - implement the management of the wifi device
 *
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 */

#include "wifi.h"

Wifi::Wifi()
{
    this->ready = false;
    this->status = WL_IDLE_STATUS;

    // enable serial debug from esp8266
    Serial1.begin(ESP8266_SERIAL_SPEED);
    Serial1.setTimeout(100);
}

/* initialize the class */
void Wifi::init()
{
    // enable the wifi board
    pinMode(ESP8266_ENABLE_PIN, OUTPUT);
    digitalWrite(ESP8266_ENABLE_PIN, 1);

    // Initialize the WifiSpi library
    WiFiSpi.init();

#if ENABLE_DEBUG
    // WiFiSpi versions
    Serial.print("WiFiSpi ESP8266 firmware version: ");
    Serial.println(WiFiSpi.firmwareVersion());
    Serial.print("WiFiSpi ESP8266 protocol version: ");
    Serial.println(WiFiSpi.protocolVersion());
    Serial.print("WiFiSpi master protocol version: ");
    Serial.println(WiFiSpi.masterProtocolVersion());
#endif

    // check for the presence of the ESP module:
    if (WiFiSpi.status() == WL_NO_SHIELD)
    {
        Serial.println("WiFi module not present");
        
        // continue the execution without network
        return;
    }

    if (!WiFiSpi.checkProtocolVersion())
    {
        Serial.println("Protocol version mismatch. Please upgrade the firmware");
        return;
    }

    // attempt to connect to Wifi network:
    uint8_t retry = WIFI_RETRY;
    while (status != WL_CONNECTED && retry > 0)
    {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(WIFI_SSID);
        
        // Connect to WPA/WPA2 network:
        status = WiFiSpi.begin(WIFI_SSID, WIFI_PASS);

        // decrement retry
        retry--;

        // wait connection
        delay(WIFI_WAIT_CONNECTION);
    }

    // you're connected now, so print out the data:
    Serial.println("You're connected to the network");
    printCurrentNet();
    printWifiData();
    Serial.println("");

    this->ready = true;
}

/*
 * return if the wifi is in good shape
 * useful to enable other functions like, ntp
 */
bool Wifi::isReady()
{
    return this->ready;
}

void Wifi::readSerial()
{
    if (Serial1.available()) {
        // TODO analyze the following problem
        // use Serial1.setTimeout() so probably not good for Scheduling
        Serial.println("ESP8266: " + Serial1.readString());
    }
}

/* instantiate the class */
Wifi wifi;