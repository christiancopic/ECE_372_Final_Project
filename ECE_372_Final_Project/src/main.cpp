#include <Arduino.h>
#include <stdio.h>
#include "lcd.h"
#include "timer.h"
#include <avr/io.h>
#include <string.h>
#include "spi.h"
#include "switch.h"


#define echoPin 19   //attach pin D2 of Arduino to pin Echo of HC-SR04
#define trigPin 18   //attach pin D3 of Arduino to pin Trig of HC-SR04

double loopTime = 0.0;
double timeCount = 0.0;

long duration;
int distance;

long INIT_DISTANCE = 50;

float fastest_time = 1000.0;
long curr_time = 0;




//Define set of states for lap state
typedef enum stateEnum{
  wait_start,
  pass_start,
  wait_finish,
  pass_finish
} stateType;

volatile stateType state = wait_start;

//Define set of states for switch debouncing
typedef enum switchStateEnum{
    wait_press,
    bounce_low,
    wait_release,
    bounce_high
  } switchState;


volatile switchState switch_state = wait_press;

void initUSS(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}


int main(){
  //initialize everything
  initTimer0();
  initTimer1();
  initTimer3();
  initSPI();
  initLCD();
  initUSS();
  initSwitchPB7();
  


  Serial.begin(9600); //baud rate
  sei();

  delayMs(2000);
  Serial.println("Printing, 'Fastest Time'");
  moveCursor(0,0);
  writeString("Fastest Time:");

  

  while(1){
    //Clear trigPin condition
    digitalWrite(trigPin, LOW);
    delayUs(2);
    //Set the trigPin HIGH
    digitalWrite(trigPin, HIGH);
    delayUs(10);
    digitalWrite(trigPin, LOW);
    //Read echoPin, returns sound wave travel time in Us
    duration = pulseIn(echoPin, HIGH);
    //Calculate Distance
    distance = duration*0.034/2;

    //Timer debugging
    Serial.println(fastest_time);
    Serial.println(curr_time/100.0);
    Serial.println(state);


    if (distance < INIT_DISTANCE){
      if(state == wait_start){
        //Reset timer
        curr_time = 0;
        state = pass_start;
      }
      else if(state == wait_finish){
        state = pass_finish;
      }
    }

    //Button Press state Machine
    switch(switch_state){
        case wait_press:
          break;
        case bounce_low:
          delayMs(1);
          switch_state = wait_release;
          break;
        case wait_release:
          break;
        case bounce_high:
          delayMs(1);
          switch_state = wait_press;
          break;
      }

    //Timer state machine
    switch(state){
      case wait_start:
        break;
      case pass_start:
        delayMs(3000);  //Calculate vehicle speed to get this time
        state = wait_finish;
        break;
      case wait_finish:
        break;
      case pass_finish:
        if(curr_time/100.0 < fastest_time){
          fastest_time = curr_time/100.0;
          curr_time = 0;
          //convert fastestTime to String
          char timeString[50];
          dtostrf(fastest_time, 4, 3, timeString);
          //Display on LCD
          moveCursor(1,0);
          writeString("            ");
          moveCursor(1,0);
          writeString(timeString);
          moveCursor(1,8);
          writeString("s");
          //Display smiley face
          display(0x01,0b00000000);
          display(0x02,0b00100000);
          display(0x03,0b01000100);
          display(0x04,0b10000000);
          display(0x05,0b10000000);
          display(0x06,0b01000100);
          display(0x07,0b00100000);
          display(0x08,0b00000000);
        }
        else{
          curr_time = 0;
          //Display frownie face
          display(0x01,0b00000000);
          display(0x02,0b01000000);
          display(0x03,0b00100100);
          display(0x04,0b00010000);
          display(0x05,0b00010000);
          display(0x06,0b00100100);
          display(0x07,0b01000000);
          display(0x08,0b00000000);
        }
        delayMs(3000);  //Calculate vehicle speed to get this time
        state = wait_finish;
        break;
    }
  }
  return 0;
}

ISR(PCINT0_vect){
  //Handle button press
  if (switch_state == wait_press){
    switch_state = bounce_low;
  }
  else if (switch_state == wait_release){
    switch_state = bounce_high;
  }
  //Reset time counter
  state = wait_start;
  fastest_time = 100.0;
  moveCursor(1,0);
  writeString("restart     ");
}

ISR(TIMER3_COMPA_vect){
  curr_time += 1;
}
