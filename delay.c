/* 
 * delay utilite for STM8 family
 * COSMIC and SDCC
 * Terentiev Oleg
 * t.oleg@ymail.com
 */

#include "delay.h"
#include "hardware.h"

#ifndef F_CPU_MHZ
#warning F_CPU_MHZ is not defined!
#endif

/* 
 * Func delayed N cycles, where N = 3 + ( ticks * 3 )
 * so, ticks = ( N - 3 ) / 3, minimum delay is 6 CLK
 * when tick = 1, because 0 equels 65535
 */

static inline void _delay_cycl( unsigned short __ticks )
{
  #define T_COUNT(x) ((( F_CPU_MHZ * x )-5)/5)
	__asm__("nop\n nop\n"); 
	do { 		// ASM: ldw X, #tick; lab$: decw X; tnzw X; jrne lab$
                __ticks--;//      2c;                 1c;     2c    ; 1/2c   
        } while ( __ticks );
	__asm__("nop\n");
}

void delay_us( unsigned short __us )
{
	_delay_cycl( (unsigned short)( T_COUNT(__us) ));
}

void delay_ms( unsigned short __ms )
{
	while ( __ms-- )
	{
		delay_us( 1000 );
	}
}

