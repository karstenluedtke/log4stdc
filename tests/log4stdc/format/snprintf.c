
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

int
main (int argc, char *argv[])
{
	test1("abcdef", "abc%s", "def");
	test1("abcd",   "abc%.1s", "def");
	test1("abc  d", "abc%3.1s", "def");
	test1("abcd  ", "abc%-3.1s", "def");
	return (0);
}

