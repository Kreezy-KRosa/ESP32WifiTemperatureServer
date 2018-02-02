// ESP32 Wifi Temperature Server with Remote Client via Web Browser
// By Kreezy(KRosa) 1.30.2018

// This code is public domain 


#include "DHTesp.h"                 // Install these libraries if you don't already have them
#include "ESP32Ticker.h"
#include <WiFi.h>

DHTesp dht;               

int dhtPin = 4;                        // DHT-11 sensor data wire to Pin D4



const char* ssid = "SSID";             // enter your wifi's ssid name inside the ""'s // EXAMPLE "MyWifi"

const char* password = "PASSWORD";     // enter your wifi's password inside the ""'s  // Example "Password"

int ledPin = 2;                        // declaring the onboard LED on Pin D2
int clientLed = 5;                     // declaring the external LED on Pin D5

WiFiServer server(80);                // wifi server setup on port 80

void setup()
{ 
  Serial.begin(9600);
  dht.setup(dhtPin, DHTesp::DHT11);  // for DHTesp:: enter sensor type (DHT11, DHT22, etc)
  
  pinMode(ledPin,OUTPUT);            // making the onboard LED pin an output
  pinMode(clientLed,OUTPUT);         // making the external LED pin an output 
  
  digitalWrite(clientLed,LOW);

  WiFi.begin(ssid,password);                   // this is when we try to connect to your network
  while ((!(WiFi.status() == WL_CONNECTED))){
  delay(300);
  digitalWrite(ledPin,HIGH);                   // this makes the onboard LED blink until connected
  delay(50);
  digitalWrite(ledPin,LOW);
  delay(50);
  }
  Serial.println("Connected");                 // once connected to a network, the onboard LED will stay on
  Serial.println((WiFi.localIP()));
  server.begin();
  digitalWrite(ledPin,HIGH);
  }

void loop() {
  
  WiFiClient client = server.available();               // if your connected device is available this will happen
    if (!client) { return; }
    while(!client.available()){  delay(1); }
    float newTempValue = dht.getTemperature();          // reads the sensor
    int fahrenValue = (newTempValue * 1.8 + 32);        // converts the reading to Degrees Farhenheit
    digitalWrite(clientLed,HIGH);                       // external LED will light when this info is being served to client
    Serial.println("Client Connected");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");   
    client.print("Temperature: ");                       // this is where the text on the webpage served is created
    client.print(fahrenValue);                           // this variable will hold the temperature data
    client.print(" Degrees Fahrenheit");
    client.println("</html>");
    client.println("<meta http-equiv=\"refresh\" content=\"5\">");  // this line will refresh the webpage every 5 seconds
    client.stop();                                     // to change the refresh rate, change the 5 to any desired number of seconds 
    digitalWrite(clientLed,LOW);                                    // turns the external led off when finished serving client
    Serial.println("Client Disconnected");                          // prints this line in the serial monitor for debugging
}  
