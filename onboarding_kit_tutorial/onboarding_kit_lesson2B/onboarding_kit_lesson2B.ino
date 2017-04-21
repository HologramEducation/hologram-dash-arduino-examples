//Hologram Lesson 2B
//This lesson uses a PIR sensor to turn on an LED

int pirReading = 0;

void setup() {
  
  // put your setup code here, to run once:

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(D30, OUTPUT);
  pinMode(D09, INPUT);

}

void loop() {
  
  pirReading = digitalRead(D09);
  digitalWrite(D30, pirReading);

}
