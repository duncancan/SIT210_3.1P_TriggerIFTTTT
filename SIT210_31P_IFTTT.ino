#include <WiFiNINA.h>
#include "secrets.h"

// Set up our network connection
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
WiFiClient client;

// Set up our IFTTT connection
String myKey = SECRET_IFTTT_KEY;
char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME = "/trigger/light_level_changed/with/key/" + myKey;
String queryStringBoilerplate = "?value1=";

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
    Serial.println("\nConnected.");
  }

  // connect to web server on port 80:
  if (client.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
  }
  else {// if not connected:
    Serial.println("connection failed");
  }

  // Initialise serial port
  Serial.begin(9600);
  while (!Serial);

  // Initialise light sensor                                                                            // TO DO
}

void loop() {
  // Read our light sensor                                                                              // TO DO
  String lightStatus = "shade";

  // Check if read failed and exit early (to try again).                                                // TO DO
  if (0) {
    Serial.println(F("Failed to read from sensor!"));
    delay(1000); // Wait 1 second to check again
    return;
  }

  // Send to our IFTTT channel
  // make a HTTP request:
  // send HTTP header
  client.println("GET " + PATH_NAME + queryStringBoilerplate + lightStatus + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println(); // end HTTP header

  // And wait a bit
  delay(20000);

}