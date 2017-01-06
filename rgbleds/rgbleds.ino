/* rgbleds.ino
 *
 * This is a sketch for the Hologram dash that receives a color name
 * or list of names via SMS and then controls an RGB LED strip to
 * display that color.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


int pins[3] = {L09, L08, L07}; //R, G, B
int curColVal[3] = {0,0,0};
int val;
int colorStep = 4;
int timeBetweenSteps = 100;
int nextColorStep = 0;
int curColors[10];
int curColorIdx = 0;
String tempBuffer = "";
String payload = "";
bool foundSMS = false;

#define NUMCOLORS 20
String colors[] =
  {"BLACK", "RED", "GREEN", "BLUE", "WHITE",
   "PURPLE", "YELLOW", "CYAN", "PINK", "BT",
   "INDIGO", "NAVY", "TEAL", "TURQUOISE",
   "CHARTREUSE", "GOLD", "ORANGE", "BROWN", "MAROON", "GRAY"};

int colorVals[][3] =
  { { 0, 0, 0 }, //Black
    { 255, 0, 0 }, //Red
    { 0, 255, 0 }, //Green
    { 0, 0, 255 }, //Blue
    { 255, 255, 255}, //White
    { 255, 0, 255}, //Purple
    { 255, 255, 0 }, //Yellow
    { 0, 255, 255}, // Cyan
    { 255, 181, 197}, // Pink
    { 0, 255, 204}, //bright Turquoise
    { 75, 0, 130}, //indigo
    { 0, 0, 128}, //navy
    { 0, 128, 128}, //teal
    { 64, 224, 208}, //turquoise
    { 127, 255, 0}, //chartreuse
    { 238, 201, 0}, //gold
    { 255, 128, 0}, //orange
    { 165, 42, 42}, //brown
    { 128, 0, 0}, //maroon
    { 128, 128, 128} //gray    
    };

void setup()
{
  for(int i = 0; i < 3; ++i) {
    pinMode(pins[i], OUTPUT);
  }
  resetColorList();
  SerialUSB.begin(9600);
  SerialCloud.begin(115200);
  delay(4000);
  
  Dash.begin();
  Dash.pulseLED(100,5000);
}

int findColor(String color)
{
  SerialUSB.println(color);
  
  int i;
  for (i = 0; i < NUMCOLORS; ++i) {
    if (color == colors[i]) {
      SerialUSB.println("Color found");
      return i;
    }
  }
  return -1;
}

void resetColorList()
{
  curColors[0] = 0;
  for(int i = 1; i < 10; ++i) {
    curColors[i] = -1;
  }
  curColorIdx = 0;
  colorStep = 4;
  timeBetweenSteps = 100;
}

void doPartyMode()
{
  SerialUSB.println("PARTY!");
  for(int i = 0; i < 10; ++i) {
    int randcolor = random(1, NUMCOLORS);
    curColors[i] = randcolor;
  }
  curColorIdx = 0;
  colorStep = 255;
  timeBetweenSteps = 150;
}

void parseColorString(String sms)
{
  resetColorList();
  sms.toUpperCase();

  int numcolors = 0;
  int startidx = 0;
  while(startidx < sms.length() && numcolors < 10) {
    int endidx = sms.indexOf(' ', startidx);
    if(endidx == -1) {
      endidx = sms.length();
    }
    String colorStr = sms.substring(startidx, endidx);
    SerialUSB.println(colorStr);
    if(colorStr == "PARTY") {
      doPartyMode();
      return;
    } else if(colorStr == "BLINK") {
      colorStep = 255;
      timeBetweenSteps = 500;
    } else {
      int curColor = findColor(colorStr);
      if(curColor >= 0) {
        curColors[numcolors] = curColor;
        ++numcolors;
      }
    }
    startidx = endidx + 1;
  }
}


void loop()
{
  char currChar;

  while (SerialCloud.available()) {

    currChar = (char)SerialCloud.read();

    // check if the current buffer hits the SMSRCVD code.
    if (!foundSMS) {

      if (tempBuffer == "SMSRCVD") {
        foundSMS = true;
      }
    }
    // If it received the SMSRCVD code, the payload will get populated until
    // we get a \n.
    else if (currChar == '\n'){

      SerialUSB.println("\nSMS received: ");

      payload = stripOffLengthNumber(payload);
      payload.trim();

      SerialUSB.println(payload);

      parseColorString(payload);

      // reset foundSMS and the payload for the next iteration.
      foundSMS = false;
      payload = "";
    }
    else {
      payload.concat(currChar);
    }

    // Only keep a sliding buffer length of size 7
    // (need to only check for SMSRCVD).
    if (tempBuffer.length() >= 7) {
       tempBuffer.remove(0, 1);
    }

    // add latest char to our buffer.
    if (currChar != '\0') {
      tempBuffer.concat(currChar); 
    }
    
    SerialUSB.write(currChar);
  }

  if(nextColorStep < millis()) {
    nextColorStep = millis() + timeBetweenSteps;
    int curColor = curColors[curColorIdx];
    if(curColor >= 0) {
      // Check if all colors are where they should be
      bool colorsSet = true;
      for(int i = 0; i < 3; ++i) {
        if(curColVal[i] != colorVals[curColor][i]) {
          colorsSet = false;
          break;
        }
      }
      
      if(colorsSet) {
        ++curColorIdx;
        if(curColorIdx >= 10 || curColors[curColorIdx] == -1) {
          curColorIdx = 0;
        }
      } else {
        for(int i = 0; i < 3; ++i) {
          if(curColVal[i] != colorVals[curColor][i]) {
            int remaining = colorVals[curColor][i] - curColVal[i];
            if(remaining < 0) {
              curColVal[i] -= min((-1*remaining), colorStep);
            } else {
              curColVal[i] += min(remaining, colorStep);
            }
            analogWrite(pins[i], curColVal[i]);
          }
        }
      }
    }
  }
  
  delay(40);
}

String stripOffLengthNumber(String payload)
{

  int index = 0;
  while (payload[index] == ',') {
    ++index;
  }
  while (payload[index] != ',') {
    ++index;
  }
  ++index;

  payload.remove(0, index);

  return payload;
}
