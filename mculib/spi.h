#ifndef SPI_H
#define SPI_H

#include "stm8s.h"
void spi_init();

unsigned char spi_xfer_byte(unsigned char data);


#endif
