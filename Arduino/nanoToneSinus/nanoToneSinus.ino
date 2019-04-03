int sintable[] = { 
  0,0,0,0,0,0,1,1,2,3,3,4,5,6,7,8,9,10,12,13,15,16,18,19,21,23,25,26,28,30,33,35,
  37,39,41,44,46,49,51,54,56,59,61,64,67,70,72,75,78,81,84,87,90,93,96,99,102,105,108,111,115,118,121,124,
  127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,
  176,179,182,184,187,190,193,195,198,200,203,205,208,210,213,215,
  217,219,221,224,226,228,229,231,233,235,236,238,239,241,242,244,
  245,246,247,248,249,250,251,251,252,253,253,254,254,254,254,254,
  255,254,254,254,254,254,253,253,252,251,251,250,249,248,247,246,
  245,244,242,241,239,238,236,235,233,231,229,228,226,224,221,219,
  217,215,213,210,208,205,203,200,198,195,193,190,187,184,182,179,
  176,173,170,167,164,161,158,155,152,149,146,143,139,136,133,130,
  127,124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,
  78,75,72,70,67,64,61,59,56,54,51,49,46,44,41,39,
  37,35,33,30,28,26,25,23,21,19,18,16,15,13,12,10,
  9,8,7,6,5,4,3,3,2,1,1,0,0,0,0,0
};

static const uint32_t DELAY_1_S       = 1000UL;
static const uint32_t DELAY_1_MINUTE  = DELAY_1_S * 60UL;
static const uint32_t DELAY_5_MINUTE  = DELAY_1_MINUTE * 5UL;
static const uint32_t DELAY_15_MINUTE = DELAY_1_MINUTE * 15UL;
static const uint32_t DELAY_60_MINUTE = DELAY_1_MINUTE * 60UL;
static const uint32_t DELAY_1_HOUR    = DELAY_1_MINUTE * 60UL;

int outPIN = 5;

int graj(int skala){
    Serial.println(skala);
  int noteDuration = skala;
  for(int i=0;i<255;++i){
    int frq = (sintable[i] *skala)+(skala*20);
    tone(outPIN, frq, noteDuration);
    delay(noteDuration+1);
  }
}


int grajmm(int fmin, int fmax, int skok, int dura){
    Serial.println("nanoTone1");
    Serial.println(fmin);
    Serial.println(fmax);
    Serial.println(skok);
    Serial.println(dura);
    Serial.println("---");
  for(int i=fmin; i<fmax; i+=skok){
    int frq = i;
    Serial.println(frq);
    tone(outPIN, frq, dura);
    delay(dura+10);
  }
}


int grajrandom(int fmin, int fmax, int skok, int dura){
    Serial.println("nanoTone1");
    Serial.println(fmin);
    Serial.println(fmax);
    Serial.println(skok);
    Serial.println(dura);
    Serial.println("---");
  for(int i=fmin; i<fmax; i+=skok){
    int randNumber = random(fmin, fmax);
    int frq = randNumber;
    Serial.println(frq);
    tone(outPIN, frq, dura);
    delay(dura+10);
  }
}




void setup() {
  Serial.begin(115200);
  Serial.println(1);
  Serial.println("nanoToneSinus");
  //TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  pinMode(13,OUTPUT);
  pinMode(6,OUTPUT);
}

void loop() {

grajrandom(200,   2000, 200, 250);
grajrandom(200,   2000, 20, 50);
//grajrandom(20,     200,  10, 1000);
//grajrandom(200,    500,  30, 1000);
//grajrandom(500,   5000, 200, 1000);
//grajrandom(5000, 10000, 500, 1000);
  
/*
  graj(2);
  graj(5);
  graj(15);
  graj(20);
  graj(30);
  graj(40);
  graj(100);
  graj(200);
*/
  //graj(30); // max
  
  delay(DELAY_60_MINUTE);
  


}
