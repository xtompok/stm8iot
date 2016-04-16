#ifndef USART_H
#define USART_H

#include "stm8s.h"
// Documentation in implementation

void InitialiseUSART(void);
void USARTPutc(unsigned char ch);
char USARTGetc(void);
void USART_RX_vect (void) INT_USART1_RX;
void USART_TX_vect (void) INT_USART1_TX;
#endif
