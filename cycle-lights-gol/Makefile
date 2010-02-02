SOURCE_FILES	= cycle-lights.c

TARGET_ARCH	= atmega48 # for GCC
CLOCK_SPEED	= 8000000UL # 8MHz

PART_NO		= m48 # for avrdude

PROGRAMMER	= usbtiny
PROGRAMMER_PORT	= usb

OUT_BASE_NAME	= main

CC		= avr-gcc
CFLAGS		= -Os -Wall -I. -mmcu=$(TARGET_ARCH) -DF_CPU=$(CLOCK_SPEED)

AVRDUDE		= avrdude
AVRDUDE_BASE_CMD = $(AVRDUDE) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) \
	-p $(PART_NO)

OBJCOPY	= avr-objcopy

all: fuse flash

help:
	@echo fuse	write fuses
	@echo flash	install code into flash memory

fuse:
	$(AVRDUDE_BASE_CMD) -B 10 -U lfuse:w:0xe2:m -U  hfuse:w:0xdf:m

flash: $(OUT_BASE_NAME).bin
	$(AVRDUDE_BASE_CMD) -B 1 -U flash:w:$(OUT_BASE_NAME).bin:r

$(OUT_BASE_NAME).bin: $(OUT_BASE_NAME).elf
	$(OBJCOPY) -j .text -j .data -O binary $(OUT_BASE_NAME).elf $(OUT_BASE_NAME).bin

$(OUT_BASE_NAME).elf: $(SOURCE_FILES)
	$(CC) $(CFLAGS) $(SOURCE_FILES) -o $(OUT_BASE_NAME).elf
