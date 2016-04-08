SDCC=sdcc
SDLD=sdld
CFLAGS=--std-sdcc99 --opt-code-speed
MFLAGS=-mstm8
LDFLAGS=-lstm8 --out-fmt-ihx

STM8FLASH=~/Programy/stm8flash/stm8flash

SOURCES=firmware.c usart.c 
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
	$(SDCC) -c $(CFLAGS) $(MFLAGS) $< -o $@ 

