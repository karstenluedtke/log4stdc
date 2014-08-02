
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#if defined(_MSC_VER) || defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#define L4SC_USE_WINDOWS_LOCALTIME 1
#endif

#if defined(L4SC_USE_WINDOWS_LOCALTIME)
#include <windows.h>
errno_t _localtime64_s(struct tm *, const __time64_t *);
#endif

#include "logobjects.h"

static int init_patternlayout(void *, size_t, struct mempool *);
static size_t get_layout_size(l4sc_layout_cptr_t obj);
static unsigned get_layout_hashcode(l4sc_layout_cptr_t obj);
static int  is_equal_layout(l4sc_layout_cptr_t obj, l4sc_layout_cptr_t other);
static size_t get_layout_length(l4sc_layout_cptr_t obj);
static int  layout_tostring(l4sc_layout_cptr_t obj, char *buf, size_t bufsize);

static void set_layout_name(l4sc_layout_ptr_t obj, const char *name, int len);
static int  set_layout_option(l4sc_layout_ptr_t obj,
				const char *name,size_t namelen,
				const char *value, size_t vallen);
static int  get_layout_option(l4sc_layout_cptr_t obj,
				const char *name,size_t namelen,
				char *valbuf, size_t bufsize);
static void apply_layout_options(l4sc_layout_ptr_t obj);

static size_t format_by_pattern(l4sc_layout_cptr_t layout,
				l4sc_logmessage_cptr_t msg,
				char *buf, size_t bufsize);

static int format_logtime(char *buf, size_t bufsize, const char *fmt,
		const char *datefmtspec, l4sc_logmessage_cptr_t msg);

const struct l4sc_layout_class l4sc_patternlayout_class = {
	.super = (l4sc_layout_class_ptr_t) &l4sc_object_class,
	.name = "layout",
	.init = init_patternlayout,
	.clonesize = get_layout_size,
	.hashcode = get_layout_hashcode,
	.equals = is_equal_layout,
	.length = get_layout_length,
	.tostring = layout_tostring,

	.set_name = set_layout_name,
	.set_opt = set_layout_option,
	.get_opt = get_layout_option,
	.apply = apply_layout_options,

	.format = format_by_pattern,
};

static int
init_patternlayout(void *buf, size_t bufsize, struct mempool *pool)
{
	BFC_INIT_PROLOGUE(l4sc_layout_class_ptr_t,
			  l4sc_layout_ptr_t, layout, buf, bufsize, pool,
			  &l4sc_patternlayout_class);
	layout->name = "pattern layout";
	strncpy(layout->pattern, "%m%n", sizeof(layout->pattern));
	return (BFC_SUCCESS);
}

static size_t
get_layout_size(l4sc_layout_cptr_t obj)
{
	return (sizeof(struct l4sc_layout));
}

static unsigned  
get_layout_hashcode(l4sc_layout_cptr_t obj)
{
	unsigned x = 0;
	const unsigned char *cp;
	for (cp = (unsigned char *) obj->pattern; *cp; cp++) {
		x = (x << 7) ^ ((x >> (8*sizeof(x)-7)) & 0x7f) ^ *cp;
	}
	return (x);
}

static int
is_equal_layout(l4sc_layout_cptr_t obj, l4sc_layout_cptr_t other)
{
	if (other == obj) {
		return (1);
	} else if (BFC_CLASS(other) == BFC_CLASS(obj)) {
		return (strcmp(other->pattern, obj->pattern) == 0);
	}
	return (0);
}

static size_t
get_layout_length(l4sc_layout_cptr_t obj)
{
	return (0);
}

static int
layout_tostring(l4sc_layout_cptr_t obj, char *buf, size_t bufsize)
{
	if (obj && obj->pattern && buf) {
		size_t patlen = strlen(obj->pattern);
		if (bufsize > patlen) {
			memcpy(buf, obj->pattern, patlen);
			buf[patlen] = 0;
			return((int) patlen);
		}
	}
	return (0);
}


static void
set_layout_name(l4sc_layout_ptr_t obj, const char *name, int len)
{
	if (obj && name) {
		if (len >= sizeof(obj->namebuf)) {
			memcpy(obj->namebuf, name, sizeof(obj->namebuf));
			obj->namebuf[sizeof(obj->namebuf)-1] = 0;
			obj->name = obj->namebuf;
		} else if (len > 0) {
			memcpy(obj->namebuf, name, len);
			obj->namebuf[len] = 0;
			obj->name = obj->namebuf;
		}
	}
}

static int
set_layout_option(l4sc_layout_ptr_t obj, const char *name, size_t namelen,
				     const char *value, size_t vallen)
{
	LOGINFO(("%s: %.*s=\"%.*s\"",__FUNCTION__,
		(int) namelen, name, (int) vallen, value));

	if ((namelen == 17) && (strncasecmp(name,"ConversionPattern",17)==0)) {
		if (vallen < sizeof(obj->pattern)) {
			memcpy(obj->pattern, value, vallen);
			obj->pattern[vallen] = 0;
		} else {
			memcpy(obj->pattern, value, sizeof(obj->pattern)-1);
			obj->pattern[sizeof(obj->pattern)-1] = 0;
		}
	}
	return (0);
}

static int
get_layout_option(l4sc_layout_cptr_t obj, const char *name, size_t namelen,
				     char *valbuf, size_t bufsize)
{
	return (0);
}

static void
apply_layout_options(l4sc_layout_ptr_t obj)
{
}

