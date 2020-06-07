/*
 * board.h
 *
 *  Created on: Oct 22, 2018
 *      Author: llewellyn
 */

#ifndef SRC_RF_H_
#define SRC_RF_H_

#include <XBee.h>
#include <EEPROM.h>
#include "controller.h"

#define RESETCONTROLLER       0xCC
#define GETROVERADDRESS       0xAB
#define SETROVERADDRESS       0xAC

#define StartAddress          0x10

void InitXBEE(Controller *tm);
void Receive(ZBRxResponse& rx, uintptr_t t);
void Transmit(uint64_t Address , uint8_t *Data , uint8_t Length);
void XLoop();
void XReset();
void UpdateEEPROM(Controller *tm);
void ReadEEPROM(Controller *tm);


#endif /* SRC_RF_H_ */
