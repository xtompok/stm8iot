#include "nrfpkt.h"
#include "nrf.h"
#include "ppp.h"

static unsigned char payload[33];

void parseMsg(char * msg){
	unsigned char reg;
	unsigned char size;
	unsigned int data;
	unsigned char * buf;
	unsigned char len;
	unsigned char i;

	switch (msg[0]){
		case NRFPKT_PWRUP:
			nrf_powerup();
			break;
		case NRFPKT_PWRDN:
			nrf_powerdown();
			break;
		case NRFPKT_INIT:
			nrf_init();
			break;
		case NRFPKT_REG_RD:
			reg = msg[1];
			size = msg[2];
			data = nrf_reg_read(reg,size);
			putMessage(&data,2);				
			break;
		case NRFPKT_REG_WR:
			reg = msg[1];
			data = msg[2];
			data <<=8;
			data != msg[3];
			size = msg[4];
			nrf_reg_write(reg,data,size);
			break;
		case NRFPKT_TX:
			len = msg[1];
			buf = msg+2;
			nrf_transmit(buf,len);
			break;
		case NRFPKT_RX:
			len = msg[1];
			data = nrf_receive(payload+1,len);
			payload[0]=data&0xFF;
			putMessage(&payload,len+1);
			break;
		case NRFPKT_FLUSH_RX:
			nrf_cmd_flush_rx();
			break;
		case NRFPKT_LSTN:
			nrf_listen();
			break;
		case NRFPKT_NLSTN:
			nrf_nolisten();
			break;
		case NRFPKT_SETRX:
			nrf_setrx();
			break;
		case NRFPKT_SETTX:
			nrf_settx();
			break;
		}	
}

