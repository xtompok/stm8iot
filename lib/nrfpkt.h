#ifndef NRFPKT_H
#define NRFPKT_H



void nrf_init();

void nrf_powerup();
void nrf_powerup_nodelay();
void nrf_powerdown();

/* Read single or two-byte register (all of them except addresses). */
unsigned int nrf_reg_read(unsigned char reg, unsigned char size);
void nrf_reg_write(unsigned char reg, unsigned int data, unsigned char size);
void nrf_transmit(unsigned char *data, unsigned char len);
int nrf_receive(unsigned char *data, unsigned char len);
void nrf_cmd_flush_rx(void);

void nrf_listen();
void nrf_nolisten();

extern unsigned char _nrf_status;
#define nrf_setrx() _nrf_setrx(1)
#define nrf_settx() _nrf_setrx(0)

#define NRFPKT_PWRUP 0x01
#define NRFPKT_PWRDN 0x10
#define NRFPKT_INIT 0x02
#define NRFPKT_REG_RD 0x03
#define NRFPKT_REG_WR 0x04
#define NRFPKT_TX 0x05
#define NRFPKT_RX 0x06
#define NRFPKT_FLUSH_RX 0x07
#define NRFPKT_LSTN 0x08
#define NRFPKT_NLSTN 0x09
#define NRFPKT_SETRX 0x11
#define NRFPKT_SETTX 0x12


#endif
