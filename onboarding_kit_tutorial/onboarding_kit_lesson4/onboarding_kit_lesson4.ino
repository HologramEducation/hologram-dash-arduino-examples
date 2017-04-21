//Hologram Lesson 4
//ARM, DISARM via SMS and send status with STATUS

#define ARMED 1
#define UNARMED 2

int lastDebounceTime = 0;
int debounceDelay = 50;
int lastButtonState = 1;
int ledState = LOW;
int buttonReading = 0;
int alarmState = UNARMED;
int buttonState = HIGH;

void cloud_sms(const String &sender, const rtc_datetime_t &timestamp, const String &message) {
  Serial.println("CLOUD SMS RECEIVED:");
  Serial.print("SMS SENDER: ");
  Serial.println(sender);
  Serial.print("SMS TIMESTAMP: ");
  Serial.println(timestamp);
  Serial.println("SMS TEXT: ");
  Serial.println(message);

  if (message == "ARM") {
    alarmState = ARMED;
    Serial.print("Alarm state is: ");
    Serial.println(alarmState);
    HologramCloud.sendMessage("System is currently armed", "ALARM");
  }

  if (message == "DISARM") {
    alarmState = UNARMED;
    Serial.print("Alarm state is: ");
    Serial.println(alarmState);
    HologramCloud.sendMessage("System is currently unarmed", "ALARM");
  }
  if (message == "STATUS") {
    Serial.println("Sending status...");
    if (alarmState == ARMED) {
      HologramCloud.sendMessage("System is currently armed", "ALARM");
    }
    else {
      HologramCloud.sendMessage("System is currently unarmed", "ALARM");
    }
  }

  Serial.println("Leaving SMS handler...");

}

void setup() {
  // put your setup code here, to run once:

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(D30, OUTPUT);
  pinMode(D19, INPUT_PULLUP);
  pinMode(D09, INPUT_PULLDOWN);

  Serial.begin();
  HologramCloud.attachHandlerSMS(cloud_sms);
}

void loop() {
  //blink LED if alarm is unarmed.  arm alarm by pressing the button
  if (alarmState == UNARMED) {

    //blink the LED
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
        if (buttonState == LOW) {
          alarmState = ARMED;
          Serial.println("Button was pressed.  System will be armed in 5 seconds.");
          delay(5000);
        }
        
      }
    }
  }

  if (alarmState == ARMED) {
    digitalWrite(D30, HIGH);
    int pirReading = digitalRead(D09);
    if (pirReading == HIGH) {
      int whileCounter = 5;
      alarmState = UNARMED;
      buttonReading = digitalRead(D19);

      Serial.println("Intruder detected! System unarmed.");
      bool messageSuccess = HologramCloud.sendMessage("Intruder Detected!", "ALARM");

      while (!messageSuccess && whileCounter--) {
        Serial.println("Retrying message send...");
        messageSuccess = HologramCloud.sendMessage();
      }

      if(messageSuccess)
        Serial.println("Message transmitted successfully.");
      else
        Serial.println("Message transmission failed.");

      lastButtonState = 0;
      buttonReading = 0; //reset button inputs

    }
  }
}
