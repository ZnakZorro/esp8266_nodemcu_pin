/*
  Example using 2 light dependent resistors (LDRs) to change
  FM synthesis parameters, and a knob for fundamental frequency,
  using Mozzi sonification library.

  Demonstrates analog input, audio and control oscillators, phase modulation
  and smoothing a control signal at audio rate to avoid clicks.
  Also demonstrates AutoMap, which maps unpredictable inputs to a set range.

  This example goes with a tutorial on the Mozzi site:
  http://sensorium.github.io/Mozzi/learn/introductory-tutorial/

  The circuit:
     Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.1, or
     check the README or http://sensorium.github.com/Mozzi/

     Potentiometer connected to analog pin 0.
       Center pin of the potentiometer goes to the analog pin.
       Side pins of the potentiometer go to +5V and ground

     Light dependent resistor (LDR) and 5.1k resistor on analog pin 1:
       LDR from analog pin to +5V (3.3V on Teensy 3.1)
       5.1k resistor from analog pin to ground

     Light dependent resistor (LDR) and 5.1k resistor on analog pin 2:
       LDR from analog pin to +5V (3.3V on Teensy 3.1)
       5.1k resistor from analog pin to ground

    Mozzi documentation/API
    https://sensorium.github.io/Mozzi/doc/html/index.html

    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users

  Tim Barrass 2013, CC by-nc-sa.
*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <Smooth.h>
#include <AutoMap.h> // maps unpredictable inputs to a range

// desired carrier frequency max and min, for AutoMap
const int MIN_CARRIER_FREQ = 22;
const int MAX_CARRIER_FREQ = 440;

// desired intensity max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;

// desired mod speed max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_MOD_SPEED = 10000;
const int MAX_MOD_SPEED = 1;

int xmap=1023; // 1023
AutoMap kMapCarrierFreq(0,xmap,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0,xmap,MIN_INTENSITY,MAX_INTENSITY);
AutoMap kMapModSpeed(0,xmap,MIN_MOD_SPEED,MAX_MOD_SPEED);

const int KNOB_PIN = 0; // set the input for the knob to analog pin 0
const int LDR1_PIN = 1; // set the analog input for fm_intensity to pin 1
const int LDR2_PIN = 2; // set the analog input for mod rate to pin 2

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator2(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(COS2048_DATA);

int mod_ratio = 5; // brightness (harmonics)
int licznik1 =0;
int licznik2 =0;
int kierunek1=1;
int kierunek2=1;
int krok1=1;
int krok2=3;
int gain=1;

long fm_intensity; // carries control info from updateControl to updateAudio

// smoothing for intensity to remove clicks on transitions
float smoothness = 0.95f;
Smooth <long> aSmoothIntensity(smoothness);


void setup(){
  //analogReference(INTERNAL);
  //analogReference(EXTERNAL);
  //Serial.begin(9600);
  Serial.begin(115200);
  Serial.print("Leo_Fono_v1");
  startMozzi(); // :))
}


void updateControl(){
  licznik1 +=kierunek1 * krok1;
  licznik2 +=kierunek2 * krok2;
  if (licznik1>100 || licznik1<1) kierunek1*=-1;
  if (licznik2>100 || licznik2<1) kierunek2*=-1;
  
  // read the knob
  int knob_value = mozziAnalogRead(KNOB_PIN)*10/7; // value is 0-1023
  //Serial.println(knob_value);
  // map the knob to carrier frequency
  int carrier_freq = kMapCarrierFreq(knob_value)+licznik1;

  //calculate the modulation frequency to stay in ratio
  int mod_freq = (carrier_freq * mod_ratio)+licznik2;

  // set the FM oscillator frequencies
  aCarrier.setFreq(carrier_freq);
  aModulator.setFreq(mod_freq);
  aModulator2.setFreq(mod_freq<<1);

  // read the light dependent resistor on the width Analog input pin
  int LDR1_value= mozziAnalogRead(LDR1_PIN)*10/7; // value is 0-1023
  // print the value to the Serial monitor for debugging
  Serial.print("LDR1 = ");
  Serial.print(LDR1_value);
  Serial.print("\t"); // prints a tab

  int LDR1_calibrated = kMapIntensity(LDR1_value);
  Serial.print("LDR1_calibrated = ");
  Serial.print(LDR1_calibrated);
  Serial.print("\t"); // prints a tab

 // calculate the fm_intensity
  fm_intensity = ((long)LDR1_calibrated * (kIntensityMod.next()+128))>>8; // shift back to range after 8 bit multiply
  Serial.print("fm_intensity = ");
  Serial.print(fm_intensity);
  Serial.print("\t"); // prints a tab

  // read the light dependent resistor on the speed Analog input pin
  int LDR2_value= (int) mozziAnalogRead(LDR2_PIN)*10/7; // value is 0-1023
  krok1 = LDR1_value >> 5;
  krok2 = LDR2_value >> 5;
  Serial.print("LDR2 = ");
  Serial.print(LDR2_value);
  Serial.print("\t");

  // use a float here for low frequencies
  float mod_speed = (float)kMapModSpeed(LDR2_value)/1000;
                if (LDR2_value<2) gain =0; else gain=1;
  
  Serial.print("   mod_speed = ");
  Serial.print(mod_speed);
  Serial.print("\t");
  Serial.print(gain);
  kIntensityMod.setFreq(mod_speed);

  Serial.println(); // finally, print a carraige return for the next line of debugging info
}


int updateAudio(){
  //long modulation = aSmoothIntensity.next(fm_intensity) * (aModulator.next();
  long modulation = aSmoothIntensity.next(fm_intensity) * (aModulator.next()) + (aModulator2.next()+aModulator2.next()>>1);
  modulation *=gain; 
  return aCarrier.phMod(modulation);
}


void loop(){
  audioHook();
}
