#include <WiFiNINA.h>
#include "secrets.h"
#include <BH1750.h>
#include <Wire.h>

// Variables for our network connection
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
WiFiClient client;

// Variables for our IFTTT connection
String myKey = SECRET_IFTTT_KEY;
char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME = "/trigger/light_level_changed/with/key/" + myKey;
String queryStringBoilerplate = "?value1=";

// Variables for our light sensor
BH1750 lightMeter;
float lux;
String currentLightCondition = "";
String priorLightCondition = "";

void setup() {
  // Connect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected to WiFi.");
  }

  // Initialise serial port
  Serial.begin(9600);
  while (!Serial);

  // Initialise light sensor
  Wire.begin(); // Initialise I2C bus
  lightMeter.begin(); // And then sensor on that bus
}

void loop() {
  // Read our light sensor
  lux = lightMeter.readLightLevel();
  // Categorise as sunny or not
  if (lux > 500) {
    currentLightCondition = "sunny";
  } else {
    currentLightCondition = "shady";
  }

  // Output light conditions to our serial monitor for debugging purposes
  Serial.println();
  Serial.print("Prior light condition was: ");
  Serial.println(priorLightCondition);
  Serial.print("Light meter read luminocity in lux: ");
  Serial.println(lux);
  Serial.print("Current light condition is now ");
  Serial.println(currentLightCondition);

  // If our light condition has changed, send a HTTP request to IFTTT
  if (currentLightCondition != priorLightCondition) {
    // connect to web server on port 80:
    if (client.connect(HOST_NAME, 80)) {
      // if connected:
      Serial.println("Connected to IFTTT server.");
      Serial.println("Light condition has changed. Sending HTTP request to IFTTT");
      client.println("GET " + PATH_NAME + queryStringBoilerplate + currentLightCondition + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: close");
      client.println(); // end HTTP header
      Serial.println("Done");
    }
    else {// if not connected:
      Serial.println("connection failed. Failed to trigger IFTTT.");
    }
  }

  // Update prior light condition to check against next time
  priorLightCondition = currentLightCondition;

  // And wait a bit before checking again
  delay(20000);

}