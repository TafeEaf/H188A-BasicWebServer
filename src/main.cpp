#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <SSD1306.h>

// Initialize the webserver
#define ssid      "WemosWifi"     // WiFi SSID
#define password  "12345678"      // WiFi password
ESP8266WebServer server ( 80 );   // create Objects

// Initialize the LED output and associated webpage colors
#define GreenLed          D6          // Led Pin
#define RedLed            D7          // Led Pin
#define BlueLed           D8          // Led Pin
// Initialize the associated LED webpage font colors
#define  GreenLedOn   "<font color=\"#7fbf7f\"><b>ON</b></font>"
#define  GreenLedOff  "<font color=\"#006600\"><b>OFF</b></font>"
#define  RedLedOn     "<font color=\"#ff7f7f\"><b>ON</b></font>"
#define  RedLedOff    "<font color=\"#cc0000\"><b>OFF</b></font>"
#define  BlueLedOn    "<font color=\"#7f7fff\"><b>ON</b></font>"
#define  BlueLedOff   "<font color=\"#0000cc\"><b>OFF</b></font>"

// Initialize the DHT11 Temperature sensor
#define DHTPIN            D4         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT11      // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// Initialize the OLED display using i2c
#define SCL_PIN	          D1         // SCL-PIN
#define SDA_PIN	          D2         // SDA-PIN
SSD1306  display(0x3c, SDA_PIN, SCL_PIN);

String getPage(){
  String checkedOn, checkedOff;
  String GreenLedStatus, RedLedStatus, BlueLedStatus;

  float humidity = dht.readHumidity();        // Read humidity as percentual
  float temperature = dht.readTemperature();  // Read temperature as Celsius (the default)

  display.clear();      // Clear the Oled display associated memory
  display.display();    // Print an empty screen

  //                                           Automatic refresh every 30 seconds
  String page = "<html lang=en-EN><head><meta http-equiv='refresh' content='30'/>";
  page += "<title>ESP8266</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>Wemos Wifi Basic Server</h1>";
  page += "<h3>Remote GPIO configuration</h3>";
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("DHT sensor error");
  }
  else {
    // Send temperature and humidity to Serial
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C ");

    // Send temperature and humidity to Oled Screen
    //                 x  y  String
    display.drawString(0, 0, "Temp:");
    display.drawString(96, 0, String(temperature));
    display.drawString(0, 16, "Humidity:");
    display.drawString(96, 16, String(humidity));

    // Send temperature and humidity to Web Page
    page += "<p>Temperature ";
    page += temperature; page += " </p>";
    page += "<p>Humidity ";
    page += humidity; page += " </p>";

  }

  page += "<form action='/' method='POST'>";

  // handle Green LED
  display.drawString(0, 32, "G:");
  if (digitalRead(GreenLed)==0){
    GreenLedStatus = GreenLedOn; checkedOn = "checked"; checkedOff = "";
    display.drawString(28, 32, "On");
  }
  else{
    GreenLedStatus = GreenLedOff; checkedOn = ""; checkedOff = "checked";
    display.drawString(28, 32, "Off");
  }
  page += "<ul><li>Green Led Current status: ";
  page += GreenLedStatus;
  page += "<INPUT type='radio' name='LED1' value='0'"; page += checkedOn;  page += ">ON";
  page += "<INPUT type='radio' name='LED1' value='1'"; page += checkedOff;  page += ">OFF</li></ul>";

  // handle Red LED
  display.drawString(64, 32, "R:");
  if (digitalRead(RedLed)==0){
     RedLedStatus = RedLedOn; checkedOn = "checked"; checkedOff = "";
     display.drawString(96, 32, "On");
  }
  else{
     RedLedStatus = RedLedOff; checkedOn = ""; checkedOff = "checked";
     display.drawString(96, 32, "Off");
  }
  page += "<ul><li>Red   Led Current status: ";
  page += RedLedStatus;
  page += "<INPUT type='radio' name='LED2' value='0'"; page += checkedOn;  page += ">ON";
  page += "<INPUT type='radio' name='LED2' value='1'"; page += checkedOff;  page += ">OFF</li></ul>";

  // handle Blue LED
  display.drawString(0, 48, "B:");
  if (digitalRead(BlueLed)==0){
    BlueLedStatus = BlueLedOn; checkedOn = "checked"; checkedOff = "";
    display.drawString(28, 48, "On");
  }
  else{
    BlueLedStatus = BlueLedOff; checkedOn = ""; checkedOff = "checked";
    display.drawString(28, 48, "Off");
  }
  page += "<ul><li>Blue  Led Current status: ";
  page += BlueLedStatus;
  page += "<INPUT type='radio' name='LED3' value='0'"; page += checkedOn;  page += ">ON";
  page += "<INPUT type='radio' name='LED3' value='1'"; page += checkedOff;  page += ">OFF</li></ul>";

  // send form
  page += "<INPUT type='submit' value='Send'>";
  page += "</body></html>";

  
  display.display();   //  Visualise the new constructed Oled screen
  return page;
}


void handleSubmit() {
  // update Green LED
  Serial.print("Set Green Led: ");
  if ( server.arg("LED1") == "0" )
    digitalWrite(GreenLed, 0);
  else
    digitalWrite(GreenLed, 1);
  Serial.println(!digitalRead(GreenLed));

  // update RedLed LED
  Serial.print("Set Red Led: ");
  if ( server.arg("LED2") == "0" )
    digitalWrite(RedLed, 0);
  else
    digitalWrite(RedLed, 1);
  Serial.println(!digitalRead(RedLed));

  // update Blue LED
  Serial.print("Set Blue Led: ");
  if ( server.arg("LED3") == "0" )
    digitalWrite(BlueLed, 0);
  else
    digitalWrite(BlueLed, 1);
  Serial.println(!digitalRead(BlueLed));

  //After changes resend the recompiled web page
  server.send ( 200, "text/html", getPage() );
}

void handleRoot(){
  //               Green Led                  Red Led                  Blue Led
  if ( server.hasArg("LED1") || server.hasArg("LED2") || server.hasArg("LED3")) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }
}

void setup() {
  pinMode(GreenLed, OUTPUT);            // sets the digital pin D6 as output
  digitalWrite(GreenLed, 1);            // sets the digital pin D6 high

  pinMode(RedLed, OUTPUT);              // sets the digital pin D7 as output
  digitalWrite(RedLed, 1);              // sets the digital pin D7 high

  pinMode(BlueLed, OUTPUT);             // sets the digital pin D8 as output
  digitalWrite(BlueLed, 1);             // sets the digital pin D8 high

  Serial.begin ( 115200 );              // init serial communication

  WiFi.begin ( ssid, password );
  while ( WiFi.status() != WL_CONNECTED ) {     // Wait for connection
    delay ( 500 ); Serial.print ( "." );
  }

  // WiFi connection is OK
  Serial.println ( "" );
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  // Link to the function that manage launch page
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );

  // Initialising the Oled screen
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  Serial.println("Oled Screen initialized");

  // Initialising the temperatuse sensor
  dht.begin();
  Serial.println("Temperature sensor initialized");
}

void loop() {
  server.handleClient();
  delay(1000);
}
