
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "logobjects.h"

int
l4sc_vlog(l4sc_logger_ptr_t logger, int level, size_t maxbytes, int partial,
	  const char *file, int line, const char *func,
	  const char *fmt, va_list ap)
{
	int rc;
	size_t len = 0;
	char msg[maxbytes+1];

	if (IS_LEVEL_ENABLED(level, logger->level)) {
		rc = vsnprintf(msg, maxbytes+1, fmt, ap);
		if ((rc < 0) || ((len = (size_t) rc) > maxbytes)) {
			if (partial) {
				len = maxbytes;
			} else {
				return (rc);
			}
		}
		l4sc_log(logger, level, msg, len, file, line, func);
	}
	return ((int) len);
}

