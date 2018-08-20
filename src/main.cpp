#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define ssid      "WemosWifi"     // WiFi SSID
#define password  "12345678"      // WiFi password
#define GreenLed    D2            // Led Pin
#define RedLed      D3            // Led Pin
#define BlueLed     D4            // Led Pin

#define  GreenLedOn   "<font color=\"#7fbf7f\"><b>ON</b></font>"
#define  GreenLedOff  "<font color=\"#006600\"><b>OFF</b></font>"
#define  RedLedOn     "<font color=\"#ff7f7f\"><b>ON</b></font>"
#define  RedLedOff    "<font color=\"#cc0000\"><b>OFF</b></font>"
#define  BlueLedOn    "<font color=\"#7f7fff\"><b>ON</b></font>"
#define  BlueLedOff   "<font color=\"#0000cc\"><b>OFF</b></font>"

String GreenLedStatus, RedLedStatus, BlueLedStatus;

// create Objects
ESP8266WebServer server ( 80 );

String getPage(){
  String page = "<html lang=en-EN><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>ESP8266</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>Wemos Wifi Basic Server</h1>";
  page += "<h3>Remote GPIO configuration</h3>";
  page += "<form action='/' method='POST'>";
  // handle Green LED
  page += "<ul><li>Green Led (Current status: ";
  page += GreenLedStatus;
  page += ")";
  page += "<INPUT type='radio' name='LED1' value='0'>ON";
  page += "<INPUT type='radio' name='LED1' value='1'>OFF</li></ul>";
  // handle Red LED
  page += "<ul><li>Red Led (Current status: ";
  page += RedLedStatus;
  page += ")";
  page += "<INPUT type='radio' name='LED2' value='0'>ON";
  page += "<INPUT type='radio' name='LED2' value='1'>OFF</li></ul>";
  // handle Blue LED
  page += "<ul><li>Red Led (Current status: ";
  page += BlueLedStatus;
  page += ")";
  page += "<INPUT type='radio' name='LED3' value='0'>ON";
  page += "<INPUT type='radio' name='LED3' value='1'>OFF</li></ul>";
  // send form
  page += "<INPUT type='submit' value='Send'>";
  page += "</body></html>";
  return page;
}


void handleSubmit() {
  String LED_1_Value = server.arg("LED1");
  String LED_2_Value = server.arg("LED2");
  String LED_3_Value = server.arg("LED3");
  // update Green LED
  Serial.print("Set Green Led: ");
  if ( LED_1_Value == "0" ) {
    digitalWrite(GreenLed, 0);
    GreenLedStatus = GreenLedOn;
  } else if ( LED_1_Value == "1" ) {
    digitalWrite(GreenLed, 1);
    GreenLedStatus = GreenLedOff;
  } else {
    Serial.print("No parameter sent, using previous: ");
  }
  Serial.println(!digitalRead(GreenLed));
  // update RedLed LED
  Serial.print("Set Red Led: ");
  if ( LED_2_Value == "0" ) {
    digitalWrite(RedLed, 0);
    RedLedStatus = RedLedOn;
  } else if ( LED_2_Value == "1" ) {
    digitalWrite(RedLed, 1);
    RedLedStatus = RedLedOff;
  } else {
    Serial.print("No parameter sent, using previous: ");
  }
  Serial.println(!digitalRead(RedLed));
  // update Blue LED
  Serial.print("Set Blue Led: ");
  if ( LED_3_Value == "0" ) {
    digitalWrite(BlueLed, 0);
    BlueLedStatus = BlueLedOn;
  } else if ( LED_3_Value == "1" ) {
    digitalWrite(BlueLed, 1);
    BlueLedStatus = BlueLedOff;
  } else {
    Serial.print("No parameter sent, using previous: ");
  }
  Serial.println(!digitalRead(BlueLed));

  server.send ( 200, "text/html", getPage() );
}

void handleRoot(){
  if ( server.hasArg("LED1") || server.hasArg("LED2") || server.hasArg("LED3")) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }
}

void setup() {
  pinMode(GreenLed, OUTPUT);            // sets the digital pin D2 as output
  pinMode(RedLed, OUTPUT);              // sets the digital pin D3 as output
  pinMode(BlueLed, OUTPUT);             // sets the digital pin D4 as output
  digitalWrite(GreenLed, 1);            // sets the digital pin D2 high
  digitalWrite(RedLed, 1);              // sets the digital pin D3 high
  digitalWrite(BlueLed, 1);             // sets the digital pin D4 high
  GreenLedStatus  =   GreenLedOff;
  RedLedStatus    =   RedLedOff;
  BlueLedStatus   =   BlueLedOff;

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
