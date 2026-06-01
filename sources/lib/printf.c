/*
 * Copyright (c) 2015 Konstantin Tcholokachvili
 * All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <openfirmware/ofw.h>
#include "printf.h"
#include "typedefs.h"
#include "stdarg.h"

static void
print_char(char c)
{
	ofw_putchar(c);
}

static void
print_string(const char *s)
{
	while (*s)
		ofw_putchar(*s++);
}

static void
reverse_string(char *str, uint8_t len)
{
	uint8_t i, j;
	char tmp;

	for (i = len - 1, j = 0; j < i; i--, j++) {
		tmp = str[j];
		str[j] = str[i];
		str[i] = tmp;
	}
}

static void
format_dec(int value, char *str)
{
	uint8_t i = 0;
	uint8_t is_negative = 0;
	uint8_t remainder;

	if (value == 0)
		str[i++] = '0';

	if (value < 0) {
		is_negative = 1;
		value = -value;
	}

	while (value > 0) {
		remainder = value % 10;
		str[i++] = remainder + '0';
		value = value / 10;
	}

	if (is_negative)
		str[i++] = '-';

	str[i] = '\0';
	reverse_string(str, i);
}

static void
format_hex(unsigned int value, char *str)
{
	uint8_t i = 0;
	uint8_t remainder;

	if (value == 0)
		str[i++] = '0';

	while (value > 0) {
		remainder = value % 16;
		str[i++] = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
		value = value / 16;
	}

	str[i++] = 'x';
	str[i++] = '0';

	str[i] = '\0';
	reverse_string(str, i);
}

static void
print_format(char spec, va_list args)
{
	char buffer[20];

	switch (spec) {
	case 'c':
		print_char((char)va_arg(args, int));
		break;

	case 'd':
		format_dec(va_arg(args, int), buffer);
		print_string(buffer);
		break;

	case 'x':
		format_hex(va_arg(args, unsigned int), buffer);
		print_string(buffer);
		break;

	case 's':
		print_string(va_arg(args, char *));
		break;

	case '%':
		print_char('%');
		break;

	default:
		break;
	}
}

static void
vprintf(const char *fmt, va_list args)
{
	while (*fmt) {
		if (*fmt != '%') {
			ofw_putchar(*fmt++);
			continue;
		}

		fmt++;
		print_format(*fmt++, args);
	}
}

void
printf(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}
