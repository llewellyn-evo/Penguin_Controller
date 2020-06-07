#include "controller.h"
#include "rf.h"
#include <timer.h>

Controller control;
auto timer = timer_create_default(); // create a timer with default settings

void setup() {
  InitHardware();
  InitXBEE(&control);
  //Print the callibration values here.
  ReadEEPROM(&control);
  if(control.RoverAddressH == 0xFFFFFFFF && control.RoverAddressL == 0xFFFFFFFF){
    control.EEPROMSuccess = 120;
  }else{
     control.EEPROMSuccess = 1;
  }
  timer.every(500, ToggleLED , &control);                 //toggle LED
  timer.every(600, VoltageMonitor , &control);                 //Voltage Monitor LED
  timer.every(50, ReadAllHardware ,  &control);           //Read Analog Values
  timer.every(100 , SendRFData , &control);               //Send RF Data
}


void loop() {
  timer.tick();                 // tick the timer
  XLoop();                      //Loop for Xbeee callbacks
}
