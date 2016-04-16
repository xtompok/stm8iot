#include "nrfpktsrv.h"

unsigned char payload[33];


void nrf_init(){
	unsigned char cmd;
	cmd = NRFPKT_INIT;
	putMessage(&cmd,1);
}

void nrf_powerup(){
	unsigned char cmd;
	cmd = NRFPKT_PWRUP;
	putMessage(&cmd,1);
}


void nrf_powerdown(){
	unsigned char cmd;
	cmd = NRFPKT_PWRDN;
	putMessage(&cmd,1);
}



/* Read single or two-byte register (all of them except addresses). */
unsigned int nrf_reg_read(unsigned char reg, unsigned char size){
	unsigned char cmd;
	unsigned int data;
	cmd = NRFPKT_REG_RD;
	putMessage(&cmd,1);
	getMessage(&data,2);
	return data;
}


void nrf_reg_write(unsigned char reg, unsigned int data, unsigned char size){
	payload[0] = NRFPKT_REG_WR;
	payload[1] = reg;
	payload[2] = (data >> 8);
	payload[3] = data&0xFF;
	payload[4] = size;
	putMessage(payload,5);
}


void nrf_transmit(unsigned char *data, unsigned char len){
	unsigned char i;
	payload[0] = NRFPKT_TX;
	payload[1] = len;
	for (i = 0;i<len;i++){
		payload[i+2] = data[i];	
	}
	putMessage(payload,len+1);
}


int nrf_receive(unsigned char *data, unsigned char len){
	unsigned char cmd[2];
	unsigned char i;
	cmd[0] = NRFPKT_RX;
	cmd[1] = len;
	putMessage(cmd,2);
	getMessage(payload,len+1);
	for (i=0;i<len;i++){
		data[i]=payload[i+1];	
	}
	return payload[0];
}

void nrf_cmd_flush_rx(void){
	unsigned char cmd;
	cmd = NRFPKT_FLUSH_RX;
	putMessage(&cmd,1);
}

void nrf_listen(){
	unsigned char cmd;
	cmd = NRFPKT_LISTEN;
	putMessage(&cmd,1);
}

void nrf_nolisten(){
	unsigned char cmd;
	cmd = NRFPKT_NOLISTEN;
	putMessage(&cmd,1);
}

void nrf_setrx(){
	unsigned char cmd;
	cmd = NRFPKT_SETRX;
	putMessage(&cmd,1);
}

void nrf_settx(){
	unsigned char cmd;
	cmd = NRFPKT_SETTX;
	putMessage(&cmd,1);
}


