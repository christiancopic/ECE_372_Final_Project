#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "spi.h"


#define wait_for_completion while(!(SPSR & (1 << SPIF)));

void initSPI(){
    
    DDRB = (1 <<DDB0 )| (1 << DDB1) | (1 <<DDB2); // set data direction as out( SS pin 53, SCK pin 52, MOSI pin 50)
                                                    // we do not need for MISO pin 22 
    PORTB |= (1<<PORTB0); // set SS pin high 

    // (SPCR = SPI control register)
    SPCR |= (1<<SPE); // enble SPI
    SPCR |= (1<<MSTR); // enble SPI as master mode
    SPCR |= (1<<CPOL);  // setup at the falling edge
    SPCR |= (1<<CPHA); // sample on the raising edge 

    // Fosc/ 2 ( devide oscallator frequecy by 64)
    // SPR1  = 1, SPR0 = 0
    SPCR |= (1<<SPR1);
    SPCR &= ~(1<<SPR0);

    Command(0x0A,0x0F ); // set intensity to max
    Command(0x0B,0x07 ); // set scan limtit register  to all row and column
    Command(0x0C,0x01 ); // set shutdown register to normal mode
    Command(0x0F, 0x00); // test display
}
void Command(unsigned int address, unsigned int command){
  
    PORTB &= ~(1<<PORTB0); // enble select bit
    SPDR = address;  // load the target address 
    wait_for_completion;  
    SPDR = command; // load command into data resister 
    wait_for_completion;
    PORTB |= (1<<PORTB); // disable select bit 

}
void display(unsigned int ADDRESS, unsigned int data){
      
    PORTB &= ~(1<<PORTB0); // enble select bit
    SPDR = ADDRESS;  // load the target address 
    wait_for_completion;  
    SPDR = data; // load DATA into data resister 
    wait_for_completion;
    PORTB |= (1<<PORTB); // disable select bit 

}