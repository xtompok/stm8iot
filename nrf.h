
/******************************************************************************
 * Filename: nrf.h
 *
 * Created: 2013/02/02 19:33
 * Author: Ladislav Láska
 * e-mail: laska@kam.mff.cuni.cz
 *
 ******************************************************************************/

#ifndef __NRF_H__
#define __NRF_H__

#define BIT0 (1<<0)
#define BIT1 (1<<1)
#define BIT2 (1<<2)
#define BIT3 (1<<3)
#define BIT4 (1<<4)
#define BIT5 (1<<5)
#define BIT6 (1<<6)
#define BIT7 (1<<7)


/* Initialize device communication, including power-up delay and port setup. */
void nrf_init();

void nrf_powerup();
void nrf_powerup_nodelay();
void nrf_powerdown();

/* Read single or two-byte register (all of them except addresses). */
unsigned int nrf_reg_read(unsigned char reg, unsigned char size);
void nrf_reg_write(unsigned char reg, unsigned int data, unsigned char size);
void nrf_transmit(unsigned char *data, unsigned char len);
int nrf_receive(unsigned char *data, unsigned char len);
void _nrf_setrx(unsigned char prx);
void nrf_cmd_flush_rx(void);

void nrf_listen();
void nrf_nolisten();

extern unsigned char _nrf_status;


/* Config register */
#define NRF_REG_CONFIG 	0x00
#define MASK_RX_DR 	BIT6
#define MASK_TX_DS 	BIT5
#define MASK_MAX_RT BIT4
#define EN_CRC 		BIT3
#define CRCO 		BIT2
#define PWR_UP 		BIT1
#define PRIM_RX 	BIT0

/* EN_AA register */
#define NRF_REG_EN_AA	0x01
#define ENAA_P0	BIT0
#define ENAA_P1	BIT1
#define ENAA_P2	BIT2
#define ENAA_P3	BIT3
#define ENAA_P4	BIT4
#define ENAA_P5	BIT5

/* EN_RXADDR register */
#define NRF_REG_EN_RXADDR 0x02
#define ERX_P5 BIT5
#define ERX_P4 BIT4
#define ERX_P3 BIT3
#define ERX_P2 BIT2
#define ERX_P1 BIT1
#define ERX_P0 BIT0

/* Radio channel register */
#define NRF_REG_RF_CH 0x05

/* STATUS register */
#define NRF_REG_STATUS	0x07
#define RX_DR	BIT6
#define TX_DS	BIT5
#define MAX_RT	BIT4
#define RX_P_NO	(BIT3 + BIT2 + BIT1)
#define TX_FULL BIT0

/* CD register */
#define NRF_REG_CD	0x09
#define CD BIT0

/* RX_PW_P? registers */
#define NRF_REG_RX_PW_P0	0x11
#define NRF_REG_RX_PW_P1	0x12
#define NRF_REG_RX_PW_P2	0x13
#define NRF_REG_RX_PW_P3	0x14
#define NRF_REG_RX_PW_P4	0x15
#define NRF_REG_RX_PW_P5	0x16

/* RF_SETUP register */
#define NRF_REG_RF_SETUP 0x06
#define PLL_LOCK BIT4
#define RF_DR BIT3
#define RF_PWR1 BIT2
#define RF_PWR0 BIT1
#define LNA_HCURR BIT0

#define nrf_setrx() _nrf_setrx(1)
#define nrf_settx() _nrf_setrx(0)

#endif
