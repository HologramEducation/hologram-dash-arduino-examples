/*
 *  receive_sms.ino
 *
 *  Demonstrates HologramCloud SMS received handler.
 *  Requires Hologram Dash library 0.9.2 or later
 */

void cloud_sms(const sms_event *sms) {
  Serial.print("SMS received from ");
  Serial.print(sms->sender);
  Serial.print(" at ");
  Serial.println(sms->timestamp);
  Serial.println(sms->message);
}

void setup()
{
  Serial.begin(9600);
  HologramCloud.begin();
  HologramCloud.attachHandlerSMS(cloud_sms)
}

void loop() {
  HologramCloud.pollEvents();
}
