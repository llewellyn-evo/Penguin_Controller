
#include "rf.h"
#include <Printers.h>

XBeeWithCallbacks xbee;

void(* resetFunc) (void) = 0;//declare reset function at address 0

void InitXBEE(Controller *tm) {
  XReset();
  Serial.begin(38400);
  xbee.setSerial(Serial);
  xbee.onZBRxResponse(Receive , (uintptr_t)tm);
  //xbee.onPacketError(printErrorCb, (uintptr_t)(Print*)&mySerial);
}

//TODO: Add Commands to read Rover address and set Rover address in eeprom
void Receive(ZBRxResponse& rx, uintptr_t t) {
  uint8_t Data[12];
  uint16_t CRC = 0;
  Controller *tm = (Controller*)t;
  if (rx.getDataLength() < 12) {
    memset(Data , 0 , sizeof(Data));
    for (int i = 0; i < rx.getDataLength(); i++) {
      Data[i] = rx.getData()[i];
      //mySerial.println(Data[i] , HEX);
    }
    if (Data[0] == PACKETHEADER) {
      //CRC = CRC8.smbus(Data , rx.getDataLength() - 1);    //CRCs
      CRC = compute(Data,  rx.getDataLength() - 2 );
      //Process Data Here
      if (highByte(CRC) == Data[rx.getDataLength() - 2]   && lowByte(CRC) == Data[rx.getDataLength() - 2]) {
        if (Data[1] == PROTOCOLID && Data[2] == SONOBOTRESPONSE) {
          //Response from Sonobot
          tm->LastReply = millis();
        }
      }
    }else if(Data[0] == GETROVERADDRESS){
      tm->ResponseData[0] = GETROVERADDRESS;
      tm->ResponseData[1] = ((tm->RoverAddressH & 0xff000000) >> 24);
      tm->ResponseData[2] = ((tm->RoverAddressH & 0x00ff0000) >> 16);
      tm->ResponseData[3] = ((tm->RoverAddressH & 0x0000ff00) >> 8);
      tm->ResponseData[4] = (tm->RoverAddressH & 0x000000ff);
      tm->ResponseData[5] = ((tm->RoverAddressL & 0xff000000) >> 24);
      tm->ResponseData[6] = ((tm->RoverAddressL & 0x00ff0000) >> 16);
      tm->ResponseData[7] = ((tm->RoverAddressL & 0x0000ff00) >> 8);
      tm->ResponseData[8] = (tm->RoverAddressL & 0x000000ff);
      tm->ResponseID = rx.getRemoteAddress64() ;
      tm->SendResponse = 1;
    }else if(Data[0] == SETROVERADDRESS){
      tm->RoverAddressH = (uint32_t)Data[1]<< 24 | (uint32_t)Data[2] << 16 | (uint32_t)Data[3] << 8 | (uint32_t)Data[4];
      tm->RoverAddressL = (uint32_t)Data[5]<< 24 | (uint32_t)Data[6] << 16 | (uint32_t)Data[7] << 8 | (uint32_t)Data[8];
      UpdateEEPROM(tm);
    }else if(Data[0] == RESETCONTROLLER){
      resetFunc();
    }
  } else {
    //Dump All the Data
    for (int i = 0; i < rx.getDataLength(); i++) {
      rx.getData()[i];
    }
  }
}

void Transmit(uint64_t Address , uint8_t *Data , uint8_t Length) {
  ZBTxRequest tx;
  tx.setAddress64(Address);
  tx.setPayload(Data , Length);
  xbee.send(tx);
}

void XLoop() {
  xbee.loop();
}

void XReset() {
  pinMode(XRST , OUTPUT);
  digitalWrite(XRST , LOW);
  delay(500);
  pinMode(XRST , INPUT);
}

void UpdateEEPROM(Controller *tm){
  uint8_t Index = StartAddress;
  uint8_t value = 0;
  
  value = ((tm->RoverAddressH & 0xff000000) >> 24);
  EEPROM.update(StartAddress, value);
  value = ((tm->RoverAddressH & 0x00ff0000) >> 16);
  EEPROM.update(StartAddress + 1, value);
  value = ((tm->RoverAddressH & 0x0000ff00) >> 8);
  EEPROM.update(StartAddress + 2, value);
  value = (tm->RoverAddressH & 0x000000ff);
  EEPROM.update(StartAddress + 3, value);

  value = ((tm->RoverAddressL & 0xff000000) >> 24);
  EEPROM.update(StartAddress + 4, value);
  value = ((tm->RoverAddressL & 0x00ff0000) >> 16);
  EEPROM.update(StartAddress + 5, value);
  value = ((tm->RoverAddressL & 0x0000ff00) >> 8);
  EEPROM.update(StartAddress + 6, value);
  value = (tm->RoverAddressL & 0x000000ff);
  EEPROM.update(StartAddress + 7, value);
}

void ReadEEPROM(Controller *tm){
  tm->RoverAddressH = (uint32_t)EEPROM.read(StartAddress)<< 24 | (uint32_t)EEPROM.read(StartAddress + 1) << 16 | (uint32_t)EEPROM.read(StartAddress + 2)  << 8 | (uint32_t)EEPROM.read(StartAddress + 3) ;
  tm->RoverAddressL = (uint32_t)EEPROM.read(StartAddress + 4)<< 24 | (uint32_t)EEPROM.read(StartAddress + 5) << 16 | (uint32_t)EEPROM.read(StartAddress + 6) << 8 | (uint32_t)EEPROM.read(StartAddress + 7);
}
