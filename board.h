/*
 * board.h
 *
 *  Created on: Oct 22, 2018
 *      Author: llewellyn
 */

#ifndef SRC_BOARD_H_
#define SRC_BOARD_H_

#define VERSION           1.0
#define ANALOG_SAMPLES    20

#define PACKETHEADER      0x3f
#define PROTOCOLID        0x01
#define MANUALCONTROL      0x01
#define STATUS            0x02
#define SONOBOTRESPONSE   0x03

#define XAXIS             A1
#define YAXIS             A0
#define YAXIS2            A2


#define VI                A4

#define XRST              A5
#define ALED              5
#define MLED              4


#define ESTOP             7
#define SW                8

#define SW1               6
#define SW2               3
#define SW3               10
#define SW4               2

#define AUTO              10
#define MANUAL            11

#define MAX               405

#endif /* SRC_BOARD_H_ */
