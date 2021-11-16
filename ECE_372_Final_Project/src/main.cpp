#include <Arduino.h>
#include "lcd.h"
#include "timer.h"
#include "i2c.h"


#define SLA 0x## //Lidar device address
#define PWR_MGMT 0x6B
#define WAKEUP 0x00
#define SL_INTERFERENCE 0x##


//Define set of states for lap state
typedef enum stateEnum{
  wait_start,
  pass_start,
  wait_finish,
  pass_finish
} stateType;

volatile stateType state = wait_start;


int main(){
  //initialize everything
  initTimer0();
  initLCD();
  //TODO: initialize Lidar???

  Serial.begin(9600); //baud rate
  sei();
  initI2C();

  //variable to hold whether the lidar senses something
  unsigned int interference = 0;   //0 means nothing sensed

  StartI2C_Trans(SLA);
  Write(PWR_MGMT);
  Write(WAKEUP);



  moveCursor(0,0);
  writeString("Fastest Time:");

  while(1){
    delayMs(1000);

    Read_from(SLA, SL_INTERFERENCE);
    interference = Read_data();

    switch(state){
      case wait_start:
        break;
      case pass_start:
        state = wait_finish;
        break;
      case wait_finish:
        break;
      case pass_finish:
        state = wait_start;
        break;
    }

    StopI2C_Trans();
  }

  return 0;
}
