#include <RFM69.h>
#include <SPI.h>
#include <SPIFlash.h>
#include <WirelessHEX69.h>

#define MYID        1   // node ID used for this unit
#define TARGET_ID   11  // ID of node being wirelessly reprogrammed
#define NETWORKID   250
#define FREQUENCY   RF69_868MHZ // Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define LED         9
#define SERIAL_BAUD 115200
#define ACK_TIME    500  // # of ms to wait for an ack
#define TIMEOUT     3000
#define KEY         "thisIsEncryptKey"

RFM69 radio;
char c = 0;
char input[64]; //serial input buffer
bool promiscuousMode = false; //set to 'true' to sniff all packets on the same network

void setup(){
  Serial.begin(SERIAL_BAUD);
  //delay(10);
  radio.initialize(FREQUENCY,MYID,NETWORKID);
  //radio.promiscuous(promiscuousMode);
  Serial.print("Start wireless gateway...");
}

void loop(){
  if (radio.receiveDone())
  {
    //Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");
    for (byte i = 0; i < radio.DATALEN; i++)
      Serial.print((char)radio.DATA[i]);
    Serial.print("   [RX_RSSI:");Serial.print(radio.readRSSI());Serial.print("]");
    Serial.println();
    
    if (radio.ACK_REQUESTED)
    {
      radio.sendACK();
      Serial.print(" - ACK sent");
    }
    
    Serial.println();
  }
  Blink(LED,5); //heartbeat
}

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}
