#include <Arduino.h>
#include <stdio.h>
#include "lcd.h"
#include "timer.h"
#include <avr/io.h>
#include <string.h>
#include <time.h>


#define echoPin 19   //attach pin D2 of Arduino to pin Echo of HC-SR04
#define trigPin 18   //attach pin D3 of Arduino to pin Trig of HC-SR04

long duration;
int distance;

long INIT_DISTANCE = 150;

double fastest_time = 0.0;
clock_t curr_start;
clock_t curr_end;

//Define set of states for lap state
typedef enum stateEnum{
  wait_start,
  pass_start,
  wait_finish,
  pass_finish
} stateType;

volatile stateType state = wait_start;

void initUSS(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}


int main(){
  //initialize everything
  initTimer0();
  initTimer1();
  initLCD();
  initUSS();
  
  double diff;


  Serial.begin(9600); //baud rate
  sei();


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

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    //Serial.print(state);


    if (distance < INIT_DISTANCE){
      if(state == wait_start){
        //TODO: start timer
        state = pass_start;
      }
      else if(state == wait_finish){
        curr_end = clock();
        diff = curr_end-curr_start;
        curr_start = curr_end;
        Serial.println(diff);
        state = pass_finish;
        //TODO: restart timer
      }
    }


    switch(state){
      case wait_start:
        break;
      case pass_start:
        curr_start = clock(); 
        delayMs(3000);  //Calculate vehicle speed to get this time
        state = wait_finish;
        break;
      case wait_finish:
        break;
      case pass_finish:
        if(diff < fastest_time){
          fastest_time = diff;
          //convert fastestTime to String
          char timeString[50];
          dtostrf(fastest_time, 4, 3, timeString);
          //Display on LCD
          moveCursor(1,0);
          writeString(timeString);
          moveCursor(1,8);
          writeString("s");
          //TODO: Display smiley face
        }
        else{
          //TODO: Display frownie face
        }
        delayMs(3000);  //Calculate vehicle speed to get this time
        state = wait_finish;
        break;
    }
    
  }

  return 0;
}

ISR(PCINT0_vect){
  state = wait_start;
  fastest_time = 0.0;
}