static size_t
format_by_pattern(l4sc_layout_cptr_t layout,
		  l4sc_logmessage_cptr_t msg,
		  char *buf, size_t bufsize)
{
	char *dp = buf;
	const char *limit = buf + bufsize;
	const char *cp = layout->pattern;
	const char *percent;
	char c;
	size_t len;
	int level = msg? msg->level: 0;
	char fmt[12]; // , tmp[40];

	while ((c = *(cp++)) && (dp < limit)) {
		if (c == '%') {
			percent = cp-1;
			if (*cp == '-') cp++;
			while (((*cp >= '0') && (*cp <= '9')) || (*cp == '.'))
				cp++;
			if (cp < percent + sizeof(fmt) - 2) {
				memcpy(fmt, percent, cp - percent);
				fmt[cp - percent] = 's';
				fmt[cp - percent + 1] = 0;
			} else {
				fmt[0] = '%'; fmt[1] = 's'; fmt[2] = 0;
			}
			len = 0;
			c = *cp++;
			switch (c) {
			case 0: --cp; /* let cp point to '\0' again */
				break;
			case 'c':
			     /* The logger of the logging event.
			      * The logger conversion specifier can be
			      * optionally followed by precision specifier,
			      * that is a decimal constant in brackets.
			      */
				len = snprintf(dp, limit-dp, fmt,
						msg->logger->name);
				if (*cp == '{') {
					const char *sep = strchr(cp, '}');
					if (sep) {
						cp = sep+1;
					}
				}
				break;
			case 'd':
			     /* The date of the logging event.
			      * The date conversion specifier may be followed
			      * by a set of braces containing a pattern string.
			      */
				len = format_logtime(dp,limit-dp,fmt,cp,msg);
				if (*cp == '{') {
					const char *sep = strchr(cp, '}');
					if (sep) {
						cp = sep+1;
					}
				}
				break;
			case 'F':
				len = snprintf(dp, limit-dp, fmt, msg->file);
				break;
			case 'l':
				len = snprintf(dp, limit-dp, "%s(%s:%d)",
					msg->func, msg->file, msg->line);
				break;
			case 'L':
				len = snprintf(dp, limit-dp, "%d", msg->line);
				break;
			case 'm':
				if (msg && ((len = msg->msglen) > 0)) {
					if (dp + len < limit) {
						memcpy(dp, msg->msg, len);
					} else if (dp < limit) {
						len = limit - dp;
						memcpy(dp, msg->msg, len);
					}
				}
				break;
			case 'n':
				if ((dp < buf+1) || (dp[-1] != '\n')) {
#if defined(_MSC_VER) || defined(_WIN32) || defined(__MINGW32__)
					if (dp + 2 < limit) {
						dp[0]='\r'; dp[1]='\n'; len = 2;
					}
#else
					if (dp < limit) {
						*dp = '\n'; len = 1;
					}
#endif
				}
				break;
			case 'p':
				len = snprintf(dp, limit-dp, fmt,
					IS_AT_LEAST_FATAL_LEVEL(level)?"FATAL":
					IS_AT_LEAST_ERROR_LEVEL(level)?"ERROR":
					IS_AT_LEAST_WARN_LEVEL(level)? "WARN":
					IS_AT_LEAST_INFO_LEVEL(level)? "INFO":
					IS_AT_LEAST_DEBUG_LEVEL(level)?"DEBUG":
					"TRACE");
				break;
			case 'r':
				len = snprintf(dp, limit-dp, "%lu",
					1000 * (unsigned long) msg->time.tv_sec
					+ msg->time.tv_usec / 1000);
				break;
			case 't':
				len = snprintf(dp,limit-dp, fmt,msg->threadid);
				break;
			default:
				*dp = c; len = 1;
			}
			if ((len > 0) && (len < (limit - dp))) {
				dp += len;
			}
		} else {
			*(dp++) = c;
		}
	}
	if (dp < limit) {
		*dp = 0;
	}
	return (dp - buf);
}

#if defined(L4SC_USE_WINDOWS_LOCALTIME)
#include <windows.h>
#endif

/*
 * Not yet implemented:
 * The date conversion specifier may be followed by a set of braces containing
 * a date and time pattern string compatible with java.text.SimpleDateFormat,
 * ABSOLUTE, DATE or ISO8601.
 * For example, d{HH:mm:ss,SSS}, d{dd MMM yyyy HH:mm:ss,SSS} or d{DATE}.
 * If no date format specifier is given then ISO8601 format is assumed.
 */

static int
format_logtime(char *buf, size_t bufsize, const char *fmt,
		const char *datefmtspec, l4sc_logmessage_cptr_t msg)
{
	char *dp = buf;
	const char *limit = buf+bufsize;
	int n;
	struct tm tmbuf;
	char tmp[40];

	if ((fmt[1] != 's') || (fmt[2] != 0) || (bufsize < 9)) {
		dp = tmp;
		limit = tmp + sizeof(tmp);
	}
#if defined(L4SC_USE_WINDOWS_LOCALTIME)
	do {
		__time64_t t = msg->time.tv_sec;
		_localtime64_s(&tmbuf, &t);
	} while (0 /* just once */);
#else
	do {
		time_t t = (time_t) msg->time.tv_sec;
		localtime_r(&t, &tmbuf);
	} while (0 /* just once */);
#endif
	n = strftime(dp, limit - dp, "%H:%M:%S", &tmbuf);
	if ((n > 0) && (dp+n < limit)) {
		dp += n;
	}
	if (dp+4 < limit) {
		int n = snprintf(dp, limit-dp, ".%03u",
				(unsigned) (msg->time.tv_usec / 1000));
		if ((n > 0) && (dp+n < limit)) {
			dp += n;
		}
	} else {
		dp += 4;
	}
	if (limit == tmp + sizeof(tmp)) {
		return (snprintf(buf, bufsize, fmt, tmp));
	}
	return ((int) (dp - buf));
}
