#include "stm8s.h"
#include "usart.h"
#include "nrf.h"
#include "spi.h"
//
//  Setup the system clock to run at 16MHz using the internal oscillator.
//
void InitialiseSystemClock()
{
	CLK_ICKR = 0;					   //  Reset the Internal Clock Register.
	CLK_ICKR = CLK_ICKR_HSIEN;				 //  Enable the HSI.
	CLK_ECKR = 0;					   //  Disable the external clock.
	while (!(CLK_ICKR & CLK_ICKR_HSIRDY));	   //  Wait for the HSI to be ready for use.
	CLK_CKDIVR = 0;					 //  Ensure the clocks are running at full speed.
	CLK_PCKENR1 = 0xff;				 //  Enable all peripheral clocks.
	CLK_PCKENR2 = 0xff;				 //  Ditto.
	CLK_CCOR = 0;					   //  Turn off CCO.
	CLK_HSITRIMR = 0;				   //  Turn off any HSIU trimming.
	CLK_SWIMCCR = 0;					//  Set SWIM to run at clock / 2.
	CLK_SWR = 0xe1;					 //  Use HSI as the clock source.
	CLK_SWCR = 0;					   //  Reset the clock switch control register.
	CLK_SWCR = CLK_SWCR_SWEN;				  //  Enable switching.
	while (CLK_SWCR & CLK_SWCR_SWBSY);		//  Pause while the clock switch is busy.
}

//
//  Main program loop.
//
int main(void)
{
	int d;
	unsigned char data[8];
	
	data[0]=0xAA;	
	data[1]=0xF0;	
	data[2]=0x0F;	

	__disable_interrupt();
	InitialiseSystemClock();
	InitialiseUSART();
	spi_init();
	nrf_init();
	__enable_interrupt();

	// Configure pins
	PB_DDR = (1<<5);
	PB_CR1 = (1<<5);
	// Loop
	do {
		PB_ODR ^= (1<<5);
		nrf_listen();
		for(d = 0; d < 29000; d++) { }
		nrf_nolisten();
		USARTPrintf("Hello from my microcontroller....\n\r");
//#define TX
#ifdef TX
		nrf_transmit(data,3);
#else
		if (!nrf_receive(data,3)){
			USARTPrintf("Nothing\n");
		}else{
			USARTPrintf("Data:");	
			USARTPutc(data[0]);
			USARTPutc(data[1]);
			USARTPutc(data[2]);
			USARTPrintf("\n");	
		}
#endif
		
	} while(1);
}
