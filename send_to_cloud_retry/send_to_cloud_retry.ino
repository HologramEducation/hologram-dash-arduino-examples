/*
 * send_to_cloud_retry.ino
 *
 * HologramCloud.sendMessage returns a boolean indicating if the send was
 * successful. We can use this to automatically retry failed writes to the cloud.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

bool sendWithRetry(const char* message, const char* tag, int attempts=3) {
  bool success = false;
  while (!success && attempts > 0) {
    success = HologramCloud.sendMessage(message, tag);
    --attempts;
    if (!success) {
      Serial.print("Failed to send. Remaining attempts: ");
      Serial.println(attempts);
    }
  }
  return success;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  HologramCloud.begin();
  if (HologramCloud.getConnectionStatus()) {
    Dash.onLED();
  }
}

void loop() {
  delay(5000);
  Serial.println("Sending message...");
  bool success = sendWithRetry("hi there!", "CUSTOMTAG", 5);
  if (success) {
    Serial.println("Success");
  }
  else {
    Serial.println("Failed after 5 attempts");
  }
  Serial.print("Signal strength: ");
  Serial.println(HologramCloud.getSignalStrength());
}

