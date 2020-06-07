/*
 * board.h
 *
 *  Created on: Oct 22, 2018
 *      Author: llewellyn
 */

#ifndef SRC_CONTROLLER_H_
#define SRC_CONTROLLER_H_

#include "board.h"
#include <stdint.h>
#include <Arduino.h>
#include <XBee.h>

typedef struct{
  uint16_t ValueX ,ValueY;      //Values for Controll Joystick and Pot
  uint16_t ValueY2;
  uint8_t Switch , EStop;          //Values for Mode SW and ESTOP switch
  uint8_t S1 , S2 , S3 , S4;
  float Battery;                   // Main Battery Voltage and Joystick Voltage
  uint8_t Mode;                   //Mode of the Controller
  unsigned long LastReply;        //Last Reply time from Sonobot
  uint8_t RFData[12];              //Buffer to Send RF Data
  uint32_t RoverAddressH , RoverAddressL;
  uint8_t ResponseData[10];
  XBeeAddress64 ResponseID;
  uint8_t SendResponse;
  uint8_t EEPROMSuccess;
}Controller;



void InitHardware();
void GetCallibration(Controller *tm);
uint16_t ReadADC(uint8_t Pin);
bool ReadAllHardware(void *t);
bool ToggleLED(void *t);
bool SendRFData(void *t);
bool VoltageMonitor(void *t);
uint16_t compute(const uint8_t* buffer, uint16_t len);


extern void Transmit(uint64_t Address , uint8_t *Data , uint8_t Length);
extern void ReadEEPROM(Controller *tm);

#endif /* SRC_CONTROLLER_H_ */
