/***************************************************************************************
//
//                         L o L i n   3 2   A c c e s s   P o i n t      
//
//                                                                      қuran aug 2023
***************************************************************************************/
#include <Arduino.h>
#include <WiFi.h>

#define LED                            5
const char* ssid = "LoLin32AP";
const char* pw = "123456789";
WiFiServer serverWiFi(80);
IPAddress lclIP(192,168,2,204);
IPAddress gateway(192,168,2,1);
IPAddress subnet(255,255,255,0);


String header;

void wifiSend(WiFiClient client);
void wifiReceive(WiFiClient client);


String ledStatus = "off";

void setup() 
{
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);  // Led on = LOW
    Serial.println("start: LoLin 32 Access Point! now !");
    WiFi.softAP(ssid, pw, 1, 0, 8);
    delay(1000);
    Serial.println("Zeile 75");
    //IPAddress apIP = WiFi.softAP();

    WiFi.softAPConfig(lclIP, gateway, subnet);
    digitalWrite(LED, HIGH);
    Serial.print("AP IP-Adresse: ");
    Serial.println(lclIP);
    serverWiFi.begin();
    Serial.println("Server startet");
  
}

void loop() 
{
     WiFiClient client = serverWiFi.available();
     if (client)
     {
        wifiReceive(client);
     }
     delay(1);
}

void wifiSend(WiFiClient client)
{
    int sec = (millis()/1000)%60;
    int min = (millis()/(1000*60))%60;
    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.print("<!DOCTYPE HTML>");
    client.print("  <head>");
    client.print("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.print("    <meta http-equiv=\"refresh\" content=\"5\">");
    client.print("    <link rel=\"icon\" href=\"data:,\">");
    client.print("    <title> HTL St.Poelten</title>");
        
    client.print("    <style>html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}");
    client.print("    .buttonON  { background-color: #ff0000; border: none; color: white; padding: 16px 40px; border-radius: 4px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    client.print("    .buttonOFF { background-color: #00ff00;}");
    client.print("    </style>");
    client.print("  </head>");
    client.print("  <body>");
    client.print("  <div style=\"font-size: 1.0rem;\">");
    client.print("    <h1> LoLin 32 AP </h1>");
    client.print("  </div>");
    client.print("  <div style=\"font-size: 2.0rem;\">");
    client.print("    <p>   time: " + String(min) + ":" + String(sec) + "<br></p>");
    client.print("    <p> LED-STATUS:"  + ledStatus + "</p>");

    if (ledStatus == "off")
    {
        client.print("<p> <a href=\"/L\"><button class=\"buttonON\"> ein </button></a></p>");
    }
    else
    {
        client.print("<p> <a href=\"/H\"><button class=\"buttonOFF\"> aus </button></a></p>");
    }
    client.print("  </div>");
    client.print("  </body>");
    client.print("</html>");
    client.println();
}

void wifiReceive(WiFiClient client)
{
    Serial.println("neue Anfrage.");
    String currentLine= "";
    while (client.connected())
    {
        if (client.available())
        {
            char c = client.read();
            Serial.print(c);
            if (c == '\n')
            {
                if (currentLine.length() == 0)
                {
                    wifiSend(client);
                    break;
                }
                else
                {
                    currentLine = "";
                }
            }
            else if (c != '\r')
            {
                currentLine += c;
            }

            if (currentLine.endsWith("GET /H"))
            {
                digitalWrite(LED, HIGH);
                ledStatus = "off";
            }

            if (currentLine.endsWith("GET /L"))
            {
                digitalWrite(LED, LOW);
                ledStatus = "on";

            }

        }
    }
    client.stop();
    Serial.println("Client Dissconnected.");
    Serial.println();
}
