CC=ppc-linux-gnu-gcc
LD=ppc-linux-gnu-ld
PWD := $(shell pwd)
CFLAGS=-m32 -ffreestanding -fno-builtin -nostdinc -nostdlib -fno-exceptions \
	   -fno-leading-underscore -mcpu=powerpc -msoft-float -O3 \
	   -fexec-charset=UTF-8 -fwide-exec-charset=UTF-32BE \
	   -finput-charset=UTF-8 -std=gnu99 -pipe -Wall -Wextra -Werror \
	   -Wno-unused-parameter -Wmissing-prototypes \
	   -Werror-implicit-function-declaration -Wwrite-strings -I$(PWD)
LDFLAGS=-melf32ppc -n --warn-common --strip-all

MAPS=yaboot/boot/maps
CDROM_NAME=einherjar.iso

OBJECTS=boot.o lib/printf.o openfirmware/ofw.o main.o
EXECUTABLE=yggdrasil.elf

all: $(EXECUTABLE) cdrom

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $^

%.o: %.S
	$(CC) -D__ASM__ -c $< -o $@ $(CFLAGS)
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

cdrom:
	mv $(EXECUTABLE) yaboot/boot/
	genisoimage -hfs -part -map $(MAPS) -no-desktop -hfs-volid "Einherjar" \
		-hfs-bless yaboot/boot -r -o $(CDROM_NAME) yaboot/

clean:
	rm -f $(OBJECTS) yaboot/boot/$(EXECUTABLE) $(CDROM_NAME)
