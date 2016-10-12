#include <arch/asm/regname.h>

.section K_UNMAPPED_TEXT_START, "ax"

.org 0x500
.global exc_external
exc_external:

	li r3, 4


.org 0x900
.global exc_decrementer
exc_decrementer:

	li r3, 8
