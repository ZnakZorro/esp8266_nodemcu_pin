#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>

#define CONTROL_RATE 128
Oscil <2048, AUDIO_RATE> aSin0(SIN2048_DATA);
Oscil <2048, AUDIO_RATE> aSin1(SIN2048_DATA);
Oscil <2048, AUDIO_RATE> aSin2(SIN2048_DATA);

float freq0 = 220.0;
float freq1 = 440.0;
float freq2 = 880.0;


void setup(){
  Serial.begin(115200);
  Serial.print("add3");
  startMozzi(CONTROL_RATE);
}

void updateControl(){
  float adc0 = mozziAnalogRead(0);
  float adc1 = mozziAnalogRead(1);
  float adc2 = mozziAnalogRead(2);
      Serial.print(adc0);Serial.print("\t");
      Serial.print(adc1);Serial.print("\t");
      Serial.println(adc2);
      
  aSin0.setFreq(adc0*3);
  aSin1.setFreq(adc1/20);
  aSin2.setFreq(adc2/2000);
}

int updateAudio(){
  return (aSin0.next()>>1 * aSin1.next()>>1 * aSin2.next()>>1);
}

void loop(){
  audioHook();
}
