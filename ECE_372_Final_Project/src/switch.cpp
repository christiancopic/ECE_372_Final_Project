// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB2 and sets it into input mode
 */
void initSwitchPB2(){
    DDRB &= ~(1<<DDB2);   //data direction to input
    PORTB |= (1<<PORTB2);    //enable pullup resistor


    PCICR |= (1<<PCIE0);   //enable PCINT0-7
    PCMSK0 |= (1<<PCINT2);   //enable PCINT 2
}
