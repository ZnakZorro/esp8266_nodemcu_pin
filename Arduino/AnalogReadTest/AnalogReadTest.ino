int analogPin0 = 0;
int analogPin1 = 1;
int analogPin2 = 2;
int analogPin3 = 3;
int analogPin4 = 4;
int val0 = 0;
int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;
void setup(){
  //Serial.begin(9600);
  Serial.begin(115200);
  Serial.println("AnalogReadTest");
}

void loop(){
  val0 = analogRead(analogPin0);
  val1 = analogRead(analogPin1);
  val2 = analogRead(analogPin2);
  val3 = analogRead(analogPin3);
  val4 = analogRead(analogPin4);
  Serial.print(val0);
  Serial.print("\t");
  Serial.print(val1);
  Serial.print("\t");
  Serial.print(val2);
  Serial.print("\t");
  Serial.print(val3);
  Serial.print("\t");
  Serial.println(val4);
  delay(333);
}
