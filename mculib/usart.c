#include "usart.h"
#include "circular.h"

volatile struct CQueue RXBuf;
volatile struct CQueue TXBuf;

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

	// Initialize queues
	CQInit(&RXBuf);
	CQInit(&TXBuf);
    
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
    USART1_CR1 &= ~USART_CR1_M;        //  8 Data bits.
    USART1_CR1 &= ~USART_CR1_PCEN;     //  Disable parity.
    USART1_CR3 &= ~(USART_CR3_STOP1|USART_CR3_STOP2) ;     //  1 stop bit.
    USART1_BRR2 = 0x0b;      //  Set the baud rate registers to 115200 baud
    USART1_BRR1 = 0x08;      //  based upon a 16 MHz system clock.
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
    USART1_CR2 |= (USART_CR2_TEN|USART_CR2_REN|USART_CR2_RIEN);

	// Set interrupt software priority to 1
	//SET_SPR_USART1_RX_1;
     USART1_DR = 'b';
}

void USARTPutc(unsigned char ch){
	CQPush(&TXBuf,ch);
	USART1_CR2 |= USART_CR2_TIEN;
}


// Get character from buffer
char USARTGetc(void){
	return CQPop(&RXBuf);
}

// Character received interrupt
// Get it and push it in the circular buffer
void USART_RX_vect (void) INT_USART1_RX {
	char ch;
	ch = USART1_DR;
	CQPushNB(&RXBuf,ch);
}

void USART_TX_vect (void) INT_USART1_TX {
	char ch;
	if (CQisEmpty(&TXBuf)){
		USART1_CR2 &= ~USART_CR2_TIEN;
		return;
	}
	ch = CQPopNB(&TXBuf);
	USART1_DR = ch;
}
