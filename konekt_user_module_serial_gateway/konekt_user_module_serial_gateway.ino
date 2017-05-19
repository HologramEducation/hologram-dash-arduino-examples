/* Hologram Dash User Module Cloud Serial Gateway Program
 *
 * Author: Hologram <support@hologram.io>
 *
 * Purpose: This firmware sends all data that comes in on a serial port to the
 * Hologram Cloud. Data is sent once a newline is detected.
 *
 * License: Licensed under the MIT License.
 *
 * The MIT License (MIT)
 *
 * Copyright 2017 Hologram (Konekt, Inc.)
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

/* globals */
unsigned ledStopMillis;
bool isHandlingData;

void setup() {
  Serial2.begin(9600);
  SerialUSB.begin(9600);
  ledStopMillis = 0; /* LED off by default */  
  isHandlingData = false;
  printToBoth("+++");
}

void printToBoth(String msg) {
  // Write to USB and UART
  Serial2.println(msg);
  SerialUSB.println(msg);
}

void writeOrFlush(char cur) {
  // Write each character to HologramCloud buffer and then
  // send to cloud on a newline character
  isHandlingData = true;
  if(cur == '\n') {
      printToBoth("Sending message to cloud...");
      int retriesLeft = 2;
      while(retriesLeft > 0) {
        bool res = HologramCloud.sendMessage();
        if(!res) {
          String msg = "Failed. Trying ";
          msg += retriesLeft + " more times";
          printToBoth(msg);
        } else {
          printToBoth("Data sent to cloud successfully");
          break;
        }
        retriesLeft--;
      }
    } else {
      HologramCloud.write(cur);
      setLedState();
    }
}


void loop() {
  char currChar;

  isHandlingData = false;

  while(Serial2.available()) {
    writeOrFlush(Serial2.available());
  }

  while(SerialUSB.available()) {
    writeOrFlush(SerialUSB.read());
  }

  while(SerialCloud.available()) {
    currChar = (char)SerialCloud.read();
    SerialUSB.write(currChar);
    Serial2.write(currChar);
    isHandlingData = true;
  }

  setLedState();
  
  delay(1);
}

void setLedState() {
  /* Blink LED while we're doing data operations and 200ms afterward */
  
  if(ledStopMillis > 0 && ledStopMillis < millis()) {
    Dash.offLED();
    ledStopMillis = 0;
  }
  
  if(isHandlingData && ledStopMillis == 0) {
    Dash.pulseLED(25, 50);
    ledStopMillis = millis() + LED_DATA_INDICATOR_DURATION;
  }
}

