#include <RFM69.h>
#include <SPI.h>
#include <SPIFlash.h>
#include <avr/wdt.h>
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

void setup(){
	Serial.begin(SERIAL_BAUD);
	radio.initialize(FREQUENCY,MYID,NETWORKID);
	//radio.encrypt(KEY); //OPTIONAL
	//radio.setHighPower(); //for RFM69HW only!
	
	char buff[50];
	sprintf(buff, "\nsetup transmitting at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
	Serial.println(buff);

	Serial.println("setup start wireless gateway...");
}

void loop(){
	if (Serial.available() > 0) {
		byte inputLen = readSerialLine(input);

		if (inputLen == 4 && input[0]=='F' && input[1]=='L' && input[2]=='X' && input[3]=='?') {
	    	bool hexok = CheckForSerialHEX((byte*)input, inputLen, radio, TARGET_ID, TIMEOUT, ACK_TIME, true);
	    	if (hexok) {
	    		Serial.println("hex ok");
	    	}
	    	else {
	    		Serial.println("hex fail");	
	    	}
		}
		else if ((inputLen == 1 || inputLen == 2) && input[0]=='r') {
			Serial.println("rebooting");
    		resetUsingWatchdog(true);
		}
		else if (inputLen>0) { //just echo back
			Serial.print("unknown command ");
			Serial.print(inputLen);
			Serial.println(" ");
			Serial.println(input);
		}
	}

	if (radio.receiveDone())
	{
		Serial.print("debug ");
		Serial.print(radio.SENDERID);
		Serial.print(" ");
		Serial.print(radio.readRSSI());
		Serial.print(" ");
		Serial.print(radio.DATALEN);
		Serial.print(" ");
		for (byte i = 0; i < radio.DATALEN; i++) {
			Serial.print((char)radio.DATA[i]);
		}
		//Serial.print("\"");
		Serial.println();

		// Send data to app
		Serial.print(radio.SENDERID);
		Serial.print(' ');
		for (byte i = 0; i < radio.DATALEN; i++) {
			Serial.print((char)radio.DATA[i]);
		}
		Serial.println();

		if (radio.ACK_REQUESTED)
		{
			radio.sendACK();
			Serial.println("ack sent");
		}
		Blink(LED,5); //heartbeat
	}
}

void Blink(byte PIN, int DELAY_MS)
{
	pinMode(PIN, OUTPUT);
	digitalWrite(PIN,HIGH);
	delay(DELAY_MS);
	digitalWrite(PIN,LOW);
}
