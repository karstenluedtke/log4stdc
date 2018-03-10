
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "log4stdc.h"

#define LF "\n"

static const char configstring[] =
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" LF
    "<log4j:configuration xmlns:log4j=\"http://jakarta.apache.org/log4j/\""
    " debug=\"1\">" LF
    "	<appender class=\"org.apache.log4j.ConsoleAppender\""
    " name=\"Console\">" LF
    "		<layout class=\"org.apache.log4j.PatternLayout\">" LF
    "		    <param name=\"ConversionPattern\""
    " value=\"%d{ABSOLUTE} [%t] %-5p %c - %m%n\"/>" LF
    "		</layout>" LF "	</appender>" LF
    "	<logger additivity=\"false\" name=\"barefootc\">" LF
    "		<level value=\"WARN\"/>" LF
    "		<appender-ref ref=\"Console\"/>" LF "	</logger>" LF
    "	<logger name=\"barefootc.string\">" LF
    "		<level value=\"ERROR\"/>" LF "	</logger>" LF
    "	<logger name=\"barefootc.vector\">" LF
    "		<level value=\"DEBUG\"/>" LF "	</logger>" LF
    "	<logger name=\"barefootc.vector.set\">" LF
    "		<level value=\"INHERIT\"/>" LF "	</logger>" LF
    "</log4j:configuration>" LF;

int
main(int argc, char *argv[])
{
    l4sc_logger_ptr_t logger;
    l4sc_configure_from_xml_string(configstring, 0);

    logger = l4sc_get_logger("barefootc", 0);
    assert(logger != NULL);
    assert(l4sc_error_enabled(logger));
    assert(l4sc_warn_enabled(logger));
    assert(!l4sc_info_enabled(logger));
    assert(!l4sc_debug_enabled(logger));
    assert(!l4sc_trace_enabled(logger));

    logger = l4sc_get_logger("barefootc.string", 0);
    assert(logger != NULL);
    assert(l4sc_error_enabled(logger));
    assert(!l4sc_warn_enabled(logger));
    assert(!l4sc_info_enabled(logger));
    assert(!l4sc_debug_enabled(logger));
    assert(!l4sc_trace_enabled(logger));

    logger = l4sc_get_logger("barefootc.vector", 0);
    assert(logger != NULL);
    assert(l4sc_error_enabled(logger));
    assert(l4sc_warn_enabled(logger));
    assert(l4sc_info_enabled(logger));
    assert(l4sc_debug_enabled(logger));
    assert(!l4sc_trace_enabled(logger));

    logger = l4sc_get_logger("barefootc.vector.set", 0); /*explicit INHERIT*/
    assert(logger != NULL);
    assert(l4sc_error_enabled(logger));
    assert(l4sc_warn_enabled(logger));
    assert(l4sc_info_enabled(logger));
    assert(l4sc_debug_enabled(logger));
    assert(!l4sc_trace_enabled(logger));

    logger = l4sc_get_logger("barefootc.vector.get", 0); /*implicit INHERIT*/
    assert(logger != NULL);
    assert(l4sc_error_enabled(logger));
    assert(l4sc_warn_enabled(logger));
    assert(l4sc_info_enabled(logger));
    assert(l4sc_debug_enabled(logger));
    assert(!l4sc_trace_enabled(logger));

    logger = l4sc_get_logger("barefootc.vector.mem.alloc.ptrs", 0);
    assert(logger != NULL);
    assert(l4sc_error_enabled(logger));
    assert(l4sc_warn_enabled(logger));
    assert(l4sc_info_enabled(logger));
    assert(l4sc_debug_enabled(logger));
    assert(!l4sc_trace_enabled(logger));

    exit(0);
}
