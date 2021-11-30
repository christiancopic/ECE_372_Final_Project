#ifndef SPI_H
#define SPI_H
void initSPI();
void Command(unsigned int address, unsigned int command);
void display(unsigned int ADDRESS, unsigned int COMMAND);
#endif