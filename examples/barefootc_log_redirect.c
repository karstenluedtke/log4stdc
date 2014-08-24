
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "log4stdc.h"

static int
enatest(void *user_arg, l4sc_logger_cptr_t logger, int level)
{
	return (level >= INFO_LEVEL);
}

static void
logfunc(void *user_arg, l4sc_logger_ptr_t logger,
	int level, const char *msg, size_t msglen,
	const char *file, int line, const char *func)
{
	printf("%s: %.*s [%s:%d]\n", func, (int) msglen, msg, file, line);
}

int
main(int argc, char *argv[])
{
	int some_state;
	l4sc_logger_ptr_t logger;

	l4sc_insert_custom_logger("barefootc", &some_state, enatest, logfunc);

	logger = l4sc_get_logger("barefootc.some.subclass", 0);
	L4SC_INFO(logger, "this to be output through custom logfunc");

	exit(0);
}

