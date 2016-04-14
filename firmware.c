#include "stm8s.h"
#include "usart.h"
#include "nrf.h"
#include "spi.h"
#include "delay.h"
#include "hardware.h"
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

static void nrf_csh() { NRF_POUT |= NRF_CS; }
static void nrf_csl() { NRF_POUT &= ~NRF_CS; }
static void nrf_ceh() { NRF_POUT |= NRF_CE; }
static void nrf_cel() { NRF_POUT &= ~NRF_CE; }

//
//  Main program loop.
//
int main(void)
{
	int d;
	unsigned char data[8];
	unsigned char config;
	
	data[0]=0xAA;	
	data[1]=0xF0;	
	data[2]=0x0F;	

	__disable_interrupt();
	InitialiseSystemClock();
	InitialiseUSART();
	spi_init();
	__enable_interrupt();
	
	delay_ms(100);

	delay_ms(20); /* 10.3ms should be enough */
	NRF_PDIR |= NRF_CS | NRF_CE;
	NRF_PCR |= NRF_CS | NRF_CE;
	nrf_csh();
	nrf_cel();
        delay_ms(20); /* 10.3ms should be enough */
	

        config = nrf_reg_read(NRF_REG_CONFIG, 1);
	USARTPutByte(config);
	config |= PWR_UP;
	USARTPutByte(config);
        nrf_reg_write(NRF_REG_CONFIG, config, 1);
        config = nrf_reg_read(NRF_REG_CONFIG, 1);
	USARTPutByte(config);
        delay_ms(20); /* 10.3ms should be enough */

//      nrf_reg_write(NRF_REG_CONFIG, config | PWR_UP | CRCO, 1);
        delay_ms(2); /* 1.5ms should be enough */

        nrf_reg_write(NRF_REG_EN_AA, 0, 1); /* Disable Auto Acknowledgment */
        nrf_reg_write(NRF_REG_RF_CH, 1, 1);
        nrf_reg_write(NRF_REG_RX_PW_P0, 3, 1);
        nrf_reg_write(NRF_REG_RF_SETUP, RF_PWR1 | RF_DR | LNA_HCURR, 1);
        nrf_reg_write(NRF_REG_CONFIG, EN_CRC | PWR_UP | PRIM_RX | CRCO, 1);
        config = nrf_reg_read(NRF_REG_CONFIG, 1);
	USARTPutByte(config);


	USARTPutc('\n');
	USARTPutByte(nrf_reg_read(0x07,1));
	USARTPutByte(nrf_reg_read(0x00,1));
	USARTPutc('\n');



/*	USARTPutByte(nrf_reg_read(0x04,1));
	USARTPutc('c');
	nrf_reg_write(0x04,0xAA,1);
	USARTPutByte(nrf_reg_read(0x04,1));
	USARTPutc('c');
	nrf_reg_write(0x04,0xEE,1);
	USARTPutByte(nrf_reg_read(0x04,1));
	USARTPutc('c');
*/
//	delay_ms(1000);
//#define TX
#ifdef TX
	nrf_settx();
#else
	nrf_setrx();
#endif

	// Configure pins
	PB_DDR = (1<<5);
	PB_CR1 = (1<<5);
	// Loop
	do {
		PB_ODR ^= (1<<5);
//		nrf_listen();
//		for(d = 0; d < 29000; d++) { }
//		nrf_nolisten();
//		USARTPrintf("Hello from my microcontroller....\n\r");

		USARTPutc('s');
		USARTPutByte(nrf_reg_read(NRF_REG_STATUS,1));
		USARTPutc('c');
		USARTPutByte(nrf_reg_read(NRF_REG_CONFIG,1));
		USARTPutc('\n');
#ifdef TX
		delay_ms(10);
		nrf_transmit(data,3);
		while (!(nrf_reg_read(NRF_REG_STATUS,1)&TX_DS));
		nrf_reg_write(NRF_REG_STATUS,TX_DS,1);
#else
		nrf_listen();
		delay_ms(10);
		nrf_nolisten();
		data[0]=0xba;
		data[1]=0xdc;
		data[2]=0xaf;
		if (!nrf_receive(data,3)){
			USARTPrintf("Nothing\n");
		}else{
			USARTPrintf("Data:");	
			USARTPutc(data[0]);
			USARTPutc(data[1]);
			USARTPutc(data[2]);
			USARTPrintf("\n");	
		}
//		nrf_setrx();
#endif
		
	} while(1);
}
