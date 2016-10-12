ENTRY(start)

SECTIONS {
    .unmapped 0: AT (0) {
		unmapped_ktext_start = .;
		*(K_UNMAPPED_TEXT_START);
		unmapped_ktext_end = .;
	}
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
