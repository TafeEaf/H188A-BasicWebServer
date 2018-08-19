#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define ssid      "WemosWifi"   // WiFi SSID
#define password  "12345678"    // WiFi password
#define LEDPIN1    D3            // Led Pin
#define LEDPIN2    D4            // Led Pin

String  LedStatus1 = "OFF";
String  LedStatus2 = "OFF";

// create Objects
ESP8266WebServer server ( 80 );

String getPage(){
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>ESP8266</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>Wemos Wifi Basic Server</h1>";
  page += "<h3>Remote GPIO configuration</h3>";
  page += "<form action='/' method='POST'>";
  page += "<ul><li>Green Led (Current status: ";
  page += LedStatus1;
  page += ")";
  page += "<INPUT type='radio' name='LED1' value='0'>ON";
  page += "<INPUT type='radio' name='LED1' value='1'>OFF</li></ul>";
  page += "<ul><li>Red Led (Current status: ";
  page += LedStatus2;
  page += ")";
  page += "<INPUT type='radio' name='LED2' value='0'>ON";
  page += "<INPUT type='radio' name='LED2' value='1'>OFF</li></ul>";
  page += "<INPUT type='submit' value='Send'>";
  page += "</body></html>";
  return page;
}


void handleSubmit() {
  // Update GPIO
  String LEDValue1,LEDValue2;
  LEDValue1 = server.arg("LED1");
  LEDValue2 = server.arg("LED2");

  if ( LEDValue1 == "0" ) {
    digitalWrite(LEDPIN1, 0);
    LedStatus1 = "On";
  } else if ( LEDValue1 == "1" ) {
    digitalWrite(LEDPIN1, 1);
    LedStatus1 = "Off";
  } else {
    Serial.println("Err Green Led");
  }
  Serial.print("Set Green Led "); Serial.println(!digitalRead(LEDPIN1));

  if ( LEDValue2 == "0" ) {
    digitalWrite(LEDPIN2, 0);
    LedStatus2 = "On";
  } else if ( LEDValue2 == "1" ) {
    digitalWrite(LEDPIN2, 1);
    LedStatus2 = "Off";
  } else {
    Serial.println("Err Red Led");
  }
  Serial.print("Set Red Led "); Serial.println(!digitalRead(LEDPIN2));

  server.send ( 200, "text/html", getPage() );
}

void handleRoot(){
  if ( server.hasArg("LED1") || server.hasArg("LED2")) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }
}

void setup() {
  pinMode(LEDPIN1, OUTPUT);           // sets the digital pin D3 as output
  pinMode(LEDPIN2, OUTPUT);           // sets the digital pin D4 as output
  digitalWrite(LEDPIN1, 1);           // sets the digital pin D3 low
  digitalWrite(LEDPIN2, 1);           // sets the digital pin D4 low

  Serial.begin ( 115200 );

  WiFi.begin ( ssid, password );
  // Attente de la connexion au réseau WiFi / Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }

  // WiFi connexion is OK
  Serial.println ( "" );
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  // link to the function that manage launch page
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop() {
  server.handleClient();
  delay(1000);
}
