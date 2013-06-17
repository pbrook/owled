name=RGBLED

OBJS=main.o

#CPU=atmega328p
#CPU=attiny45
CPU=attiny25

HEX=image.hex
ELF=image.elf
CC=avr-gcc
LD=$(CC)
CFLAGS=-mmcu=$(CPU) -g -Os -Wall
LDFLAGS=$(CFLAGS) -Wl,--gc-sections 
LIBS=-lc -lm
OBJCOPY=avr-objcopy

.PHONY: all

all: $(HEX)

$(OBJS): Makefile

upload: $(HEX)
	avrdude -q -p $(CPU) -c stk500v2 -P usb \
	  -U flash:w:$(HEX):i

$(ELF): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

clean:
	rm -f $(HEX) $(ELF) $(OBJS)
