//Hologram Lesson 2A
//This lesson teaches how to read a button output.

int lastDebounceTime = 0;
int debounceDelay = 50;
int lastButtonState = 1;
int ledState = LOW;
int buttonReading = 0;
int buttonState = HIGH;

void setup() {
  
  // put your setup code here, to run once:

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(D30, OUTPUT);
  pinMode(D19, INPUT);

}

void loop() {
  
  buttonReading = digitalRead(D19);
  
  if (buttonReading != lastButtonState) {
      lastDebounceTime = millis();
      lastButtonState = buttonReading;
  } 

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonState != lastButtonState) {
      buttonState = lastButtonState;
        if (buttonState == LOW) {
                 ledState = !ledState;
                 digitalWrite(D30, ledState);
          }
      }
   }

}
