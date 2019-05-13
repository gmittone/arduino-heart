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
}

/* initialize the class */
void Wifi::init()
{
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
    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(WIFI_SSID);
        // Connect to WPA/WPA2 network:
        status = WiFiSpi.begin(WIFI_SSID, WIFI_PASS);

        // wait connection
        delay(WAIT_CONNECTION);
    }

    // you're connected now, so print out the data:
    Serial.print("You're connected to the network");
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

/* instantiate the class */
Wifi wifi;