// ---------------------------------------------------------------------------
// Connect your piezo buzzer (without internal oscillator) or speaker to these pins:
//   Pins  9 & 10 - ATmega328, ATmega128, ATmega640, ATmega8, Uno, Leonardo, etc.
//   Pins 11 & 12 - ATmega2560/2561, ATmega1280/1281, Mega
//   Pins 12 & 13 - ATmega1284P, ATmega644
//   Pins 14 & 15 - Teensy 2.0
//   Pins 25 & 26 - Teensy++ 2.0
// Be sure to include an inline 100 ohm resistor on one pin as you normally do when connecting a piezo or speaker.
// ---------------------------------------------------------------------------

#include <toneAC.h>

// Melody liberated from the toneMelody Arduino example sketch by Tom Igoe.
int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

static const uint32_t DELAY_1_S       = 1000UL;
static const uint32_t DELAY_1_MINUTE  = DELAY_1_S * 60UL;
static const uint32_t DELAY_5_MINUTE  = DELAY_1_MINUTE * 5UL;
static const uint32_t DELAY_15_MINUTE = DELAY_1_MINUTE * 15UL;
static const uint32_t DELAY_60_MINUTE = DELAY_1_MINUTE * 60UL;

void setup() {
  Serial.begin(115200);
  Serial.println(1);
  Serial.println("toneAC_demo2");  
}

int grajrandom(int fmin, int fmax, int ile, int dura){
    Serial.println("toneAC_demo2");
    Serial.println(fmin,fmax);
    //Serial.println(fmax);
    //Serial.println(ile);
    //Serial.println(dura);
    Serial.println("--------------");
  for(int i=0; i<ile; i++){
    int randNumber = random(fmin, fmax);
    int frq = randNumber;
    Serial.println(frq);
    toneAC(frq); 
    delay(dura);
  }
}

int cogodzine(){
   Serial.println("co godzine");
  grajrandom(200,   2000, 50, 50);
  toneAC();
  delay(DELAY_60_MINUTE);
  cogodzine();
}

void loop() {
  for (unsigned long freq = 125; freq <= 5000; freq += 10) {  
    toneAC(freq);
    delay(5);
  }
  toneAC();
  

  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    toneAC(melody[thisNote], 10, noteDuration, true);
    delay(noteDuration * 4 / 3);
  }

  grajrandom(200,   2000, 50, 250);
  toneAC();
  delay(1000);
  grajrandom(200,   2000, 50, 50);
  toneAC();
  cogodzine();
  //delay(DELAY_60_MINUTE);
  while(1);
}
