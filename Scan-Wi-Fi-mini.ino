#include "ESP8266WiFi.h"

void setup() { 
  Serial.begin(115200);
  Serial.println("\nNetwork Scan");
 
  int numberOfNetworks = WiFi.scanNetworks();
 
  for(int i =0; i<numberOfNetworks; i++){
      Serial.print(WiFi.SSID(i));      Serial.print("\t");
      Serial.print(WiFi.channel(i));      Serial.print("\t");
      Serial.print(WiFi.encryptionType(i));      Serial.print("\t");
      
      Serial.println(WiFi.RSSI(i));
 
      //Serial.print("Network name: ");
      //Serial.println(WiFi.SSID(i));
      //Serial.print("Signal strength: ");
      //Serial.println(WiFi.RSSI(i));
      //Serial.println("-----------------------");
 
  }
 
}
 
void loop() {}
