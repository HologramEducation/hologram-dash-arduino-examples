//Hologram Lesson 3A
//This lesson turns the Lesson 2 system into a local motion detection security system

#define ARMED 1
#define UNARMED 2

int lastDebounceTime = 0;
int debounceDelay = 50;
int lastButtonState = 1;
int ledState = LOW;
int buttonReading = 0;
int pirReading = 0;
int motionDetect = LOW;
int alarmState = UNARMED;

int buttonState = HIGH;

void setup() {
  
  // put your setup code here, to run once:

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(D30, OUTPUT);
  pinMode(D19, INPUT);
  pinMode(D09, INPUT);

  Serial.begin(9600);

}

void loop() {


 //blink LED if alarm is unarmed.  arm alarm by pressing the button
 if (alarmState == UNARMED) {
  
    digitalWrite(D30, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(D30, LOW);    // turn the LED off by making the voltage LOW
    delay(100);                       // wait for a second

    buttonReading = digitalRead(D19);
    
    if (buttonReading != lastButtonState) {
      lastDebounceTime = millis();
      lastButtonState = buttonReading;
  } 

  //the button toggles security system ARMED (1) or UNARMED (0)
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (buttonState != lastButtonState) {
        buttonState = lastButtonState;
        if (buttonState == LOW){
          alarmState = ARMED;
          Serial.println("Button was pressed.  System will be armed in 5 seconds.");
          delay(5000);
        }
        }
     }
  
 }

 if (alarmState == ARMED) {
  digitalWrite(D30, HIGH);
  pirReading = digitalRead(D09);
  if (pirReading == HIGH){
    alarmState = UNARMED;
    Serial.println("Intruder detected! System unarmed.");
  }
 }
 

}
