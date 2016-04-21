#define MCU

#include "mculib/stm8s.h"
#include "lib/print.h"
#include "mculib/nrf.h"
#include "mculib/spi.h"
#include "mculib/delay.h"
#include "mculib/hardware.h"
#include "magique.h"
#include "lib/network.h"

struct node my_info;
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
	unsigned char i;	
	unsigned char data[32];
	unsigned char config;
	
	data[0]=0xAA;	
	data[1]=0xF0;	
	data[2]=0x0F;	

	__disable_interrupt();
	InitialiseSystemClock();
	InitialiseUSART();
	spi_init();
	nrf_init();
	nrf_powerdown();
	nrf_powerup();
	// Setup the radio: default addresses, 0dBm tx power, 1Mbit
	nrf_reg_write(NRF_REG_CONFIG, EN_CRC | PWR_UP | PRIM_RX | CRCO, 1);
	nrf_reg_write(NRF_REG_RF_CH, 110, 1);
	nrf_reg_write(NRF_REG_RX_PW_P0, 32, 1);
	//nrf_reg_write(NRF_REG_RF_SETUP, RF_SETUP, 1);
	// Make everything async
	nrf_reg_write(NRF_REG_EN_AA, 0, 1);
	__enable_interrupt();

//	puts("Sent: 0xAA");
//	putchex(spi_xfer_byte(0xAA));
	delay_ms(1000);
	
//#define TX
#ifdef TX
	nrf_settx();
#else
	nrf_setrx();
#endif

	// Configure pins
	PB_DDR = (1<<5);
	PB_CR1 = (1<<5);
//	PB_ODR ^= (1<<5);
	// Loop
	do {
		PB_ODR ^= (1<<5);
//		nrf_listen();
//		for(d = 0; d < 29000; d++) { }
//		nrf_nolisten();
//		puts("Hello from my microcontroller....\n\r");

/*		putc('s');
		putcbin(nrf_reg_read(NRF_REG_STATUS,1));
		putchex(nrf_reg_read(NRF_REG_STATUS,1));
		putc('c');
		putcbin(nrf_reg_read(NRF_REG_CONFIG,1));
		putc('\n');
*/
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
		if (!nrf_receive(data,32)){
//			puts("Nothing\n");
		}else{
			nrf_listen();
			puts("Data:");	
			for (i=0;i<32;i++){
				putchex(data[i]);	
				putc(' ');
			}
			puts("\n");	
		}
//		nrf_setrx();
#endif
		
	} while(1);
}
