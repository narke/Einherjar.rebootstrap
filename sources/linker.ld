ENTRY(start)

SECTIONS {
	. = 0x01000000;
	.text : {
		*(BOOTSTRAP);
		*(REALMODE);
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
[[COMPONENTS]]
	}

	/DISCARD/ : {
		*(.gnu.*);
		*(.mdebug*);
		*(.pdr);
		*(.comment);
		*(.note.*);
	}
}
