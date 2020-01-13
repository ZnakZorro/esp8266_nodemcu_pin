
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <ArduinoJson.h>    //https://github.com/bblanchon/ArduinoJson

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);
char buf1[16];
char buf2[16];
char buf3[16];
char *text = "V1=";
String t1="";
String t2="";
String txt="";
//String payload = "_______________";
  String payload = "???????????????";
char   buftt[16];
String tt="";
byte bajt = 0;
StaticJsonDocument<200> doc;
ESP8266WiFiMulti WiFiMulti;

void setup() {
  u8g2.begin();
  u8g2.setFontMode(0); // mode transparent
  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    t1 = "WiFi:";
    t1.concat(t);
    printOLED(t1);
    Serial.flush();
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
    //const char* ssid     = "webWI";
    //const char* password = "wi77tam77";   
  WiFiMulti.addAP("webWI", "wi77tam77");
}

void printOLED(String txt) {
  Serial.println(txt);
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_profont29_mf);
    txt.toCharArray(buftt, 16);
    u8g2.drawStr(0,32,buftt);
  } while (u8g2.nextPage());   
}

void olejOLED(){   
    /*
    String t1  = doc["t1"];  if(t1=="null") {t1="?";}
    String t2 = doc["t2"]; 
    String t3 = doc["t3"]; 
    String d1 = doc["d1"];  if(d1=="null") {d1="?";}
    String d2 = doc["d2"];
    String d3 = doc["d3"];
    */
    String s1 = doc["s1"]; if(s1=="null") {s1="B";}
    String s2 = doc["s2"]; if(s2=="null") {s2="D";}
    String s3 = doc["s3"]; if(s3=="null") {s3="E";}
    String s4 = doc["s4"]; if(s4=="null") {s4="D";}
    
    //Serial.print(t1); Serial.print("; ");    Serial.print(t2); Serial.print("; ");    Serial.print(d1); Serial.print("; ");    Serial.print(d2); Serial.print("; ");     Serial.print(s1); Serial.print("; ");    Serial.print(s2); Serial.print("; "); 

        byte moda = bajt % 4;
        bajt++;
        //Serial.print(bajt); Serial.print(" %= ");  Serial.println(moda);
            
        String symbolIcon = "";    
        String linijka    = "";   
        int txtLong       = 0; 
        u8g2.firstPage();
        do {
            if (moda==0) {
               linijka="-     ";
               symbolIcon = s1;
               String t  = doc["t1"];  if(t=="null") {t="?";}
               String d  = doc["d1"];  if(d=="null") {d="?";}
               txt="t"+t+" d"+d;
            }           
            if (moda==1) {
               linijka="--    ";
               symbolIcon = s2;
               String t  = doc["t2"];  if(t=="null") {t="?";}
               String d  = doc["d2"];  if(d=="null") {d="?";}
               txt="t"+t+" d"+d;
            }
            if (moda==2) {
               linijka="----  ";
               symbolIcon = s3;
               String t  = doc["t3"];  if(t=="null") {t="?";}
               String d  = doc["d3"];  if(d=="null") {d="?";}
               txt="t"+t+" d"+d;
            }
            if (moda==3) {
               linijka="------";
               symbolIcon = s4;
               String t  = doc["t4"];  if(t=="null") {t="?";}
               String d  = doc["d4"];  if(d=="null") {d="?";}
               txt="t"+t+" d"+d;
            }
            
            txtLong = txt.length();
            //Serial.print("txtLong=");Serial.println(txtLong);

            /*Wybór fontu zaleznie od dlugosci*/
            //if (txtLong>5) {u8g2.setFont(u8g2_font_profont22_mr);} // 10,11,12,15,17,22,29 //_mr=krotki  _mf=pelny _tr=skrocony _mn=numery
            //else {u8g2.setFont(u8g2_font_profont29_mr);}

            /*Wybór fontu zaleznie od dlugosci*/
            if (txtLong>5) {u8g2.setFont(u8g2_font_logisoso18_tr);} // 16,18,22,24,26,28 // _mr=krotki _mf=pelny _tr=skrocony _mn=numery
            else {u8g2.setFont(u8g2_font_logisoso24_tr);}

            // print linijka
            linijka.toCharArray(buftt, 16);
            u8g2.drawStr(0,8 ,buftt);

            // print buff with data
            txt.toCharArray(buftt, 16);
            u8g2.drawUTF8(0, 32, buftt);
            
            // print symbol
            u8g2.setFont(u8g2_font_open_iconic_weather_4x_t); // 4x=32px  2x=16px
            symbolIcon.toCharArray(buftt, 16);
            u8g2.drawUTF8(96, 32, buftt); 
            /*
              * u8g2_font_unifont_t_weather  W=15 H=16 S=888 bytes
              * info inne weather = https://raw.githubusercontent.com/wiki/olikraus/u8g2/fntpic/u8g2_font_unifont_t_weather.png
            */

        } while ( u8g2.nextPage() );
}


void server() {
    printOLED("Update");
    Serial.print("WiFiMulti.run()=");Serial.println(WiFiMulti.run());  //0=not conect
    if ((WiFiMulti.run() == WL_CONNECTED)) {
      WiFiClient client;
      HTTPClient http;
      Serial.println("[HTTP] begin...\n");
      printOLED("[HTTP]...");
      if (http.begin(client, "http://zszczech.zut.edu.pl/e/oled.json.php")) { 
        Serial.print("[HTTP] GET...\n");
        printOLED("[HTTP]...");
        int httpCode = http.GET();
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        if (httpCode > 0) {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            printOLED(itoa(httpCode, buftt, 10));
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                payload = http.getString();
                Serial.println(payload);
                DeserializationError error = deserializeJson(doc, payload);
                if (error) {
                  Serial.println(error.c_str());
                } else {
                   olejOLED();
                }
            }
        } else {printOLED("Error"); Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str()); }
        http.end();
      } else {printOLED("Unable to connect"); Serial.printf("[HTTP} Unable to connect\n");}
      
    } // if connected 
}


long intervalOled = 3*777; // ~3 sek
long previousOledMillis = 0;    

long intervalServer = 900*1000; // 15 minut
long previousServerMillis = 0;    

void loop() {
  unsigned long currentMillis = millis();

  // OLED timing system
  if(currentMillis - previousOledMillis > intervalOled  && previousServerMillis > 0) {
      previousOledMillis = currentMillis;   
      //Serial.print(currentMillis);Serial.println(" intervalOled===");
      olejOLED();
  }

   // Server timing system
  if(currentMillis - previousServerMillis > intervalServer || previousServerMillis == 0) {
      previousServerMillis = currentMillis;   
      //Serial.print(currentMillis); Serial.println(" intervalServer===");
      printOLED("Server=900");
      server();
  }

    // LOOP version
    //server();
    //delay(60000*15);
}
