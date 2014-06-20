
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "logobjects.h"

int
l4sc_logprintf(l4sc_logger_ptr_t logger, int level,
	 const char *file, int line, const char *func, const char *fmt, ...)
{
	int rc = 0;
	va_list ap;

	if (IS_LEVEL_ENABLED(level, logger->level)) {
		int estimate = 200 * strlen(fmt);
		if (file) estimate += strlen(file);
		if (func) estimate += strlen(func);

		do {
			va_start(ap, fmt);
			rc = l4sc_vlog(logger, level, estimate, 0,
						file, line, func, fmt, ap);
			va_end(ap);
			if (rc > estimate) {
				estimate = rc + 80;
			} else if (rc < 0) /* Microsoft vsnprintf ... */ {
				estimate += 4*estimate + 200;
			} else {
				return (rc); /* success */
			}
		} while ((rc < 0) && (estimate < 64000));
	
		/* final attempt, setting partial flag */
		va_start(ap, fmt);
		rc = l4sc_vlog(logger, level, estimate, 1,
					file, line, func, fmt, ap);
		va_end(ap);
	}

	return (rc);
}

