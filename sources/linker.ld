/* Yaboot/OFW load address for yggdrasil.elf; must match boot protocol. */
ENTRY(start)

SECTIONS {
	/* Kernel linked at 0x01000000 (see yaboot image=/boot/yggdrasil.elf). */
	. = 0x01000000;
	.text : {
		/* boot.S entry and early bootstrap (start). */
		*(BOOTSTRAP);
		*(.text);
	}
	.data : {
		*(.data);       /* initialized data */
		*(.rodata);
		*(.rodata.*);
		*(.sdata);
		*(.reginfo);
		*(.sbss);
		*(.scommon);
		*(.bss);        /* uninitialized static variables */
		*(COMMON);      /* global variables */
	}

	/DISCARD/ : {
		*(.gnu.*);
		*(.mdebug*);
		*(.pdr);
		*(.comment);
		*(.note.*);
	}
}
