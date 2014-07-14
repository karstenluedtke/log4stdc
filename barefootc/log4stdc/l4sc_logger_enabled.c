
#include <stddef.h>

#include "log4stdc.h"
#include "logobjects.h"

int
l4sc_logger_enabled(l4sc_logger_cptr_t logger, int level)
{
	return (logger && IS_LEVEL_ENABLED(level, logger->level));
}

int
l4sc_error_enabled(l4sc_logger_cptr_t logger)
{
	return (logger && IS_LEVEL_ENABLED(ERROR_LEVEL, logger->level));
}

int
l4sc_warn_enabled(l4sc_logger_cptr_t logger)
{
	return (logger && IS_LEVEL_ENABLED(WARN_LEVEL, logger->level));
}

int
l4sc_info_enabled(l4sc_logger_cptr_t logger)
{
	return (logger && IS_LEVEL_ENABLED(INFO_LEVEL, logger->level));
}

int
l4sc_debug_enabled(l4sc_logger_cptr_t logger)
{
	return (logger && IS_LEVEL_ENABLED(DEBUG_LEVEL, logger->level));
}

int
l4sc_trace_enabled(l4sc_logger_cptr_t logger)
{
	return (logger && IS_LEVEL_ENABLED(TRACE_LEVEL, logger->level));
}

