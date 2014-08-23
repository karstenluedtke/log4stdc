
#include <stddef.h>

#include "log4stdc.h"
#include "logobjects.h"

int
l4sc_error_enabled(l4sc_logger_cptr_t logger)
{
	return (l4sc_logger_enabled(logger, ERROR_LEVEL));
}

int
l4sc_warn_enabled(l4sc_logger_cptr_t logger)
{
	return (l4sc_logger_enabled(logger, WARN_LEVEL));
}

int
l4sc_info_enabled(l4sc_logger_cptr_t logger)
{
	return (l4sc_logger_enabled(logger, INFO_LEVEL));
}

int
l4sc_debug_enabled(l4sc_logger_cptr_t logger)
{
	return (l4sc_logger_enabled(logger, DEBUG_LEVEL));
}

int
l4sc_trace_enabled(l4sc_logger_cptr_t logger)
{
	return (l4sc_logger_enabled(logger, TRACE_LEVEL));
}

