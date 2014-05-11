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
#include <OneWire.h>
#include <DallasTemperature.h>
#include <dht.h>

#define MYID        11       // node ID used for this unit
#define NETWORKID   250
#define GATEWAYID   1
#define FREQUENCY   RF69_868MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define SERIAL_BAUD 115200
#define ACK_TIME    500  // # of ms to wait for an ack
#define KEY         "thisIsEncryptKey"
#define LED         9
#define ONE_WIRE_BUS 4
#define DHT11_1_PIN 5
 #define DHT11_2_PIN 6

long eventTimeout;
long TRANSMITPERIOD = 5000; //transmit a packet to gateway so often (in ms)
RFM69 radio;
char input = 0;
//long lastPeriod = -1;
char payload[61];
char floatBuffer1[8];
char floatBuffer2[8];
byte sendSize=0;

//////////////////////////////////////////
// flash(SPI_CS, MANUFACTURER_ID)
// SPI_CS          - CS pin attached to SPI flash chip (8 in case of Moteino)
// MANUFACTURER_ID - OPTIONAL, 0x1F44 for adesto(ex atmel) 4mbit flash
//                             0xEF30 for windbond 4mbit flash
//////////////////////////////////////////
SPIFlash flash(8, 0xEF30); //EF30 for windbond 4mbit flash, EF40 for 16/64mbit

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

dht DHT;

void setup(){
	wdt_enable(WDTO_8S);
	wdt_reset();
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
	
	// Start up the library
	Serial.println("DallasTemperature init");
	sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
	//sensors.setResolution(12);
	Serial.print("DallasTemperature device count: ");
	Serial.println(sensors.getDeviceCount());

	eventTimeout = (long)millis() + TRANSMITPERIOD;
}

void loop()
{
	wdt_reset();
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
	wdt_reset();

	// Check for existing RF data, potentially for a new sketch wireless upload
	// For this to work this check has to be done often enough to be
	// picked up when a GATEWAY is trying hard to reach this node for a new sketch wireless upload
	if (radio.receiveDone())
	{
		wdt_reset();
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
		wdt_reset();
		wdt_disable();
		CheckForWirelessHEX(radio, flash, true);
		wdt_enable(WDTO_8S);
		wdt_reset();
		if (radio.ACK_REQUESTED)
		{
			radio.sendACK();
			Serial.println("ACK sent");
			delay(10);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// Real sketch code here, let's blink the onboard LED every 0.5sec
	if ((long)millis() - eventTimeout >= 0) {
	//if ((long)(millis()/TRANSMITPERIOD) > lastPeriod)
	//{
		// DHT
		CheckDHT(DHT11_1_PIN);
		wdt_reset();
		CheckDHT(DHT11_2_PIN);
		wdt_reset();

		// DallasTemperature
		// call sensors.requestTemperatures() to issue a global temperature 
		// request to all devices on the bus
		Serial.print("Requesting temperatures from ");
		int num_devices = sensors.getDeviceCount();
		Serial.print(num_devices);
		Serial.print(" device(s) with resolution ");
		Serial.println(sensors.getResolution()); 
		sensors.requestTemperatures(); // Send the command to get temperatures
		Serial.println("DONE");
		wdt_reset();

		uint8_t addr;
		float temp;
		for (int n=0; n<num_devices; n++) {
			Serial.print("Temperature for device with idx:");
			Serial.print(n);
			Serial.print(" addr:");
			sensors.getAddress(&addr, n);
			Serial.print(addr);
			Serial.print(" is: ");
			temp = sensors.getTempCByIndex(n);
			Serial.println(temp); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
			ftoa(&floatBuffer1[0], temp, 2);
			wdt_reset();
			//Serial.print("floatBuffer1 (");
			//Serial.print(strlen(floatBuffer1));
			//Serial.print(") ");
			//Serial.println(floatBuffer1);
			sprintf(payload, "ds18x20 %d %d %s\n", ONE_WIRE_BUS, addr, &floatBuffer1[0]);
			SendPayload(GATEWAYID, payload);
			wdt_reset();
		}

		// RFM69
		//int temp = random(-100,300);
		//sprintf(payload, "ds18x20 %d.%d\n", temp/10, temp - (10*(temp/10)));
		byte temperature =  radio.readTemperature(-2);
		sprintf(payload, "radio %d %ul\n", temperature, millis());
		SendPayload(GATEWAYID, payload);
		wdt_reset();

		eventTimeout = (long)millis() + TRANSMITPERIOD;
		//lastPeriod++;
		//pinMode(LED, OUTPUT);
		//digitalWrite(LED, lastPeriod%2);
		Blink(LED,5); //heartbeat
	}
	////////////////////////////////////////////////////////////////////////////////////////////
}

void CheckDHT(byte pin) 
{
	Serial.print("DHT11 pin:");
	Serial.print(pin);
	int chk = DHT.read11(pin);
	wdt_reset();
	switch (chk)
	{
		case DHTLIB_OK:  
			Serial.print(" OK "); 
			break;
		case DHTLIB_ERROR_CHECKSUM: 
			Serial.print(" Checksum error "); 
			break;
		case DHTLIB_ERROR_TIMEOUT: 
			Serial.print(" Timeout error "); 
			break;
		default: 
			Serial.print(" Unknown error "); 
			break;
	}
	if (chk == DHTLIB_OK) {
		Serial.print(" temperature:");
		Serial.print(DHT.temperature,1);
		Serial.print(" humidity:");
		Serial.print(DHT.humidity,1);
		Serial.println();
		ftoa(&floatBuffer1[0], DHT.temperature, 1);
		ftoa(&floatBuffer2[0], DHT.humidity, 1);
		sprintf(payload, "dht11 %d %s %s\n", pin, &floatBuffer1[0], &floatBuffer2[0]);
		SendPayload(GATEWAYID, payload);
		wdt_reset();
	}
}

void SendPayload(byte to_address, const void* buffer) 
{
	sendSize = strlen(payload);
	Serial.print("Sending[");
	Serial.print(sendSize);
	Serial.print("]:[");
	for(byte i = 0; i < sendSize; i++){
		Serial.print((char)payload[i]);
	}
	Serial.print("]");
	if (radio.sendWithRetry(GATEWAYID, payload, sendSize)){
		Serial.print(" ok!");
	}
	else {
		Serial.print(" nothing...");
	}
	wdt_reset();
	Serial.println();
}

void Blink(byte PIN, int DELAY_MS)
{
	pinMode(PIN, OUTPUT);
	digitalWrite(PIN,HIGH);
	delay(DELAY_MS);
	digitalWrite(PIN,LOW);
}

char *ftoa(char *a, float f, int precision)
{
	int p[] = {0,10,100};

	char *ret = a;
	int heiltal = (int)f;
	//Serial.println(heiltal);
	itoa(heiltal, a, 10);
	while (*a != '\0') a++;
	*a++ = '.';
	//Serial.print(f - (float)heiltal);
	int desimal = abs((int)((f - heiltal) * p[precision]));
	//Serial.println(desimal);
	if (desimal < p[precision]/10 -1) {
		*a++ = '0';
	}
	itoa(desimal, a, 10);
	return ret;
}