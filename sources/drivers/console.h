#pragma once

/* Console driver using Open Firmware stdout. */
void console_init(void);
void console_clear(void);
void console_puts(const char *s);
void console_putchar(char c);
