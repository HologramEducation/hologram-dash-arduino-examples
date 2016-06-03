/*
Note: This code is a demo for how to use the Hologram Dash and/or
Dash Pro with the Hologram Magnetic Relay Shield.

First, upload this user program to your Dash or Dash Pro. Then,
at any point in time, enter one of the command characters in
the serial monitor to open/close the corresponding relay
circuit.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Character Command
a Open all relay circuits
1 Open K1 relay circuit
2 Open K2 relay circuit
3 Open K3 relay circuit
4 Open K4 relay circuit

z Close all relay circuits
q close K1 relay circuit
w close K2 relay circuit
e Close K3 relay circuit
r Close K4 relay circuit

*/

#include <SPI.h>

#define SPI_SELECT()(SPI.beginTransaction(L07))
#define SPI_DESELECT()(SPI.endTransaction())
#define SPI_WRITE(data) SPI_SELECT(); SPI.transfer(data); SPI_DESELECT()
#define RELAYSHIELD_Close_ALL() SPI_WRITE(0b01010101)
#define RELAYSHIELD_Open_ALL()SPI_WRITE(0b10101010)
#define RELAYSHIELD_Close_K1()SPI_WRITE(0b00000001)
#define RELAYSHIELD_Open_K1() SPI_WRITE(0b00000010)
#define RELAYSHIELD_Close_K2()SPI_WRITE(0b00000100)
#define RELAYSHIELD_Open_K2() SPI_WRITE(0b00001000)
#define RELAYSHIELD_Close_K3()SPI_WRITE(0b00010000)
#define RELAYSHIELD_Open_K3() SPI_WRITE(0b00100000)
#define RELAYSHIELD_Close_K4()SPI_WRITE(0b01000000)
#define RELAYSHIELD_Open_K4() SPI_WRITE(0b10000000)

char currChar;

void setup() {
    SerialUSB.begin(9600); /* USB UART */
    Serial2.begin(9600); /* TTL UART */
    SerialUSB.println("Hologram Dash Magnetic Relay Shield Example Started!");
    SerialCloud.begin(115200); /* Hologram Cloud */
    SPI.begin();
}

void loop()
{
    if (SerialUSB.available())
    {
        switch(SerialUSB.read())
        {
            case 'a':
                RELAYSHIELD_Open_ALL();
                SerialUSB.println("Open ALL");
                break;
            case '1':
                RELAYSHIELD_Open_K1();
                SerialUSB.println("Open K1");
                break;
            case '2':
                RELAYSHIELD_Open_K2();
                SerialUSB.println("Open K2");
                break;
            case '3':
                RELAYSHIELD_Open_K3();
                SerialUSB.println("Open K3");
                break;
            case '4':
                RELAYSHIELD_Open_K4();
                SerialUSB.println("Open K4");
                break;

            case 'z':
                RELAYSHIELD_Close_ALL();
                SerialUSB.println("Close ALL");
                break;
            case 'q':
                RELAYSHIELD_Close_K1();
                SerialUSB.println("Close K1");
                break;
            case 'w':
                RELAYSHIELD_Close_K2();
                SerialUSB.println("Close K2");
                break;
            case 'e':
                RELAYSHIELD_Close_K3();
                SerialUSB.println("Close K3");
                break;
            case 'r':
                RELAYSHIELD_Close_K4();
                SerialUSB.println("Close K4");
                break;
        }
    }

    while (Serial2.available()) {
        SerialCloud.write(Serial2.read());
    }

    while (SerialCloud.available()) {
        currChar = (char)SerialCloud.read();
        SerialUSB.write(currChar);
        Serial2.write(currChar);
    }

    delay(5);
}
