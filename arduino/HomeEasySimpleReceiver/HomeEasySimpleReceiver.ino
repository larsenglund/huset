/*
 * AM-HRR3 receiver for homeeasy simple protocol
 * This handles the simple homeeasy protocol compatible with the domia lite
 * units.
 *
 * This is emitted by the HE200 remote control.
 *
 * The protocol consists of house code A-P and unit number 1-16. The messages received
 * by the arduino are emitted out on the serial link, one message per line.
 *
 * See the Technical forum on:
 * http://www.byebyestandby.com/forum/
 * Particularly the pdf for the Protocol Specification.
 *
 * Barnaby Gray 12/2008
 */

int ledPin = 13;
int rxPin = 12;

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(rxPin, INPUT);

  Serial.begin(9600);
  Serial.println("HomeEasy Simple Protocol Receiver test");
}

boolean readBit()
{
  unsigned long t;
  do {
    t = pulseIn(rxPin, HIGH);
  } while (t < 300 || t > 1500);

  return t > 750;
}

void latch()
{
  boolean b = readBit();
  while (!b) {
    b = readBit();
  }
}

void loop()
{
  int i = 0;
  unsigned long d = 0;
  unsigned int data = 0;
  unsigned long t;

  while (i < 25) {
    // wait for first pulse to latch
    t = pulseIn(rxPin, HIGH, 1500);
    // pulses are expected to be either 375us or 1125us, but there's
    // some tolerance here. 
    if (t < 250 || t > 1250) {
      // pulse timing off or timeout - reset
      i = 0;
      data = 0;
      continue;
    }

    if (i % 2 == 0) {
      if (t > 400) {
        // should be zero pulses
        i = 0; data = 0;
        continue;
      }
    }
    else {
      data = (data>>1) + (t > 400 ? 0x800 : 0);
    }

    ++i;
  }

  int group = data & 15;
  int unit = (data >> 4) & 15;
  int cmd = (data >> 8) & 15;

  Serial.print(" group: ");
  Serial.write(group+65);
  Serial.print(" (");
  Serial.print(group);
  Serial.print(") ");
  Serial.print(" unit+1: ");
  Serial.print(unit+1);
  Serial.print(" cmd: ");
  Serial.print((cmd == 14 ? "ON" : (cmd == 6 ? "OFF" : "UNK")));
  Serial.print(" (");
  Serial.print(cmd);
  Serial.print(") ");
  Serial.println();  

}
