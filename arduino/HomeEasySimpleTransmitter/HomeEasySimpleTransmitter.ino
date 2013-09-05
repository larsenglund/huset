/*
 * AM-HRR3 transmitter test.
 * Connect two push switches up to onPin and offPin, pulled high.
 *
 * Emits a homeeasy simple protocol ON message when the onPin is brought low,
 * and vice versa for offPin.
 *
 * Barnaby Gray 12/2008.
 */

int txPin = 11;
//int onPin = 12;
//int offPin = 11;
int ledPin = 13;
int inByte = 0;         // incoming serial byte

void setup()
{
  pinMode(txPin, OUTPUT);
  //pinMode(onPin, INPUT);
  //pinMode(offPin, INPUT);

  Serial.begin(9600);
  Serial.println("HomeEasy Simple Protocol Transmitter test");
}

void sendBit(boolean b) {
  if (b) {
    digitalWrite(txPin, HIGH);
    delayMicroseconds(1125);
    digitalWrite(txPin, LOW);
    delayMicroseconds(375);
  }
  else {
    digitalWrite(txPin, HIGH);
    delayMicroseconds(375);
    digitalWrite(txPin, LOW);
    delayMicroseconds(1125);
  }
}

void sendPair(boolean b) {
  sendBit(false);
  sendBit(b);
}

void switchcode(boolean b)
{
// group: P (15)  unit+1: 13 cmd: ON (14) 
// group: P (15)  unit+1: 13 cmd: OFF (6)
 
  // house code 1 = B
  sendPair(true);
  sendPair(true);
  sendPair(true);
  sendPair(true);

  // unit code 2
  sendPair(false);
  sendPair(false);
  sendPair(true);
  sendPair(true);

  // on = 14
  sendPair(false);
  sendPair(true);
  sendPair(true);
  sendPair(b);

  sendBit(false);  
}

void transmit(boolean b) {
  switchcode(b);
  delayMicroseconds(10000);
  switchcode(b);
  delayMicroseconds(10000);
  switchcode(b);
  delayMicroseconds(10000);
  switchcode(b);
}

void loop() {
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    if (inByte == '1') {
      Serial.println("Switching on");
      transmit(true);
      //transmit(true);
      digitalWrite(ledPin, HIGH);
    }
    else {
      Serial.println("Switching off");
      transmit(false);
      //transmit(false);
      digitalWrite(ledPin, LOW);
    }
  }
/*  int val = digitalRead(onPin);
  if (val == LOW) {
    transmit(true);
  }
  val = digitalRead(offPin);
  if (val == LOW) {
    transmit(false);
  }*/
}
