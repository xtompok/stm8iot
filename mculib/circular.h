#ifndef CIRCULAR_H
#define CIRCULAR_H

#define CQ_CAPACITY 32

// Circular queue structure
struct CQueue{
	volatile char buf[CQ_CAPACITY];	// buffer
	volatile unsigned char first;	// index of the first element to pop
	volatile unsigned char last;	// index of the first empty space to push
};
void CQInit(struct CQueue * queue);

void CQPush(struct CQueue * queue, char c);
char CQPushNB(struct CQueue * queue, char c);

char CQPop(struct CQueue * queue);
char CQPopNB(struct CQueue * queue);

char CQisEmpty(struct CQueue * queue);

void CQPrint(struct CQueue * queue);


#endif
