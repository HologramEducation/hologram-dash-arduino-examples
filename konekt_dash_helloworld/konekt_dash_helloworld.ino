/* Konekt Dash Hello World
 *
 * Author: Patrick F. Wilbur <hello@konekt.io> <pdub@pdub.net>
 *
 * Purpose: This firmware demonstrates interactive serial mode,
 * a mechanism for performing cable replacement serial passthrough
 * over cellular to the cloud. This example works out-of-the-box
 * with zero configuration.
 *
 * License: Copyright 2015 Konekt, Inc. All Rights Reserved.
*/

void setup() {
  Serial2.begin(9600); /* TTL UART */
  SerialCloud.begin(115200); /* Konekt Cloud */
  SerialCloud.println("Hello, World!"); /* one-time message */
}

void loop() {
  /* we don't loop sending data, since we don't want to eat up
   * a lot of data on our SIM! */

  /* the code here will pass data between Cloud<-->UART */

  while(Serial2.available()) {
    SerialCloud.write(Serial2.read());
  }
  
  while(SerialCloud.available()) {
    Serial2.write(SerialCloud.read());
  }

  delay(5);
}


