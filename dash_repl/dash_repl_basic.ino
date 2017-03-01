/*
  dash_repl_basic.ino - processes Dash commands in a Read-Eval-Print-Loop.

  http://hologram.io

  Copyright (c) 2016 Konekt, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <DashReadEvalPrint.h>

//send a notifcation message on RTC alarm
//trigger the alarm to go off in 5 seconds with the command:
//alarm seconds 5
void alarm_handler(void)
{
  rtc_datetime_t dt;
  if(HologramCloud.getNetworkTime(dt))
    Clock.setDateTime(dt);
  HologramCloud.println("Alarm at ");
  HologramCloud.println(Clock.currentDateTime());
  HologramCloud.attachTag("alarm");
  if(HologramCloud.sendMessage())
    Serial.println("Alarm notification sent");
  else
    Serial.println("Alarm notification failed");
}

//Toggle the LED on timer expiration
//trigger the timer every 2 seconds with the command:
//timer repeat 2000
void timer_handler(void)
{
  Dash.toggleLED();
}

//On received SMS, print the sender, timestamp and message
//Send a message to the cloud that an SMS was received with the sender number
//as the content and the tag SMSRX
void cloud_sms(const String &sender, const rtc_datetime_t &timestamp, const String &message) {
  Serial.println("CLOUD SMS RECEIVED:");
  Serial.print("SMS SENDER: ");
  Serial.println(sender);
  Serial.print("SMS TIMESTAMP: ");
  Serial.println(timestamp);
  Serial.println("SMS TEXT: ");
  Serial.println(message);

  if(HologramCloud.sendMessage(sender, "SMSRX")) {
    Serial.println("SMS received message sent to cloud.");
  } else {
    Serial.println("Notification send failed.");
    Serial.println("Check failure reason by typing:");
    Serial.println("cloud status");
  }
}

void setup() {
  Serial.begin();                                       //Start USB Serial
  DashReadEvalPrint.begin();                            //Initialize ReadEvalPrint
  Clock.attachAlarmInterrupt(alarm_handler);            //Handle RTC alarm
  Dash.attachTimer(timer_handler);                      //Handle timer expiration
  HologramCloud.attachHandlerSMS(cloud_sms);            //Handle received SMS
}

void loop() {
  //open the Serial Monitor and type help for a list of commands
  DashReadEvalPrint.run(Serial);                        //Run the interpreter using USB Serial
}