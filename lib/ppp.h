#ifndef PPP_H
#define PPP_H

#define PPP_FLAG 0x7E
#define PPP_ESC 0x7D
#define PPP_ESCXOR 0x20

char getMessage(char * buf, unsigned char limit);
void putMessage(char * buf, unsigned char len);


#endif
