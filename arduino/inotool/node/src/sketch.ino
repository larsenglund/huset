
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

#include <dht.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define MYID        55       // node ID used for this unit
#define NETWORKID   250
#define FREQUENCY   RF69_868MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define SERIAL_BAUD 115200
#define ACK_TIME    100  // # of ms to wait for an ack
#define KEY         "thisIsEncryptKey"

#define DHT11_1_PIN 5
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 4

RFM69 radio;
char input = 0;
long lastPeriod = -1;

dht DHT;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);


//////////////////////////////////////////
// flash(SPI_CS, MANUFACTURER_ID)
// SPI_CS          - CS pin attached to SPI flash chip (8 in case of Moteino)
// MANUFACTURER_ID - OPTIONAL, 0x1F44 for adesto(ex atmel) 4mbit flash
//                             0xEF30 for windbond 4mbit flash
//////////////////////////////////////////
SPIFlash flash(8, 0xEF40); //EF30 for windbond 4mbit flash, EF40 for 16/64mbit

void setup(){
  Serial.begin(SERIAL_BAUD);
  Serial.print("Initializing radio...");
  radio.initialize(FREQUENCY,MYID,NETWORKID);
  radio.encrypt(KEY); //OPTIONAL
  //radio.setHighPower(); //for RFM69HW only!
  Serial.print("Start node...");
  
  if (flash.initialize())
    Serial.println("SPI Flash Init OK!");
  else
    Serial.println("SPI Flash Init FAIL!");

  Serial.print("DHT LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  
  // Start up the library
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
  //sensors.setResolution(12);
  //Serial.println(sensors.getDeviceCount());
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
    for (byte i = 0; i < radio.DATALEN; i++)
      Serial.print((char)radio.DATA[i], HEX);
    Serial.println();
    //CheckForWirelessHEX(radio, flash, true);
    Serial.println();
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Real sketch code here, let's blink the onboard LED every 0.5sec
  if ((int)(millis()/2000) > lastPeriod)
  {
    lastPeriod++;
    pinMode(9, OUTPUT);
    digitalWrite(9, lastPeriod%2);
   Serial.println(lastPeriod);
  
  // DHT11
  // READ DATA
  Serial.print("dht11 ");
  Serial.print(DHT11_1_PIN);
  int chk = DHT.read11(DHT11_1_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
                //Serial.print("OK,\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Time out error,\t"); 
                break;
    default: 
                Serial.print("Unknown error,\t"); 
                break;
  }
  // DISPLAY DATA
  Serial.println(DHT.temperature,1);
  Serial.print(DHT.humidity,1);
  
  // DS18X20
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  Serial.print(sensors.getDeviceCount());
  Serial.print(" ");
  Serial.println(sensors.getResolution()); 
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  Serial.print("Temperature for Device 1 is: ");
  Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire

  }
  ////////////////////////////////////////////////////////////////////////////////////////////
}