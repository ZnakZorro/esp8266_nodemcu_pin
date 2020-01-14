#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

void setup()
{
  Serial.begin(115200);
  delay(10);
  //connect to a WiFi network
  WiFi.mode(WIFI_STA);
  
  WiFiMulti.addAP("webWI", "wi77tam77");
  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");
  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print("*");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);
}


void loop()
{
}
