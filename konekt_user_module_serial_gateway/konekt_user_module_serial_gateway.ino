/* Konekt User Module Passthrough Serial Gateway Program
 *
 * Author: Patrick F. Wilbur <pat@konekt.io> <pdub@pdub.net>
 *
 * Purpose: This firmware implements serial passthrough between
 * the main user-programmable microcontroller's peripherals and
 * the M2 microcontroller running Konekt's drivers and networking
 * and cloud stacks. This firmware is replaced whenever a user 
 * loads custom user firmware.
 *
 * License: Licensed under the MIT License.
 *
 * The MIT License (MIT)
 *
 * Copyright 2015 Konekt, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
*/

#define LED_DATA_INDICATOR_DURATION 200
#define LED_DATA_INDICATOR_FLICKER_OFF_DURATION 50
#define LED_DATA_INDICATOR_FLICKER_ON_DURATION 5

/* globals */
unsigned ledStartMillis;
unsigned ledNextFlickerMillis;
bool uplinkDataDetected;
bool downlinkDataDetected;
bool ledOn;

void setup() {
  Dash.begin();
  Serial2.begin(9600);
  SerialUSB.begin(9600);
  SerialCloud.begin(115200);
  ledStartMillis = 0; /* LED off by default */  
  ledNextFlickerMillis = 0;
  uplinkDataDetected = false;
  downlinkDataDetected = false;
  ledOn = false;
  ledIndicateData();
}

void loop() {
  char currChar;
  
  if(ledOn) {
    Dash.onLED();
  } else {
    Dash.offLED();
  }
  if (uplinkDataDetected) {
    /* for uplink data, we turn on LED for duration */
    ledOn=true;
    if(millis() > (ledStartMillis + LED_DATA_INDICATOR_DURATION)) {
      uplinkDataDetected=false;
      ledOn=false;
    }
  }
  if (!uplinkDataDetected && downlinkDataDetected) {
    /* for downlink data, we flash LED for duration */
    if(millis() > ledNextFlickerMillis) {
      if(ledOn) {
        ledOn=false;
        ledNextFlickerMillis = millis() + LED_DATA_INDICATOR_FLICKER_OFF_DURATION;
      } else {
        ledOn=true;
        ledNextFlickerMillis = millis() + LED_DATA_INDICATOR_FLICKER_ON_DURATION;
      }
    }
    if(millis() > (ledStartMillis + LED_DATA_INDICATOR_DURATION)) {
      downlinkDataDetected=false;
      ledOn=false;
    }
  }
  
  while(Serial2.available()) {
    SerialCloud.write(Serial2.read());
    uplinkDataDetected=true;
    ledIndicateData();
  }

  while(SerialUSB.available()) {
    SerialCloud.write(SerialUSB.read());
    uplinkDataDetected=true;
    ledIndicateData();
  }

  while(SerialCloud.available()) {
    currChar = (char)SerialCloud.read();
    SerialUSB.write(currChar);
    Serial2.write(currChar);
    downlinkDataDetected=true;
    ledNextFlickerMillis = millis() + LED_DATA_INDICATOR_FLICKER_ON_DURATION;
    ledIndicateData();
  }

  delay(1);
}

void ledIndicateData() {
  ledStartMillis=millis();
  ledOn=true;
}

