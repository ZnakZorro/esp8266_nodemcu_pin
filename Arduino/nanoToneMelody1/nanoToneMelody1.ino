#include "pitches.h"
 
int melody[] = {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_GS3, NOTE_G3,0, NOTE_B3, NOTE_C4};
int noteDurations[] = { 4, 8, 8, 4,4,4,4,4 };


int graj(int melodia){
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(13, melody[thisNote],noteDuration);
    delay(noteDuration +30);
  }
}
 
void setup() {
  Serial.begin(115200);
  Serial.println(1);
  Serial.println("nanoToneMelody1");  
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(13, melody[thisNote],noteDuration);
    delay(noteDuration +30);
  }
}
 
void loop() {
  graj(1);
  delay(60000);
}
