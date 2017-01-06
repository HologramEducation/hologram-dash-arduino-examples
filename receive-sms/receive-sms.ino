int controlPin = R04;

#include <DashReadEvalPrint.h>

//Print a received SMS
void on_message(String &msg)
{
  Serial.println();
  Serial.println("Message Received:");
  Serial.println(msg);

  if (msg == "ON") {
    digitalWrite(controlPin, LOW);
    SerialUSB.println("Switched to ON");
    Serial0.println("Switched to ON");
  } else if (msg == "OFF") {
    digitalWrite(controlPin, HIGH);
    SerialUSB.println("Switched to OFF");
    Serial0.println("Switched to OFF");
  } else if (msg == "RESET") {
    digitalWrite(controlPin, HIGH);
    SerialUSB.println("Resetting... Switched to OFF");
    Serial0.println("Resetting... Switched to OFF");
    delay(30000);
    digitalWrite(controlPin, LOW);
    SerialUSB.println("Switched to ON");
    Serial0.println("Switched to ON");
  }
}

void setup()
{
  SerialUSB.begin(9600);
  Serial0.begin(9600); /* TTL UART */
  SerialCloud.begin(115200);
  pinMode(controlPin, OUTPUT);
  delay(4000);

  DashReadEvalPrint.begin();                            //Initialize ReadEvalPrint
  DashReadEvalPrint.setDebugPort(Serial0);              //Use Serial0 for debug messages
  DashReadEvalPrint.attachMessageReceived(on_message);  //Handle received SMS

  Dash.begin();
}

void loop() {
  //open the Serial Monitor and type help for a list of commands
  DashReadEvalPrint.run(Serial);                        //Run the interpreter using USB Serial
}
