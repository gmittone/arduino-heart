/*
 * ntpclient.h - Library to get time from an ntp server
 * Created by Davide `rainbow` Gerhard and Alessandro `alez` Mauro
 * 12 May 2019
 * For the license see LICENSE
 */

#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include "WiFiSpiUdp.h"

// we use the best ntp pool ;-)
#define NTP_POOL_SERVER "pool.ntp.org"

// local port to listen for UDP packets (maybe we can randomize)
#define NTP_LOCAL_UDP_PORT 12123

class NTPClient
{
public:
    NTPClient();

private:
    void SendNTPPacket(const char *ntpSrv);
    // A UDP instance to let us send and receive packets over UDP
    WiFiSpiUdp Udp;
    // NTP timestamp is in the first 48 bytes of the message
    static const int NTP_PACKET_SIZE = 48;
    // buffer to hold incoming and outgoing packets
    byte packetBuffer[NTP_PACKET_SIZE];
    // timeout in miliseconds to wait for an UDP packet to arrive
    static const int UDP_TIMEOUT = 2000;
};

#endif