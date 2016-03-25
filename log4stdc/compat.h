#ifndef _L4SC_COMPAT_H_
#define _L4SC_COMPAT_H_

#include <stddef.h>
#include <stdarg.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* alloca.h */

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>  /* for alloca */

#elif defined(HAVE_ALLOCA_H)
#include <alloca.h>
#endif


/* strncasecmp */

#if defined(_MSC_VER)
#define strncasecmp strnicmp
#endif


/* snprintf, vsnprintf */

int l4sc_snprintf(char *buf, size_t bufsize, const char *fmt, ...);
int l4sc_vsnprintf(char *buf, size_t bufsize, const char *fmt, va_list ap);

#define snprintf  l4sc_snprintf

#if !defined(__STDC__) && !defined(_WIN32) && !defined(HAVE_VSNPRINTF)
#define vsnprintf l4sc_vsnprintf
#endif


/* time.h */

#include <time.h>
#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#endif


/* unistd.h */

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


/* wchar.h */

#if defined(__STDC__) || defined(HAVE_WCHAR_H)
#include <wchar.h>
#endif


/* errno.h */

#ifndef ENOSYS
#define ENOSYS EINVAL
#endif

#endif /* _L4SC_COMPAT_H_ */
