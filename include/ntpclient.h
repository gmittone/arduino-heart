/*
 * ntpclient.h - Library to get time from an ntp server
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 * 
 * From https://github.com/arduino-libraries/NTPClient
 */

#ifndef _NTPClient_H
#define _NTPClient_H

#include <WiFiSpiUdp.h>

// local port to listen for UDP packets (maybe we can randomize)
#define NTP_DEFAULT_LOCAL_PORT 1337

// ntp packet size (bytes)
#define NTP_PACKET_SIZE 48

// set timeout for ntp packet answer (microseconds)
#define UDP_TIMEOUT 1000

// 70 years
#define SEVENZYYEARS 2208988800UL

class NTPClient
{
public:
    NTPClient(WiFiSpiUdp& udp);
    NTPClient(WiFiSpiUdp& udp, long timeOffset);
    NTPClient(WiFiSpiUdp& udp, const char* poolServerName);
    NTPClient(WiFiSpiUdp& udp, const char* poolServerName, long timeOffset);
    NTPClient(WiFiSpiUdp& udp, const char* poolServerName, long timeOffset, unsigned long updateInterval);

    /**
     * Set time server name
     *
     * @param poolServerName
     */
    void setPoolServerName(const char* poolServerName);

    /**
     * Starts the underlying UDP client with the default local port
     */
    void begin();

    /**
     * Starts the underlying UDP client with the specified local port
     */
    void begin(int port);

    /**
     * This should be called in the main loop of your application. By default an update from the NTP Server is only
     * made every 60 seconds. This can be configured in the NTPClient constructor.
     *
     * @return true on success, false on failure
     */
    bool update();

    /**
     * This will force the update from the NTP Server.
     *
     * @return true on success, false on failure
     */
    bool forceUpdate();

    int getDay() const;
    int getHours() const;
    int getMinutes() const;
    int getSeconds() const;

    /**
     * Changes the time offset. Useful for changing timezones dynamically
     */
    void setTimeOffset(int timeOffset);

    /**
     * Set the update interval to another frequency. E.g. useful when the
     * timeOffset should not be set in the constructor
     */
    void setUpdateInterval(unsigned long updateInterval);

    /**
     * @return time formatted like `hh:mm:ss`
     */
    String getFormattedTime() const;

    /**
     * @return time in seconds since Jan. 1, 1970
     */
    unsigned long getEpochTime() const;

    /**
     * Stops the underlying UDP client
     */
    void end();

private:
    WiFiSpiUdp*   _udp;
    bool          _udpSetup       = false;

    const char*   _poolServerName = "pool.ntp.org"; // Default time server
    int           _port           = NTP_DEFAULT_LOCAL_PORT;
    long          _timeOffset     = 0;

    unsigned long _updateInterval = 60000;  // In ms

    unsigned long _currentEpoc    = 0;      // In s
    unsigned long _lastUpdate     = 0;      // In ms

    byte          _packetBuffer[NTP_PACKET_SIZE];

    // send ntp packets
    void          sendNTPPacket();

    // buffer to hold incoming and outgoing packets
    byte packetBuffer[NTP_PACKET_SIZE];
};

#endif // _NTPClient_H