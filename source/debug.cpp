/* Ensure we get real prototypes not dummies from our header */
#ifndef HAVE_DEBUG
#define HAVE_DEBUG
#endif

extern "C" {
#include "libService/ns_types.h"
#include "libService/platform/ns_debug.h"
#include "libService//ip6string.h"
} // extern "C"

#include <stdio.h>
#include <stdarg.h>


void debugf(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}
void debug(const char *s)
{
	fputs(s, stdout);
}

void debug_put(char c)
{
	putchar(c);
}

void debug_hex(uint8_t x)
{
	printf("%hhx", x);
}

void debug_int(int i)
{
	printf("%d", i);
}

void printf_array(const void *buf , uint16_t len)
{
	int i;
	const uint8_t *ptr = (const uint8_t*)buf;
	for(i=0;i<len;i++)
	{
		printf("%02x:", *ptr++);
		if (i && (0 == i%16))
			putchar('\n');
	}
	putchar('\n');
}

void printf_ipv6_address(const void *addr_ptr)
{
	char buf[40];
	ip6tos(addr_ptr, buf);
	printf("%s\n", buf);
}

void printf_string(const void *ptr, uint16_t len) {
	printf("%.*s\n", len, (const char*)ptr);
}

