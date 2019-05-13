/*
 * webserver.cpp - Implement the web server
 *
 * Created by Davide `rainbow` Gerhard
 * 12 May 2019
 * For the license see LICENSE
 *
 */

#include "webserver.h"

/* instantiate web server on port 80 */
WiFiSpiServer web_server(WEB_SERVER_PORT);

WebServer::WebServer()
{
}

/* initialize the web server */
void WebServer::init()
{
    Serial.println("");
    Serial.print("Start Webserver on host: ");
    Serial.print(WiFiSpi.localIP());
    Serial.print(" and port: ");
    Serial.println(WEB_SERVER_PORT);
    Serial.println("");
    web_server.begin();
}

/* manage loop */
SchedulerTask WebServer::loop()
{
    while (1)
    {
        // listen for incoming clients
        WiFiSpiClient client = web_server.available();
        if (client)
        {
#if ENABLE_DEBUG
            Serial.println("new client");
#endif
            // an http request ends with a blank line
            bool currentLineIsBlank = true;
            while (client.connected())
            {
                if (client.available())
                {
                    char c = client.read();
#if ENABLE_DEBUG
                    Serial.write(c);
#endif
                    // if you've gotten to the end of the line (received a newline
                    // character) and the line is blank, the http request has ended,
                    // so you can send a reply
                    if (c == '\n' && currentLineIsBlank)
                    {
                        // send a standard http response header
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: close"); // the connection will be closed after completion of the response
                        client.println("Refresh: 5");        // refresh the page automatically every 5 sec
                        client.println();
                        client.println("<!DOCTYPE HTML>");
                        client.println("<html>");
                        // output the value of each analog input pin
                        for (int analogChannel = 0; analogChannel < 6; analogChannel++)
                        {
                            int sensorReading = analogRead(analogChannel);
                            client.print("analog input ");
                            client.print(analogChannel);
                            client.print(" is ");
                            client.print(sensorReading);
                            client.println("<br />");
                        }
                        client.println("</html>");
                        break;
                    }
                    if (c == '\n')
                    {
                        // you're starting a new line
                        currentLineIsBlank = true;
                    }
                    else if (c != '\r')
                    {
                        // you've gotten a character on the current line
                        currentLineIsBlank = false;
                    }
                }
            }
            // give the web browser time to receive the data
            delay(1);

            // close the connection:
            client.stop();
#if ENABLE_DEBUG
            Serial.println("client disonnected");
#endif
        }

        yield();
    }

    // avoid warning
    return nullptr;
}

/* instantiate the class */
WebServer webserver;
