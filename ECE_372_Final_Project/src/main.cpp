#include <Arduino.h>
#include <stdio.h>
#include "lcd.h"
#include "timer.h"
#include "i2c.h"
#include <avr/io.h>
#include <string.h>


//Fix this!!
//#define SLA 0x11 //Lidar device address//
#define PWR_MGMT 0x6B
#define WAKEUP 0x00
//#define SL_INTERFERENCE 0x22


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
  initTimer1();
  initLCD();
  //TODO: initialize Lidar???

  Serial.begin(9600); //baud rate
  sei();
  //initI2C();

  //variable to hold whether the lidar senses something
  //unsigned int interference = 0;   //0 means nothing sensed

  // StartI2C_Trans(SLA);
  // Write(PWR_MGMT);
  // Write(WAKEUP);

  moveCursor(0,0);
  writeString("Fastest Time:");
  char times[50];
  double time = 14.2343543;
  
  dtostrf(time, 4, 3, times);
  //sprintf(times, "%f", time);
  Serial.print(times);

  moveCursor(1,0);
  writeString(times);

  double fastest_time = 25.7;
  double curr_time = 0.0;

  while(1){
    //delayMs(1000);

    //Read_from(SLA, SL_INTERFERENCE);
    //interference = Read_data();

    switch(state){
      case wait_start:
        break;
      case pass_start:
        delayMs(3000);
        state = wait_finish;
        break;
      case wait_finish:
        break;
      case pass_finish:
        if(curr_time > fastest_time){
          fastest_time = curr_time;
          //convert fastestTime to String
          char *timeString;
          sprintf(timeString, "%f", fastest_time);
          moveCursor(0,1);
          writeString(timeString);
          moveCursor(8,1);
          writeString("s");
        }
        delayMs(3000);
        state = wait_start;
        break;
    }

    //StopI2C_Trans();
  }

  return 0;
}
