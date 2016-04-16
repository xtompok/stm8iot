#include "circular.h"
#include "hardware.h"
//#include "usart.h"

/* Circular queue
 *
 * Items are added on one end and removed on the other end. 
 */

void CQInit(struct CQueue * queue){
	queue->first = 0;
	queue->last = 0;
}

// Blocking insert
// Block when queue if full until someone pops from the queue
void CQPush(struct CQueue * queue, char c){
full:
	while ((queue->last+1)%CQ_CAPACITY==queue->first){
//		RefreshWatchdog();	
	}
	__disable_interrupt();
	if ((queue->last+1)%CQ_CAPACITY==queue->first){
		__enable_interrupt();
		goto full;
	}
	queue->buf[queue->last]=c;
	queue->last++;
	queue->last %= CQ_CAPACITY;
	__enable_interrupt();
}

// Non-blocking insert. For use in interrupt
// Discard inserted character if queue is full
#pragma nooverlay
char CQPushNB(volatile struct CQueue * queue, char c){
	if ((queue->last+1)%CQ_CAPACITY==queue->first)
		return 0;
	queue->buf[queue->last]=c;
	queue->last++;
	queue->last %= CQ_CAPACITY;
	return 1;
}

// Blocking pop
// Block until someone puts something into the queue
char CQPop(volatile struct CQueue * queue){
	char out;
empty:
	while (queue->last==queue->first){
//		RefreshWatchdog();	
	}
	__disable_interrupt();
	if (queue->last==queue->first){
		__enable_interrupt();
		goto empty;
	}
	out = queue->buf[queue->first];
	queue->first++;
	queue->first %= CQ_CAPACITY;
	__enable_interrupt();
	return out;
}

char CQPopNB(volatile struct CQueue * queue){
	char out;
	if (queue->last==queue->first){
		return 0;
	}
	out = queue->buf[queue->first];
	queue->first++;
	queue->first %= CQ_CAPACITY;
	return out;
}


// Print characters currently in queue
/*void CQPrint(struct CQueue * queue){
	unsigned char idx;
	USARTPuts("CQ:");
	idx = queue->first;
	while (idx < queue->last){
		USARTPutc(queue->buf[idx]);
		idx++;
		idx %= CQ_CAPACITY;
	}
	USARTPutc('\n');
}*/


char CQisEmpty(struct CQueue * queue){
	return (queue->last==queue->first);

}
