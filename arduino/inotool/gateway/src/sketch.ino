/*
 * This sketch demonstrates how to use InterruptChain to receive and
 * decode remote switches (old and new) and remote sensors.
 *
 * Basically, this sketch combines the features of the ShowReceivedCode
 * and ShowReceivedCodeNewKaku examples of RemoteSwitch and the
 * ThermoHygroReceiver of RemoteSensor all at the same time!
 *
 * After uploading, enable the serial monitor at 115200 baud.
 * When you press buttons on a 433MHz remote control, as supported by 
 * RemoteSwitch or NewRemoteSwitch, the code will be echoed.
 * At the same time, if data of a remote thermo/hygro-sensor is
 * received, as supported by RemoteSensor, it will be echoed as well.
 *
 * Setup:
 * - connect a 433MHz receiver on digital pin 2.
 */

#include <RemoteReceiver.h>
#include <RemoteTransmitter.h>
#include <NewRemoteReceiver.h>
#include <NewRemoteTransmitter.h>
#include <SensorReceiver.h>
#include <InterruptChain.h>

#define APP_VERSION "1.0.0"
int inByte = 0;         // incoming serial byte
// Buffer to store incoming commands from serial port
//String inData;
#define IN_DATA_MAX_LEN 100
char inData[IN_DATA_MAX_LEN];
int inDataIdx = 0;
char *p1,*p2,*p3,*p4,*p5,*p6,*p7;
//filenameString.toCharArray(filename, 100);


// shows the received code sent from an old-style remote switch
void showOldCode(unsigned long receivedCode, unsigned int period) {
  // Print the received code.
/*  Serial.print("Code: ");
  Serial.print(receivedCode);
  Serial.print(", period: ");
  Serial.print(period);
  Serial.println("us.");*/
  Serial.print("433 old ");
  Serial.print(period);
  Serial.print(" ");
  Serial.print(receivedCode);
  Serial.print("\n");
}

// Shows the received code sent from an new-style remote switch
void handleNewCode(NewRemoteCode receivedCode) {
  // Print the received code.
  /*Serial.print("Addr ");
  Serial.print(receivedCode.address);

  if (receivedCode.groupBit) {
    Serial.print(" group");
  } 
  else {
    Serial.print(" unit ");
    Serial.print(receivedCode.unit);
  }

  switch (receivedCode.switchType) {
    case NewRemoteCode::off:
    Serial.print(" off");
    break;
    case NewRemoteCode::on:
    Serial.print(" on");
    break;
    case NewRemoteCode::dim:
    Serial.print(" dim level ");
    Serial.print(receivedCode.dimLevel);
    break;
    case NewRemoteCode::on_with_dim:
    Serial.print(" on with dim level ");
    Serial.print(receivedCode.dimLevel);
    break;
  }

  Serial.print(", period: ");
  Serial.print(receivedCode.period);
  Serial.println("us.");*/
  Serial.print("433 new ");
  Serial.print(receivedCode.period);
  Serial.print(" ");
  Serial.print(receivedCode.address);
  Serial.print(" ");
  if (receivedCode.groupBit) {
    Serial.print("0");
  } 
  else {
    Serial.print(receivedCode.unit);
  }
  Serial.print(" ");
  switch (receivedCode.switchType) {
    case NewRemoteCode::off:
      Serial.print("off");
      break;
    case NewRemoteCode::on:
      Serial.print("on");
      break;
    case NewRemoteCode::dim:
      Serial.print("dim ");
      Serial.print(receivedCode.dimLevel);
      break;
    case NewRemoteCode::on_with_dim:
      Serial.print("on ");
      Serial.print(receivedCode.dimLevel);
      break;  
  }
  Serial.print("\n");
}

// Shows the received sensor data
void showTempHumi(byte *data) {
  // is data a ThermoHygro-device?
  if ((data[3] & 0x1f) == 0x1e) {
    // Yes!
    byte channel, randomId;
    int temp;
    byte humidity;

    // Decode the data
    SensorReceiver::decodeThermoHygro(data, channel, randomId, temp, humidity);

    // Print temperature. Note: temp is 10x the actual temperature!
    Serial.print("Temperature: ");
    Serial.print(temp / 10); // units
    Serial.print('.');
    Serial.println(temp % 10); // decimal
  }
}


