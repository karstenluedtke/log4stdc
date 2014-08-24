
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "log4stdc.h"

#define LF "\n"

static const char configstring[] =
"log4j.debug=false" LF
"log4j.rootLogger=error, console" LF
"" LF
"log4j.logger.barefootc= warn, console" LF
"log4j.additivity.pal=false" LF
"" LF
"log4j.logger.barefootc.string=error" LF
"log4j.logger.barefootc.vector=debug" LF
"log4j.logger.barefootc.vector.set=inherit" LF
"" LF
"log4j.appender.console=org.apache.log4j.ConsoleAppender" LF
"log4j.appender.console.layout=org.apache.log4j.PatternLayout" LF
"log4j.appender.console.layout.ConversionPattern=%d{ABSOLUTE} %-5p> %m%n" LF;

int
main (int argc, char *argv[])
{
	l4sc_logger_ptr_t logger;
	l4sc_configure_from_property_string(configstring, 0);

	logger = l4sc_get_logger("barefootc", 0);
	assert (logger != NULL);
	assert (l4sc_error_enabled(logger));
	assert (l4sc_warn_enabled(logger));
	assert (!l4sc_info_enabled(logger));
	assert (!l4sc_debug_enabled(logger));
	assert (!l4sc_trace_enabled(logger));

	logger = l4sc_get_logger("barefootc.string", 0);
	assert (logger != NULL);
	assert (l4sc_error_enabled(logger));
	assert (!l4sc_warn_enabled(logger));
	assert (!l4sc_info_enabled(logger));
	assert (!l4sc_debug_enabled(logger));
	assert (!l4sc_trace_enabled(logger));

	logger = l4sc_get_logger("barefootc.vector", 0);
	assert (logger != NULL);
	assert (l4sc_error_enabled(logger));
	assert (l4sc_warn_enabled(logger));
	assert (l4sc_info_enabled(logger));
	assert (l4sc_debug_enabled(logger));
	assert (!l4sc_trace_enabled(logger));

	logger = l4sc_get_logger("barefootc.vector.set", 0); // explicit INHERIT
	assert (logger != NULL);
	assert (l4sc_error_enabled(logger));
	assert (l4sc_warn_enabled(logger));
	assert (l4sc_info_enabled(logger));
	assert (l4sc_debug_enabled(logger));
	assert (!l4sc_trace_enabled(logger));

	logger = l4sc_get_logger("barefootc.vector.get", 0); // implicit INHERIT
	assert (logger != NULL);
	assert (l4sc_error_enabled(logger));
	assert (l4sc_warn_enabled(logger));
	assert (l4sc_info_enabled(logger));
	assert (l4sc_debug_enabled(logger));
	assert (!l4sc_trace_enabled(logger));

	logger = l4sc_get_logger("barefootc.vector.mem.alloc.ptrs", 0);
	assert (logger != NULL);
	assert (l4sc_error_enabled(logger));
	assert (l4sc_warn_enabled(logger));
	assert (l4sc_info_enabled(logger));
	assert (l4sc_debug_enabled(logger));
	assert (!l4sc_trace_enabled(logger));

	exit(0);
}

