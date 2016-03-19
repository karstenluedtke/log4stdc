
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#include "compat.h"

int
l4sc_snprintf(char *buf, size_t bufsize, const char *fmt, ...)
{
	int rc;
	va_list ap;

	va_start(ap, fmt);

#if defined(__STDC__) || defined(_WIN32) || defined(HAVE_VSNPRINTF)
	rc = vsnprintf(buf, bufsize, fmt, ap);
#else
	rc = l4sc_vsnprintf(buf, bufsize, fmt, ap);
#endif
	va_end(ap);

	return (rc);
}