void setup() {
  Serial.begin(115200);
  Serial.print("app_version ");
  Serial.println(APP_VERSION);

  // Interrupt -1 to indicate you will call the interrupt handler with InterruptChain
  RemoteReceiver::init(-1, 2, showOldCode);

  // Again, interrupt -1 to indicate you will call the interrupt handler with InterruptChain
  NewRemoteReceiver::init(-1, 2, handleNewCode);

  // And once more, interrupt -1 to indicate you will call the interrupt handler with InterruptChain
  SensorReceiver::init(-1, showTempHumi);

  // On interrupt, call the interrupt handlers of remote and sensor receivers
  InterruptChain::addInterruptCallback(0, RemoteReceiver::interruptHandler);
  InterruptChain::addInterruptCallback(0, NewRemoteReceiver::interruptHandler);
  InterruptChain::addInterruptCallback(0, SensorReceiver::interruptHandler);
}

void loop() 
{
  // You can do other stuff here!
  if (Serial.available() > 0) 
  {
    inByte = Serial.read();
    if (inByte != '\n') 
    {
      inData[inDataIdx++] = inByte;
      //Serial.print(inByte);
      //Serial.print(" ");
      //Serial.println(inDataIdx);
    }
    else 
    {
      // Process message when new line character is recieved

      inData[inDataIdx] = '\0';
      Serial.print("Arduino Received: ");
      Serial.println(inData);
      Serial.print(inDataIdx);
      Serial.println(" bytes");

      p1 = strtok(inData, ": "); // split by ":" or blank space " "

      // Handle 433MHz commands
      if (strcmp(p1, "433") == 0) 
      {
        Serial.print("OK 433 ");
        RemoteReceiver::disable();
        NewRemoteReceiver::disable();
        interrupts();

        p2 = strtok(NULL, ":. ");
        p3 = strtok(NULL, ":. ");
        unsigned int period = strtoul(p3, NULL, 0);

        if (strcmp(p2, "new") == 0) 
        {
          Serial.print("new ");
          Serial.print(period);
          Serial.print(" ");
          
          p4 = strtok(NULL, ":. ");
          unsigned long addr = strtoul(p4, NULL, 0);
          Serial.print(addr);
          Serial.print(" ");
          NewRemoteTransmitter transmitter(addr, 11, period);
          
          p5 = strtok(NULL, ":. ");
          byte unit = atoi(p5);
          Serial.print(unit);
          Serial.print(" ");

          p6 = strtok(NULL, ":. ");
          Serial.print(p6);
          Serial.print(" ");
          if (strcmp(p6, "on") == 0) 
          {
            Serial.print("On! ");
            transmitter.sendUnit(unit, true);
          }
          else if (strcmp(p6, "off") == 0) 
          {
            Serial.print("Off! ");
            transmitter.sendUnit(unit, false);
          }
          else if (strcmp(p6, "dim") == 0) 
          {
            p7 = strtok(NULL, ":. ");
            byte level = atoi(p5);
            Serial.print(level);
            Serial.print(" ");
            transmitter.sendDim(unit, level);
          }
          Serial.print("\n");
        }
        else if (strcmp(p2, "old") == 0)
        {
          Serial.print("old ");
          Serial.print(period);
          Serial.print(" ");
          
          p4 = strtok(NULL, ":. ");
          unsigned long receivedCode = strtoul(p4, NULL, 0);
          Serial.println(receivedCode);
          RemoteTransmitter::sendCode(11, receivedCode, period, 3);
        }
        RemoteReceiver::enable();
        NewRemoteReceiver::enable();
      }
      inDataIdx = 0; // Clear recieved buffer
    }

    // get incoming byte:
    /*inByte = Serial.read();
     if (inByte == '1' || inByte == '2') {
     NewRemoteTransmitter transmitter(55371774, 11, 262);
     switch (inByte) {
     case '1':
     transmitter.sendUnit(14, true);
     break;
     case '2':
     transmitter.sendUnit(14, false);
     break;
     }
     }
     else {
     switch (inByte) {
     case '3':
     RemoteTransmitter::sendCode(11, 525554, 292, 3);
     break;
     case '4':
     RemoteTransmitter::sendCode(11, 525552, 292, 3);
     break;
     }
     }*/
  }
}
