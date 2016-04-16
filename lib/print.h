#ifndef PRINT_H
#define PRINT_H

#ifdef MCU
#include "../mculib/usart.h"
#else
#include "../pclib/usart.h"
#endif

#define putc(ch) USARTPutc(ch)
void puts(char *message);
void putbytes(char * bytes, int count);
void putcdec(unsigned char ch);
void putcbin(char ch);
void putchex(char ch);

#define getc(ch) USARTGetc(ch)

#endif
