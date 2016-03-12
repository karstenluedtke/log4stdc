
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

#include "log4stdc.h"

void
test1(const char *expectation, const char *fmt, const char *s)
{
	int rc;
	char buf[200];

	rc = l4sc_snprintf(buf, sizeof(buf), fmt, s);
	printf("test1: rc %d: \"%s\", expect \"%s\"\n", rc, buf, expectation);
	assert (rc == strlen(expectation));
	assert (strcmp(buf, expectation) == 0);
	assert (rc == strlen(buf));
}

void
test2(const char *expectation, const char *fmt, int val)
{
	int rc;
	char buf[200];

	rc = l4sc_snprintf(buf, sizeof(buf), fmt, val);
	printf("test1: rc %d: \"%s\", expect \"%s\"\n", rc, buf, expectation);
	assert (rc == strlen(expectation));
	assert (strcmp(buf, expectation) == 0);
	assert (rc == strlen(buf));
}

void
test3(const char *expectation, const char *fmt, ...)
{
	int rc;
	va_list ap;
	char buf[200];

	va_start(ap, fmt);
	rc = l4sc_vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	printf("test1: rc %d: \"%s\", expect \"%s\"\n", rc, buf, expectation);
	assert (rc == strlen(expectation));
	assert (strcmp(buf, expectation) == 0);
	assert (rc == strlen(buf));
}

int
main (int argc, char *argv[])
{
	test1("abcdef", "abc%s", "def");
	test1("abcd",   "abc%.1s", "def");
	test1("abc  d", "abc%3.1s", "def");
	test1("abcd  ", "abc%-3.1s", "def");

	test2("abc1k",  "abc%dk", 1);
	test2("abc12k", "abc%dk", 12);
	test2("abc123k","abc%dk", 123);
	test2("   123", "%6d",    123);
	test2("  0123", "%6.4d",  123);
	test2("0123  ", "%-6.4d", 123);
	test2("  +123", "%+6.3d", 123);
	test2("+00123", "%+06.3d",123);
	test2("000123", "%06.3d", 123);
	test2("  -123", "%6d",   -123);
	test2(" -0123", "%6.4d", -123);
	test2("-0123 ", "%-6.4d",-123);

	test3(" +1234567890", "%+12.10ld", 1234567890L);
	test3("+1234567890 ", "%-+12ld",   1234567890L);
	test3("aabb +1234567890. kk",
		"aa%s%+12.10ld.%3s", "bb", 1234567890L, "kk");
	test3("+4012345678 ", "%-+12lu",   4012345678uL);

	return (0);
}

