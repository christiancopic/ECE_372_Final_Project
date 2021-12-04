// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPB7(){
    DDRB &= ~(1<<DDB7);   //data direction to input
    PORTB |= (1<<PORTB7);    //enable pullup resistor


    PCICR |= (1<<PCIE0);   //enable PCINT0-7
    PCMSK0 |= (1<<PCINT7);   //enable PCINT 2
}
