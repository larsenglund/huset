/*
 * Copyright (c) 2013 by Felix Rusu <felix@lowpowerlab.com>
 * Library for facilitating wireless programming using an RFM69 transceiver (get library at: https://github.com/LowPowerLab/RFM69)
 * and the SPI Flash memory library for arduino/moteino (get library at: http://github.com/LowPowerLab/SPIFlash)
 * DEPENDS ON the two libraries mentioned above
 * Install all three of these libraries in your Arduino/libraries folder ([Arduino > Preferences] for location of Arduino folder)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _WirelessHEX69_H_
#define _WirelessHEX69_H_
#define LED 9  //LED on digital pin 9

#ifndef DEFAULT_TIMEOUT
  #define DEFAULT_TIMEOUT 3000
#endif

#ifndef ACK_TIMEOUT
  #define ACK_TIMEOUT 20
#endif

#include <RFM69.h>
#include <SPIFlash.h>

//functions used in the REMOTE node
void CheckForWirelessHEX(RFM69 radio, SPIFlash flash, boolean DEBUG=false);
void resetUsingWatchdog(boolean DEBUG=false);
boolean HandleWirelessHEXData(RFM69 radio, byte remoteID, SPIFlash flash, boolean DEBUG=false);

//functions used in the MAIN node
boolean CheckForSerialHEX(byte* input, byte inputLen, RFM69 radio, byte targetID, uint16_t TIMEOUT=DEFAULT_TIMEOUT, uint16_t ACKTIMEOUT=ACK_TIMEOUT, boolean DEBUG=false);
boolean HandleSerialHandshake(RFM69 radio, byte targetID, boolean isEOF, uint16_t TIMEOUT=DEFAULT_TIMEOUT, uint16_t ACKTIMEOUT=ACK_TIMEOUT, boolean DEBUG=false);
boolean HandleSerialHEXData(RFM69 radio, byte targetID, uint16_t TIMEOUT=DEFAULT_TIMEOUT, uint16_t ACKTIMEOUT=ACK_TIMEOUT, boolean DEBUG=false);
boolean waitForAck(RFM69 radio, uint8_t fromNodeID, uint16_t ACKTIMEOUT=ACK_TIMEOUT);

byte validateHEXData(void* data, byte length);
byte prepareSendBuffer(char* hexdata, byte*buf, byte length, uint16_t seq);
boolean sendHEXPacket(RFM69 radio, byte remoteID, byte* sendBuf, byte hexDataLen, uint16_t seq, uint16_t TIMEOUT=DEFAULT_TIMEOUT, uint16_t ACKTIMEOUT=ACK_TIMEOUT, boolean DEBUG=false);
byte BYTEfromHEX(char MSB, char LSB);
byte readSerialLine(char* input, char endOfLineChar=10, byte maxLength=64, uint16_t timeout=1000);
void PrintHex83(byte* data, byte length);

#endif