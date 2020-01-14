/**/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <ArduinoJson.h>    //https://github.com/bblanchon/ArduinoJson

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);

char   buftt[16];
//char linia1[16];
//char linia2[16];
//char linia3[16];
//char linia4[16];
String linia1  = "";
String linia2  = "";
String linia3  = "";
String linia4  = "";
String symbol1  = "";
String symbol2  = "";
String symbol3  = "";
String symbol4  = "";

//char *text = "V1=";
String txt="";

String tt="";
byte bajt = 0;
StaticJsonDocument<250> doc;
ESP8266WiFiMulti WiFiMulti;

void setup() {
  u8g2.begin();
  u8g2.setFontMode(0); // mode transparent
  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  for (uint8_t t = 5; t > 0; t--) {
    //Serial.printf("[SETUP] WAIT %d...\n", t);
    txt = "WiFi #";    txt.concat(t);    printOLED(txt);
    Serial.flush();
    delay(1000);
  }
  
  WiFi.mode(WIFI_STA);
  
          WiFiMulti.addAP("webWI", "wi77tam77");
          //WiFiMulti.addAP("znakzorro", "niemieckiewino");
          Serial.print("Wait for WiFi... ");
          txt="Try %";
            while (WiFiMulti.run() != WL_CONNECTED){
              Serial.print("#");
              txt.concat("%");    printOLED(txt);
              delay(500);
            }
            Serial.println("");
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            delay(500);
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

void parseJsonSingle(int nr){
    if (nr==1){ 
      unsigned int xLength = linia1.length(); 
      if (xLength>0) { return;}
      else{
        String t  = doc["t1"];  if(t=="null") {t="?";}
        String d  = doc["d1"];  if(d=="null") {d="?";}
        linia1="t"+t+" d"+d;
        String s = doc["s1"]; if(s=="null") {s="A";}
        symbol1 = s;
      }    
    }
    if (nr==2){ 
      unsigned int xLength = linia2.length(); 
      if (xLength>0) { return;}
      else{
        String t  = doc["t2"];  if(t=="null") {t="?";}
        String d  = doc["d2"];  if(d=="null") {d="?";}
        linia2="t"+t+" d"+d;
        String s = doc["s2"]; if(s=="null") {s="A";}
        symbol2 = s;
      }    
    }
    if (nr==3){ 
      unsigned int xLength = linia3.length(); 
      if (xLength>0) { return;}
      else{
        String t  = doc["t3"];  if(t=="null") {t="?";}
        String d  = doc["d3"];  if(d=="null") {d="?";}
        linia3="t"+t+" d"+d;
        String s = doc["s3"]; if(s=="null") {s="A";}
        symbol3 = s;
      }    
    }
    if (nr==4){ 
      unsigned int xLength = linia4.length(); 
      if (xLength>0) { return;}
      else{
        String t  = doc["t4"];  if(t=="null") {t="?";}
        String d  = doc["d4"];  if(d=="null") {d="?";}
        linia4="t"+t+" d"+d;
        String s = doc["s4"]; if(s=="null") {s="A";}
        symbol4 = s;
      }    
    }
}

void parseJson(){
    parseJsonSingle(1);
    parseJsonSingle(2);
    parseJsonSingle(3);
    parseJsonSingle(4); 
}

void olejOLED(){
 
    Serial.print("1 linia1.length=");  Serial.println(linia1.length()); Serial.println(linia1);

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
               symbolIcon = symbol1;
               txt = linia1;
            }           
            if (moda==1) {
               linijka="--    ";
               symbolIcon = symbol2;
               txt = linia2;
            }
            if (moda==2) {
               linijka="---   ";
               symbolIcon = symbol3;
               txt = linia3;
            }
            if (moda==3) {
               linijka="------";
               symbolIcon = symbol4;
               txt = linia4;
            }
            
            txtLong = txt.length();
            //Serial.print("txtLong=");Serial.println(txtLong);

            /*Wybór fontu zaleznie od dlugosci*/
            if (txtLong>5) {u8g2.setFont(u8g2_font_profont22_mr);} // 10,11,12,15,17,22,29 //_mr=krotki  _mf=pelny _tr=skrocony _mn=numery
            else {u8g2.setFont(u8g2_font_profont29_mr);}

            /*Wybór fontu zaleznie od dlugosci*/
            //if (txtLong>5) {u8g2.setFont(u8g2_font_logisoso18_tr);} // 16,18,22,24,26,28 // _mr=krotki _mf=pelny _tr=skrocony _mn=numery
            //else {u8g2.setFont(u8g2_font_logisoso24_tr);}

            // print linijka
            linijka.toCharArray(buftt, 16);
            u8g2.drawStr(0,10 ,buftt);

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
                String payload = http.getString();
                Serial.println(payload);
                DeserializationError error = deserializeJson(doc, payload);
                if (error) {
                  Serial.println(error.c_str());
                } else {
                   parseJson();
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
