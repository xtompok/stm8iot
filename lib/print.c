#include "print.h"
#ifdef MCU
#include "../mculib/usart.h"
#else
#include "../pclib/usart.h"
#endif


//
//  Send a message to the debug port (USART1).
//
void puts(char *message)
{
    char *ch = message;
    while (*ch)
    {
	putc(*ch);
        ch++;                               //  Grab the next character.
    }
}

// Print unsigned char number as a number (not one character)
void putcdec(unsigned char ch){
	putc((ch/100)+'0');
	ch%=100;
	putc((ch/10)+'0');
	ch%=10;
	putc((ch)+'0');
}

void putcbin(char c){
       char i;
       for (i =0; i<8;i++){
               if (c & (1<<(7-i)))
                       putc('1');
               else
                       putc('0');
       }
       putc(' ');
}

void putchex(char c){
	char nib;
	putc('0');
	putc('x');
	nib = (c&0xF0)>>4;
	if (nib < 10)
		putc('0'+nib);
	else
		putc('a'+nib-10);
	nib = c&0x0F;
	if (nib < 10)
		putc('0'+nib);
	else
		putc('a'+nib-10);
}


