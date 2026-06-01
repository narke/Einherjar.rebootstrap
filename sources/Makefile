CC=ppc-linux-gnu-gcc
LD=ppc-linux-gnu-ld
PWD := $(shell pwd)

CFLAGS=-m32 -ffreestanding -fno-builtin -nostdinc -nostdlib -fno-exceptions \
	   -fno-leading-underscore -mcpu=powerpc -msoft-float -O3 \
	   -fexec-charset=UTF-8 -fwide-exec-charset=UTF-32BE \
	   -finput-charset=UTF-8 -std=gnu99 -pipe -Wall -Wextra \
	   -Wwrite-strings \
	   -Igeneric/include -Iarch/ppc32/include -I$(PWD)
LDFLAGS=-melf32ppc -n --warn-common --strip-all

YABOOT_DIR=yaboot
BOOT_DIR=$(YABOOT_DIR)/boot
LINKER_SCRIPT=linker.ld
MAPS=$(BOOT_DIR)/maps
CDROM_NAME=einherjar.iso
EXECUTABLE=yggdrasil.elf
KERNEL_IMAGE=$(BOOT_DIR)/$(EXECUTABLE)

# Future (not linked until native PIC/vectors replace OFW I/O):
#   arch/ppc32/src/exceptions.o  arch/ppc32/src/interrupts.o  drivers/keyboard.o
OBJECTS=boot.o				\
	lib/printf.o			\
	openfirmware/ofw.o		\
	drivers/console.o		\
	main.o

.PHONY: all cdrom clean

all: $(EXECUTABLE) cdrom

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) -T $(LINKER_SCRIPT) -o $@ $^

%.o: %.S
	$(CC) -D__ASM__ -c $< -o $@ $(CFLAGS)
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

cdrom: $(EXECUTABLE)
	mv $(EXECUTABLE) $(BOOT_DIR)/
	genisoimage -hfs -part -map $(MAPS) -no-desktop -hfs-volid "Einherjar" \
		-hfs-bless $(BOOT_DIR) -r -o $(CDROM_NAME) $(YABOOT_DIR)/

clean:
	rm -f $(OBJECTS) $(KERNEL_IMAGE) $(CDROM_NAME)
