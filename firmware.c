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

#define FL_SEC	0x01

volatile unsigned char flags;

struct node my_info;
volatile unsigned int time;
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

void InitialiseTIM2(){
	// Set prescaled
	TIM2_PSCR=10;
	// Set overflow value
	TIM2_ARRH=15625 >> 8;
	TIM2_ARRL=15625 & 0xFF;
	// Enable overflow interrupts
	TIM2_IER=TIM_IER_UIE;
	// Enable timer
	TIM2_CR1=TIM_CR1_CEN;

}

//
//  Main program loop.
//
int main(void)
{
	unsigned char memflags;
	unsigned char action;
	unsigned char stage;
	unsigned char timeidx;
	unsigned char i;

	time = 0;
	
	__disable_interrupt();
	InitialiseSystemClock();
	InitialiseUSART();
	InitialiseTIM2();
	spi_init();
	nrf_init();
	network_init(RF_ROLE_TX);
	__enable_interrupt();

	delay_ms(1000);
	
#ifdef DUMP
	nrf_setrx();
#endif
#ifdef CITY
	cities_init();
	timeidx = 0;
	memflags = 0;
	my_info.type = TYPE_CITY;
	my_info.team = NO_TEAM;
	my_info.id = MY_ID;
	my_info.units = CITY_UNITS;
	my_info.mode = MODE_DEFENSE;
	my_info.level = 0;

	pk_out.node_from = my_info.id;
	pk_out.type = my_info.type;
	for (i = 0;i<10;i++){
		pk_out.teams[i] = 0xFF;
		pk_out.times[i] = 0xFF;	
	}
#endif

#ifdef SOURCE
	my_info.type = TYPE_SOURCE;
	my_info.team = NO_TEAM;
	my_info.id = MY_ID;	

	pk_out.node_from = my_info.id;
	pk_out.type = my_info.type;
#endif

	// Configure pins
	PB_DDR = (1<<5);
	PB_CR1 = (1<<5);
//	PB_ODR ^= (1<<5);
	// Loop
	do {
	//	PB_ODR ^= (1<<5);

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
		pk_out.team = my_info.team;
		pk_out.units = SOURCE_UNITS;
		pk_out.action = ACTION_SOURCE;
		pk_out.node_to = 0;
		if (memflags & FL_SEC){
			PB_ODR ^= (1<<5);
			network_send(&pk_out,0);
		}
		delay_ms(100);
	
#endif
#ifdef CITY
		pk_out.team = my_info.team;
		if (memflags & FL_SEC){
			puts("Team: ");
			putchex(my_info.team);
			puts(" level: ");
			putchex(my_info.level);
			puts(" units: ");
			putchex(my_info.units);
			puts(" uplevel: ");
			putchex(my_info.uplevel);
			putchex(timeidx);
			putc('\n');
			cities_broadcast();		
			if (time%(10-CITY_MAXLEVEL) == 0){
				if (my_info.units < CITY_MAXUNITS){ 
					my_info.units++;
				} else if (my_info.uplevel < CITY_LEVEL_UP){
					my_info.uplevel++;
				} else {
					my_info.uplevel = 0;
					if (my_info.level < CITY_MAXLEVEL){
						my_info.level++;	
					}	
				}
			}
			if (time%60 == 0){
				pk_out.teams[timeidx]=my_info.team;
				pk_out.times[timeidx]=time/60;	
				timeidx++;
				timeidx %= 10;
			}
		} 
		network_arcv_stop();
		delay_us(100);
                while (network_arcv(&pk_in)){
			if (my_info.units==0){
				break;	
			}
			PB_ODR ^= (1<<5);
                        if (pk_in.action == ACTION_BROADCAST){
				if ((pk_in.team == my_info.team)&&(my_info.units >=1)){
					transfer_start(pk_in.node_from);	
				}
                        } else {
                                if (pk_in.node_to == my_info.id){
					cities_city_process_action();
					puts("Processing\n");
                                }
                        }
                }
 

		delay_us(100);
		network_arcv_start();	
		delay_ms(100);


#endif


			

	memflags = flags;
	flags = 0;		
	} while(1);
}


void TIM2_OVF_vect (void) INT_TIM2_OVF{
	TIM2_SR1 &= ~(TIM_SR1_UIF);
	flags |= FL_SEC;
	time++;
//	PB_ODR ^= (1<<5);
}
