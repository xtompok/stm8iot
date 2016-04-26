#define MCU

#include "mculib/stm8s.h"
#include "lib/print.h"
#include "mculib/nrf.h"
#include "mculib/spi.h"
#include "mculib/delay.h"
#include "mculib/hardware.h"
#include "magique.h"
#include "lib/network.h"
#include "modes/cities.h"

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
	
	__disable_interrupt();
	InitialiseSystemClock();
	InitialiseUSART();
	spi_init();
	nrf_init();
	network_init(RF_ROLE_TX);
	__enable_interrupt();

	delay_ms(1000);
	
#ifdef DUMP
	nrf_setrx();
#endif
#ifdef CITY
	my_info.type = TYPE_CITY;
	my_info.team = NO_TEAM;
	my_info.id = MY_ID;
	my_info.units = CITY_UNITS;
	my_info.mode = MODE_DEFENSE;
#endif

#ifdef SOURCE
	my_info.type = TYPE_SOURCE;
	my_info.team = NO_TEAM;
	my_info.id = MY_ID;	
#endif

	// Configure pins
	PB_DDR = (1<<5);
	PB_CR1 = (1<<5);
//	PB_ODR ^= (1<<5);
	// Loop
	do {
		PB_ODR ^= (1<<5);

#ifdef DUMP
		nrf_listen();
		delay_ms(10);
		nrf_nolisten();
		if (nrf_receive(data,32)){
			nrf_listen();
			puts("Data:");	
			for (i=0;i<32;i++){
				putchex(data[i]);	
				putc(' ');
			}
			puts("\n");	
		}
#endif
#ifdef SOURCE
		pk_out.node_from = my_info.id;
		pk_out.type = my_info.type;
		pk_out.team = my_info.team;
		pk_out.units = SOURCE_UNITS;
		pk_out.action = ACTION_SOURCE;
		pk_out.node_to = 0;
		network_send(&pk_out,0);
		delay_ms(1000);
	
#endif
#ifdef CITY



#endif


			

		
	} while(1);
}
