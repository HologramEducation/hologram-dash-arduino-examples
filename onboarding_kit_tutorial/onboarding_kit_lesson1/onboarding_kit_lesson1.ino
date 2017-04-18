//Hologram Lesson 1
//This lesson teaches how to blink an LED on the Dash

void setup() {
  
  // put your setup code here, to run once:
  // initialize digital pin as an output.
  pinMode(D30, OUTPUT);
  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  digitalWrite(D30, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(D30, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

}
