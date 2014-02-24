/*
 * Copyright (c) 2013 by Felix Rusu <felix@lowpowerlab.com>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

// This sketch is an example of how wireless programming can be achieved with a Moteino
// that was loaded with a custom 1k Optiboot that is capable of loading a new sketch from
// an external SPI flash chip
// The sketch includes logic to receive the new sketch 'over-the-air' and store it in
// the FLASH chip, then restart the Moteino so the bootloader can continue the job of
// actually reflashing the internal flash memory from the external FLASH memory chip flash image
// The handshake protocol that receives the sketch wirelessly by means of the RFM69 radio
// is handled by the SPIFLash/WirelessHEX69 library, which also relies on the RFM69 library
// These libraries and custom 1k Optiboot bootloader are at: http://github.com/lowpowerlab

#include <RFM69.h>
#include <SPI.h>
#include <SPIFlash.h>
#include <avr/wdt.h>
#include <WirelessHEX69.h>

#define MYID        11       // node ID used for this unit
#define NETWORKID   250
#define GATEWAYID   1
#define FREQUENCY   RF69_868MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define SERIAL_BAUD 115200
#define ACK_TIME    50  // # of ms to wait for an ack
#define KEY         "thisIsEncryptKey"
#define LED         9

int TRANSMITPERIOD = 3000; //transmit a packet to gateway so often (in ms)
RFM69 radio;
char input = 0;
long lastPeriod = -1;
char payload[61];
byte sendSize=0;

//////////////////////////////////////////
// flash(SPI_CS, MANUFACTURER_ID)
// SPI_CS          - CS pin attached to SPI flash chip (8 in case of Moteino)
// MANUFACTURER_ID - OPTIONAL, 0x1F44 for adesto(ex atmel) 4mbit flash
//                             0xEF30 for windbond 4mbit flash
//////////////////////////////////////////
SPIFlash flash(8, 0xEF30); //EF30 for windbond 4mbit flash, EF40 for 16/64mbit

void setup(){
  Serial.begin(SERIAL_BAUD);
  Serial.println("Start node...");

  radio.initialize(FREQUENCY,MYID,NETWORKID);
  //radio.encrypt(KEY); //OPTIONAL
  //radio.setHighPower(); //for RFM69HW only!
  char buff[50];
  sprintf(buff, "\nTransmitting at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Serial.println(buff);
  
  if (flash.initialize()) {
    Serial.println("SPI Flash Init OK!");
  }
  else {
    Serial.println("SPI Flash Init FAIL!");
  }
  Serial.println("Hello new code!");
}

void loop(){
  // This part is optional, useful for some debugging.
  // Handle serial input (to allow basic DEBUGGING of FLASH chip)
  // ie: display first 256 bytes in FLASH, erase chip, write bytes at first 10 positions, etc
  if (Serial.available() > 0) {
    input = Serial.read();
    if (input == 'd') //d=dump first page
    {
      Serial.println("Flash content:");
      int counter = 0;

      while(counter<=256){
        Serial.print(flash.readByte(counter++), HEX);
        Serial.print('.');
      }
      
      Serial.println();
    }
    else if (input == 'e')
    {
      Serial.print("Erasing Flash chip ... ");
      flash.chipErase();
      while(flash.busy());
      Serial.println("DONE");
    }
    else if (input == 'i')
    {
      Serial.print("DeviceID: ");
      Serial.println(flash.readDeviceId(), HEX);
    }
    else if (input == 'r')
    {
      Serial.print("Rebooting");
      resetUsingWatchdog(true);
    }
    else if (input == 'R')
    {
      Serial.print("RFM69 registers:");
      radio.readAllRegs();
    }
    else if (input >= 48 && input <= 57) //0-9
    {
      Serial.print("\nWriteByte("); Serial.print(input); Serial.print(")");
      flash.writeByte(input-48, millis()%2 ? 0xaa : 0xbb);
    }
  }
  
  // Check for existing RF data, potentially for a new sketch wireless upload
  // For this to work this check has to be done often enough to be
  // picked up when a GATEWAY is trying hard to reach this node for a new sketch wireless upload
  if (radio.receiveDone())
  {
    Serial.print("Got [");
    Serial.print(radio.SENDERID);
    Serial.print(':');
    Serial.print(radio.DATALEN);
    Serial.print("] > ");
    for (byte i = 0; i < radio.DATALEN; i++) {
      //Serial.print((char)radio.DATA[i], HEX);
    	Serial.print((char)radio.DATA[i]);
    }
    Serial.print("   [RX_RSSI:");
    Serial.print(radio.readRSSI());
    Serial.print("]");
    Serial.println();
    
    CheckForWirelessHEX(radio, flash, true);

	if (radio.ACK_REQUESTED)
    {
      radio.sendACK();
      Serial.print(" - ACK sent");
      delay(10);
    }

    Serial.println();
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Real sketch code here, let's blink the onboard LED every 0.5sec
  if ((int)(millis()/TRANSMITPERIOD) > lastPeriod)
  {
  	//int temp = random(-100,300);
  	//sprintf(payload, "ds18x20 %d.%d\n", temp/10, temp - (10*(temp/10)));
  	byte temperature =  radio.readTemperature(-1);
  	sprintf(payload, "radio %d.0 %d\n", temperature, millis());
  	sendSize = strlen(payload);
 	Serial.print("Sending[");
    Serial.print(sendSize);
    Serial.print("]: ");
    for(byte i = 0; i < sendSize; i++){
      Serial.print((char)payload[i]);
    }

    if (radio.sendWithRetry(GATEWAYID, payload, sendSize)){
    	Serial.print(" ok!");
    }
    else {
    	Serial.print(" nothing...");
    }
    lastPeriod++;
    pinMode(LED, OUTPUT);
    digitalWrite(LED, lastPeriod%2);
  }
  ////////////////////////////////////////////////////////////////////////////////////////////
}


char *ftoa(char *a, double f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
  
  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}