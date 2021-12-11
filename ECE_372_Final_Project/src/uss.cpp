#include <avr/io.h>
#include <Arduino.h>
#include "timer.h"

#define echoPin 19 //PORTD2 //19   //attach pin D2 of Arduino to pin Echo of HC-SR04
#define trigPin 18 //PORTD3 //18   //attach pin D3 of Arduino to pin Trig of HC-SR04

void initUSS(){
	DDRD |= (1 << DDD3);
  //pinMode(trigPin, OUTPUT);
  //pinMode(echoPin, INPUT);
	DDRD &= ~(1 << DDD2);
	PORTD |= (1 << PORTD2);
  //Serial.begin(9600);
}

int calcDist(){
    long duration;
    //Clear trigPin condition
    //digitalWrite(trigPin, LOW);
	PORTD &= ~(1<<PORTD3);
    delayUs(2);
    //Set the trigPin HIGH
    //digitalWrite(trigPin, HIGH);
	PORTD |= (1<<PORTD3);
    delayUs(10);
    //digitalWrite(trigPin, LOW);
	PORTD &= ~(1<<PORTD3);
    //Read echoPin, returns sound wave travel time in Us

    duration = pulseIn(echoPin, HIGH); //arduino library to see how long the pin is high for
    
    //Calculate Distance
    return (duration*0.034/2);

}