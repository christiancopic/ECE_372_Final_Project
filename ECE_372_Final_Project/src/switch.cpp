#include<Arduino.h>
#include<avr/io.h>
#include "switch.h"
void initSwitchPD0(){
    
    DDRD &= ~(1<<DDD0);      //Set pin direction as input (pin 21 on board)
    PORTD |= (1<<PORTD0);  // enble the pull up resistor 
    
    // SET UP EXTERNAL INTERRUPT
    EICRA &= ~(1<<ISC01); // the change in any edge of INT0 is enble  [A reigister is used for 0:3]
    EICRA |= (1<<ISC00);

    EIMSK |= (1<<INT0); // external INTO is enable   ( if switch to other edge, this register must be disable first)
    
}