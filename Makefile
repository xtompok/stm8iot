SDCC=sdcc
SDLD=sdld
CFLAGS=--std-sdcc99 --opt-code-speed -DSTM8
MFLAGS=-mstm8
LDFLAGS=-lstm8 --out-fmt-ihx
INC=-I. -Ilib -Imculib

STM8FLASH=~/Programy/stm8flash/stm8flash

SOURCES=firmware.c mculib/usart.c mculib/spi.c mculib/nrf.c mculib/delay.c mculib/circular.c lib/print.c lib/ppp.c lib/network.c modes/cities.c
OBJECTS=$(SOURCES:.c=.rel)
OBJECT=firmware

.PHONY: all clean flash

all: $(OBJECT).ihx

clean:
	rm -f $(OBJECTS) $(OBJECT).ihx \
	$(SOURCES:.c=.rel) $(SOURCES:.c=.rst) $(SOURCES:.c=.sym) $(SOURCES:.c=.lst) $(SOURCES:.c=.cdb) $(SOURCES:.c=.map)  $(SOURCES:.c=.lk)

flash: $(OBJECT).ihx
	$(STM8FLASH) -c stlinkv2 -p stm8s103?3 -s flash -w $(OBJECT).ihx

%.ihx: $(OBJECTS)
	$(SDCC) $(MFLAGS) $(LDFLAGS) $^ -o $@
%.rel: %.c
	$(SDCC) -c $(INC) $(CFLAGS) $(MFLAGS) -DMY_ID=$(ID) -D$(DEVICE) $< -o $@ 

