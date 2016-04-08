#ifndef SPI_H
#define SPI_H

#include "stm8s.h"
void spi_init();
//void spi_mode(unsigned char mode);
//void spi_speed(unsigned int speed);

unsigned char spi_xfer_byte(unsigned char data);


#endif
