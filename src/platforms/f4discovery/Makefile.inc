CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy

CFLAGS += -Istm32/include -mcpu=cortex-m4 -mthumb \
	-mfloat-abi=hard -mfpu=fpv4-sp-d16 \
	-DSTM32F4 -DF4DISCOVERY -I../libopencm3/include \
	-Iplatforms/stm32

LDFLAGS_BOOT = -lopencm3_stm32f4 -Wl,--defsym,_stack=0x20020000 \
	-Wl,-T,platforms/stm32/f4discovery.ld -nostartfiles -lc -lnosys \
	-Wl,-Map=mapfile -mthumb -mcpu=cortex-m4 -Wl,-gc-sections \
	-mfloat-abi=hard -mfpu=fpv4-sp-d16 \
	-L../libopencm3/lib
LDFLAGS = $(LDFLAGS_BOOT) -Wl,-Ttext=0x8010000

VPATH += platforms/stm32

SRC += 	cdcacm.c	\
	platform.c	\
	traceswo.c	\
	usbuart.c	\

all:	blackmagic.bin blackmagic_dfu.bin blackmagic_dfu.hex

blackmagic.bin:	blackmagic
	$(OBJCOPY) -O binary $^ $@

blackmagic_dfu: usbdfu.o dfucore.o dfu_f4.o
	$(CC) $^ -o $@ $(LDFLAGS_BOOT)

blackmagic_dfu.bin:	blackmagic_dfu
	$(OBJCOPY) -O binary $^ $@

blackmagic_dfu.hex:	blackmagic_dfu
	$(OBJCOPY) -O ihex $^ $@

host_clean:
	-rm blackmagic.bin blackmagic_dfu blackmagic_dfu.bin blackmagic_dfu.hex
