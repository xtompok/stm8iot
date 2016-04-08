#include "stm8s.h"


//
//  Setup the USART to run at 115200 baud, no parity, one stop bit, 8 data bits.
//
//  Important: This relies upon the system clock being set to run at 16 MHz.
//
void InitialiseUSART()
{
	//
	//  Clear the Idle Line Detected bit in the status register by a read
	//  to the USART1_SR register followed by a Read to the USART1_DR register.
	//
	unsigned char tmp = USART1_SR;
	tmp = USART1_DR;
	//
	//  Reset the USART registers to the reset values.
	//
	USART1_CR1 = 0;
	USART1_CR2 = 0;
	USART1_CR4 = 0;
	USART1_CR3 = 0;
	USART1_CR5 = 0;
	USART1_GTR = 0;
	USART1_PSCR = 0;
	//
	//  Now setup the port to 115200,n,8,1.
	//
	USART1_CR1 &= ~USART_CR1_M;		//  8 Data bits.
	USART1_CR1 &= ~USART_CR1_PCEN;	 //  Disable parity.
	USART1_CR3 &= ~(USART_CR3_STOP1|USART_CR3_STOP2) ;	 //  1 stop bit.
	USART1_BRR2 = 0x0b;	  //  Set the baud rate registers to 115200 baud
	USART1_BRR1 = 0x08;	  //  based upon a 16 MHz system clock.
	//
	//  Disable the transmitter and receiver.
	//
	USART1_CR2 &= ~(USART_CR2_TEN|USART_CR2_REN);
	//
	//  Set the clock polarity and lock phase.
	//
	USART1_CR3 = (USART_CR3_CPOL|USART_CR3_CPHA);
	//
	//  Turn on the USART transmit and receive.
	//
	USART1_CR2 |= (USART_CR2_TEN|USART_CR2_REN);
}

//
//  Send a message to the debug port (USART1).
//
void USARTPrintf(char *message)
{
	char *ch = message;
	while (*ch)
	{
		USART1_DR = (unsigned char) *ch;	 //  Put the next character into the data transmission register.
		while (!(USART1_SR & USART_SR_TXE));		  //  Wait for transmission to complete.
		ch++;							   //  Grab the next character.
	}
}

void USARTPutc(char c){
	USART1_DR = c;	 //  Put the next character into the data transmission register.
	while (!(USART1_SR & USART_SR_TXE));		  //  Wait for transmission to complete.

}
