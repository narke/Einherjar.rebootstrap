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
format_int(int value, char *str, char base)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t divisor = 10;
	uint8_t is_negative = 0;
	uint8_t remainder;
	char tmp;

	if (value == 0)
		str[i++] = '0';

	if (base == 'd' && value < 0) {
		is_negative = 1;
		value = -value;
	} else if (base == 'x') {
		divisor = 16;
	}

	while (value > 0) {
		remainder = value % divisor;
		str[i++] = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
		value = value / divisor;
	}

	if (base == 'd' && is_negative)
		str[i++] = '-';

	if (base == 'x') {
		str[i++] = 'x';
		str[i++] = '0';
	}

	str[i] = '\0';
	for (i = i - 1, j = 0; j < i; i--, j++) {
		tmp = str[j];
		str[j] = str[i];
		str[i] = tmp;
	}
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
		format_int(va_arg(args, int), buffer, 'd');
		print_string(buffer);
		break;

	case 'x':
		format_int((int)va_arg(args, unsigned int), buffer, 'x');
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
