#include <MozziGuts.h>
#include <Oscil.h> // oscillator
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <Smooth.h>
#include <AutoMap.h> // maps unpredictable inputs to a range

const int MIN_KNOB_Map = 12;
const int MAX_KNOB_Map = 840;

const int MIN_INTENSITY = 1200;
const int MAX_INTENSITY = 10;

const int MIN_LDR2_Map = 10000;
const int MAX_LDR2_Map = 1;

AutoMap kMapCarrierFreq(0,1023,MIN_KNOB_Map,MAX_KNOB_Map);
AutoMap kMapIntensity(0,1023,MAX_INTENSITY,MIN_INTENSITY);
AutoMap kMapModSpeed(0,1023,MAX_LDR2_Map,MIN_LDR2_Map);

const int KNOB_PIN = 0; // set the input for the knob to analog pin 0
const int LDR1_PIN=1; // set the analog input for fm_intensity to pin 1
const int LDR2_PIN=2; // set the analog input for mod rate to pin 2

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> aCarrier2(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> aCarrier3(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(COS2048_DATA);

int mod_ratio = 5; // brightness (harmonics)
long fm_intensity; // carries control info from updateControl to updateAudio

float smoothness = 0.95f;
Smooth <long> aSmoothIntensity(smoothness);


void setup(){
  //Serial.begin(9600);
  Serial.begin(115200);
  startMozzi(); // :))
  aCarrier2.setFreq(0.333f);
  aCarrier3.setFreq(0.333f);
}


void updateControl(){
  //AnalogRead=0-730
  int KNOB = mozziAnalogRead(KNOB_PIN);
  int KNOB_Map = kMapCarrierFreq(KNOB);
    int LDR1= mozziAnalogRead(LDR1_PIN);
    int LDR1_Map = kMapIntensity(LDR1);
      int LDR2= mozziAnalogRead(LDR2_PIN);
      float LDR2_Map = (float)kMapModSpeed(LDR2)/1000;
      
  int skala1 = ((int)KNOB/200);
  float skala2 = ((float)LDR1/200)+0.333;
  float skala3 = ((float)LDR2/200)+0.333;
    
  int mod_freq = KNOB_Map;// * mod_ratio;
  aCarrier.setFreq(KNOB_Map>>skala1);
 
  /***********************************************************************************/
  //aModulator.setFreq(mod_freq * aCarrier2.next()>>5);
  aModulator.setFreq(mod_freq>>2 * (aCarrier2.next()>>5 + aCarrier3.next()>>5));

  
  
  
  // print the value to the Serial monitor for debugging
  //Serial.print("LDR1 = ");
  //Serial.print(LDR1);
  //Serial.print("\t"); // prints a tab

  //Serial.print("LDR1_Map = ");
  //Serial.print(LDR1_Map);
  //Serial.print("\t"); // prints a tab

  fm_intensity = ((long)LDR1_Map * (kIntensityMod.next()+128))>>8; // shift back to range after 8 bit multiply
  //Serial.print("fm_intensity = ");
  //Serial.print(fm_intensity);
  //Serial.print("\t"); // prints a tab

  kIntensityMod.setFreq(LDR2_Map);
  //Serial.print("LDR2 = ");
  //Serial.print(LDR2);
  //Serial.print("\t"); // prints a tab

  //Serial.print("   LDR2_Map = ");
  //Serial.print(LDR2_Map);

  //Serial.println(); // finally, print a carraige return for the next line of debugging info

  aCarrier2.setFreq(skala2*0.333f);
  aCarrier3.setFreq(skala3*0.333f);
  Serial.print("S1=");Serial.print(skala1);Serial.print("\tS2=");Serial.print(skala2);Serial.print("\tS3=");Serial.print(skala3);
  Serial.println();
  
}


int updateAudio(){
  long modulation = aSmoothIntensity.next(fm_intensity) * aModulator.next();
  return aCarrier.phMod(modulation);
}


void loop(){
  audioHook();
}
