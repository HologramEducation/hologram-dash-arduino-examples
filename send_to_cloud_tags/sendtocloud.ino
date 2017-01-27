/*
 * This example shows how message tags can be used to mark data for different
 * uses in the Hologram cloud.  This program reads a sensor input.  If the
 * sensor reading is low, the data is transmitted without an ALARM tag
 * (in this case, it's marked as in the SAFE_ZONE).  If the sensor reading is
 * too high, the data is tagger as ALARM.  The user can see these tags in their
 * device logs
 *
 * As a next exercise, the user should consider using Hologram's cloud to route
 * any data tagged with ALARM to an SMS.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

int sensor_data = 0;
String sensor_string;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  HologramCloud.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  sensor_data = analogRead(A01);
  sensor_string = String(sensor_data); //convert int to string for use by HologramCloud

  Serial.println(sensor_data); //output data to serial interface to double check the cloud

  if (sensor_data > 750) //set your alarm threshold here
  {
    // note the .sendMessage function accepts a char array and not a string
    HologramCloud.sendMessage(sensor_string.c_str(), "ALARM"); 
    Serial.println("Alarm set");
  }
  else
  {
    HologramCloud.sendMessage(sensor_string.c_str(), "SAFE_ZONE");
    Serial.println("Alarm not set");
  }

  Dash.deepSleepSec(45); //wait 45 seconds in deep sleep to reduce power consumption
}
