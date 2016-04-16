#ifndef HARDWARE_H
#define HARDWARE_H

#include "stm8s.h"

#define F_CPU_MHZ 16

#define NRF_POUT PC_ODR 
#define NRF_PDIR PC_DDR
#define NRF_PCR PC_CR1
#define NRF_CE (1<<3)
#define NRF_CS (1<<4)


#endif
