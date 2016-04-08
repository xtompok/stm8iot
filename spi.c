#include "spi.h"
#include "usart.h"

void spi_init(){
	SPI_CR1 = 0;
	// Frequency 16MHz / 256
	SPI_CR1 |= SPI_CR1_BR2 | SPI_CR1_BR1 | SPI_CR1_BR0;
	// Idle -> CLK low
	// reading on raise edge
	// MSB first
	SPI_CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_LSBFIRST);
	SPI_CR2 = 0;
	SPI_CR2 |= SPI_CR2_SSM | SPI_CR2_SSI;
	// enable SPI
	SPI_CR1 |= SPI_CR1_SPE | SPI_CR1_MSTR;

}
void spi_mode(unsigned char mode){

}
void spi_speed(unsigned int speed){

}

unsigned char spi_xfer_byte(unsigned char data){
	SPI_DR = data;
	// Wait until Tx buffer is empty and Rx not empty
	while (!(SPI_SR & SPI_SR_TXE) || !(SPI_SR & SPI_SR_RXNE));
	return SPI_DR;
}

