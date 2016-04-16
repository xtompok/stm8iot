#include "ppp.h"
#ifdef MCU
#include "../mculib/usart.h"
#else
#include "../pclib/usart.h"
#endif

char getMessage(char * buf, unsigned char limit){	
	char ch;
	unsigned char idx;
	idx = 0;
	do {
		ch = USARTGetc();
	} while (ch!=PPP_FLAG);
	ch = USARTGetc();
	while ((ch != PPP_FLAG) && (idx < limit)){
		if (ch == PPP_ESC){
			ch = USARTGetc();
			ch ^= PPP_ESCXOR;	
		}
		buf[idx] = ch;
		idx++;
		USARTGetc();
	}
	return idx;

}
void putMessage(char * buf, unsigned char len){
	unsigned char idx;
	char ch;
	USARTPutc(PPP_FLAG);
	for (idx =0;idx < len;idx++){
		if ((buf[idx]!=PPP_FLAG)&&(buf[idx]!=PPP_ESC)){
			USARTPutc(buf[idx]);	
			continue;
		}		
		ch = buf[idx];
		ch ^= PPP_ESCXOR;
		USARTPutc(PPP_ESC);
		USARTPutc(ch);
	}
	USARTPutc(PPP_FLAG);
}
