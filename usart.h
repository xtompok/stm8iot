#ifndef USART_H
#define USART_H

#include "stm8s.h"

void InitialiseUSART();
void USARTPrintf(char *message);
void USARTPutc(char c);
void USARTPutByte(char c);


#endif
