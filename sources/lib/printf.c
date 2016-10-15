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
itoa(int value, char *str, uint8_t base)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t divisor = 10;
	uint8_t is_negative = 0;
	uint8_t remainder;
	char tmp;

	// Convert 0
	if (value == 0)
		str[i++] = '0';

	// Handle negative decimal values
	if (base == 'd' && value < 0)
	{
		is_negative = 1;
		value = -value;
	}
	else if (base == 'x') // Handle hex values
		divisor = 16;

	// Convert the value into the corresponding base
	while (value > 0)
	{
		remainder = value % divisor;
		str[i++] = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
		value = value / divisor;
	}

	// Add '-' to negative numbers now, to reverse that later
	if (base == 'd' && is_negative)
		str[i++] = '-';

	// Handle hex values
	if (base == 'x')
	{
		str[i++] = 'x';
		str[i++] = '0';
	}

	// Finalizing by ending the string and reversing it
	str[i] = '\0';
	for (i = i-1, j = 0; j < i; i--, j++)
	{
		tmp = str[j];
		str[j] = str[i];
		str[i] = tmp;
	}
}



static void
vprintf(const char *fmt, va_list args)
{
	int d;
	unsigned int u;
	char *s;
	char buffer[20];
	int i;

	while (*fmt)
	{
		if (*fmt != '%')
		{
			ofw_putchar(*fmt++);
			continue;
		}

		fmt++; // skip '%'

		switch(*fmt++)
		{
			case 'c':
				d = va_arg(args, int);
				ofw_putchar(d);
				break;

			case 'd':
				d = va_arg(args, int);
				itoa(d, buffer, 'd');
				for (i = 0; buffer[i] != '\0'; i++)
					ofw_putchar(buffer[i]);
				break;

			case 'x':
				u = va_arg(args, unsigned int);
				itoa(u, buffer, 'x');
				for (i = 0; buffer[i] != '\0'; i++)
					ofw_putchar(buffer[i]);
				break;

			case 's':
				s = va_arg(args, char *);
				while (*s)
					ofw_putchar(*s++);
				break;

			case '%':
				ofw_putchar('%');
				break;

			default:
				;

		}
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
