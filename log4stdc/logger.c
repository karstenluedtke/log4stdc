
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

#include "logobjects.h"

#define MAX_APPENDERS_PER_LOGGER 4

static l4sc_logger_ptr_t init_logger(void *buf, size_t bufsize);
static void destroy_logger(l4sc_logger_ptr_t logger);
static void do_not_destroy(l4sc_logger_ptr_t logger);
static size_t	  get_logger_size(l4sc_logger_cptr_t obj);

static void set_logger_name(l4sc_logger_ptr_t obj, const char *name, int len);
static int  set_logger_option(l4sc_logger_ptr_t obj,
				const char *name,size_t namelen,
				const char *value, size_t vallen);
static int  get_logger_option(l4sc_logger_cptr_t obj,
				const char *name,size_t namelen,
				char *valbuf, size_t bufsize);
static void apply_logger_options(l4sc_logger_ptr_t obj);
static void close_logger(l4sc_logger_ptr_t obj);

static void logger_log(l4sc_logger_ptr_t logger,
		       int level, const char *msg, size_t msglen,
		       const char *file, int line, const char *func);
static int  is_logger_enabled(l4sc_logger_ptr_t logger, int level);
static int  set_logger_parent(l4sc_logger_ptr_t logger,
			      l4sc_logger_ptr_t parent);
static int  set_logger_appender(l4sc_logger_ptr_t logger,
				l4sc_appender_ptr_t appender);

static void rootlogger_log(l4sc_logger_ptr_t logger,
		       int level, const char *msg, size_t msglen,
		       const char *file, int line, const char *func);

static const struct l4sc_logger_class loggercls = {
	.super = (const struct l4sc_logger_class *) &l4sc_object_class,
	.name = "logger",
	.init = init_logger,
	.destroy = destroy_logger,
	.clonesize = get_logger_size,

	.set_name = set_logger_name,
	.set_opt = set_logger_option,
	.get_opt = get_logger_option,
	.apply = apply_logger_options,
	.close = close_logger,

	.log = logger_log,
	.is_enabled = is_logger_enabled,
	.set_parent = set_logger_parent,
	.set_appender = set_logger_appender,
};

static const struct l4sc_logger_class rootloggercls = {
	.super = &loggercls,
	.name = "root logger",
	.init = init_logger,
	.destroy = do_not_destroy,
	.clonesize = get_logger_size,

	.set_name = set_logger_name,
	.set_opt = set_logger_option,
	.get_opt = get_logger_option,
	.apply = apply_logger_options,
	.close = close_logger,

	.log = rootlogger_log,
	.is_enabled = is_logger_enabled,
	.set_parent = set_logger_parent,
	.set_appender = set_logger_appender,
};

static struct l4sc_logger rootlogger = {
	.vptr = &rootloggercls,
	.name = "rootlogger",
	.refc = 10000,
	.level = DEBUG_LEVEL, // ERROR_LEVEL,
};

#define MAX_LOGGERS   50
int l4sc_configured  = 0;
int l4sc_num_loggers = 1;
l4sc_logger_ptr_t l4sc_loggers[MAX_LOGGERS] = { &rootlogger, NULL };

static l4sc_logger_ptr_t
init_logger(void *buf, size_t bufsize)
{
	int i;

	BFC_INIT_PROLOGUE(l4sc_logger_class_ptr_t,
			  l4sc_logger_ptr_t, logger, buf, bufsize,
			  &loggercls);

	logger->name = "logger";
	logger->additivity = 1;
	for (i=0; i < l4sc_num_loggers; i++) {
		if (l4sc_loggers[i] == NULL) {
			l4sc_loggers[i] = logger;
			return (logger);
		}
	}
	if (l4sc_num_loggers < MAX_LOGGERS) {
		l4sc_loggers[l4sc_num_loggers++] = logger;
	}
	return ((l4sc_logger_ptr_t) logger);
}

static void
destroy_logger(l4sc_logger_ptr_t logger)
{
	int i;

	for (i=0; i < l4sc_num_loggers; i++) {
		if (l4sc_loggers[i] == logger) {
			l4sc_loggers[i] = NULL;
			if (i == l4sc_num_loggers-1) {
				l4sc_num_loggers = i;
			}
			break;
		}
	}

	BFC_DESTROY_EPILOGUE(logger, &loggercls);
}

static void
do_not_destroy(l4sc_logger_ptr_t logger)
{
}

static size_t
get_logger_size(l4sc_logger_cptr_t obj)
{
	return (sizeof(struct l4sc_logger));
}


static void
set_logger_name(l4sc_logger_ptr_t obj, const char *name, int len)
{
	const char *sep;

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
		for (sep = name + len-2; sep >= name; sep--) {
			if (*sep == '.') {
				break;
			}
		}
		if (sep > name) {
			obj->parent = l4sc_get_logger(name, sep-name);
		} else {
			obj->parent = &rootlogger;
		}
	}
}

static int
set_logger_option(l4sc_logger_ptr_t obj, const char *name, size_t namelen,
				     const char *value, size_t vallen)
{
	LOGINFO(("%s: %.*s=\"%.*s\"",__FUNCTION__,
		(int) namelen, name, (int) vallen, value));
	if ((namelen == 5) && (strncasecmp(name, "level", 5) == 0)) {
		obj->level = l4sc_to_level(value, vallen, ERROR_LEVEL);
		LOGINFO(("%s: %s level set to %d",
				__FUNCTION__, obj->name, obj->level));
	} else if ((namelen == 10) && (strncasecmp(name,"additivity",10)==0)) {
		obj->additivity = (strchr("0FfNn", value[0]) == NULL);
		LOGINFO(("%s: %s additivity set to %d",
				__FUNCTION__, obj->name, obj->additivity));
	}
	return (0);
}

static int
get_logger_option(l4sc_logger_cptr_t obj, const char *name, size_t namelen,
				     char *valbuf, size_t bufsize)
{
	return (0);
}


static void
logger_log(l4sc_logger_ptr_t logger, int level, const char *msg, size_t msglen,
	   const char *file, int line, const char *func)
{
	int i;
	l4sc_logger_ptr_t p;
	l4sc_appender_ptr_t a;
	l4sc_logmessage_ptr_t m;
	l4sc_logmessage_t mbuf;

	m = l4sc_init_logmessage(&mbuf, sizeof(mbuf),
				logger, level, msg, msglen, file, line, func);
	if (m) {
		for (i=0; i < MAX_APPENDERS_PER_LOGGER; i++) {
			if ((a = logger->appenders[i]) != NULL) {
				VMETHCALL(a, append, (a, m), (void) 0);
			}
		}
	}

	if (logger->additivity && (p = logger->parent)) {
		VMETHCALL(p, log, (p, level, msg, msglen, file, line, func),
			  (void) 0);
	}
}

static int
is_logger_enabled(l4sc_logger_ptr_t logger, int level)
{
	return (logger && IS_LEVEL_ENABLED(level, logger->level));
}

static int
set_logger_parent(l4sc_logger_ptr_t logger, l4sc_logger_ptr_t parent)
{
	return (0);
}

static int
set_logger_appender(l4sc_logger_ptr_t logger, l4sc_appender_ptr_t appender)
{
	int i;
	for (i=0; i < MAX_APPENDERS_PER_LOGGER; i++) {
		if (logger->appenders[i] == appender) {
			LOGDEBUG(("%s: %s already appending to %s (class %s)",
				__FUNCTION__, logger->name, appender->name,
				BFC_CLASS(appender)->name));
			return (0);
		}
	}
	for (i=0; i < MAX_APPENDERS_PER_LOGGER; i++) {
		if (logger->appenders[i] == NULL) {
			logger->appenders[i] = appender;
			appender->refc++;
			LOGINFO(("%s: %s appending to %s (class %s) as #%d",
				__FUNCTION__, logger->name, appender->name,
				BFC_CLASS(appender)->name, i));
			return (1);
		}
	}
	LOGINFO(("%s: %s has no free slot for appender %s (class %s)",
		__FUNCTION__, logger->name, appender->name,
		BFC_CLASS(appender)->name));
	return (-ENOSPC);
}

static void
apply_logger_options(l4sc_logger_ptr_t obj)
{
}

static void
close_logger(l4sc_logger_ptr_t obj)
{
}


static void
rootlogger_log(l4sc_logger_ptr_t logger, int level, const char *msg,
	       size_t msglen, const char *file, int line, const char *func)
{
	int rc;
	if (msg && (msglen > 0)) {
		rc = write(2, msg, msglen);
		if ((rc > 0) && (msg[msglen-1] != '\n')) {
			rc = write(2, "\r\n", 2);
		}
	}
}

l4sc_logger_ptr_t
l4sc_get_root_logger(void)
{
	return (&rootlogger);
}

l4sc_logger_ptr_t
l4sc_get_logger(const char *name, int namelen)
{
	l4sc_logger_ptr_t logger = NULL;
	int i, nlen = (namelen > 0)? namelen: strlen(name);

	for (i=0; i < l4sc_num_loggers; i++) {
		if ((logger = l4sc_loggers[i]) != NULL) {
			if ((strncasecmp(logger->name, name, nlen) == 0)
			 && (logger->name[nlen] == '\0')) {
				return (logger);
			}
		}
	}

	LOGINFO(("%s: logger %.*s not found, creating ...",
				__FUNCTION__, nlen, name));
	logger = malloc(sizeof(*logger));
	CMETHCALL(&loggercls, init, (logger, sizeof(*logger)), logger);
	CMETHCALL(&loggercls, set_name, (logger, name, nlen), (void)0);
	LOGINFO(("%s: created %s (class %s).",
				__FUNCTION__, logger->name, loggercls.name));
	return (logger);
}

int
l4sc_set_logger_appender(l4sc_logger_ptr_t logger, l4sc_appender_ptr_t appender)
{
	int rc;

	rc = VMETHCALL(logger, set_appender, (logger, appender), 0);
	return (rc);
}

int
l4sc_to_level(const char *value, int vallen, int defaultlevel)
{
	if (value && (vallen > 0)) {
		switch (value[0]) {
		case 'F': case 'f':
		case 'C': case 'c': return (FATAL_LEVEL);
		case 'E': case 'e': return (ERROR_LEVEL);
		case 'W': case 'w': return (WARN_LEVEL);
		case 'I': case 'i': return (INFO_LEVEL);
		case 'D': case 'd': return (DEBUG_LEVEL);
		case 'T': case 't': return (TRACE_LEVEL);
		default: ;
		}
	}
	return (defaultlevel);
}

void
l4sc_log(l4sc_logger_ptr_t logger, int level, const char *msg,
	 size_t msglen, const char *file, int line, const char *func)
{
	VMETHCALL(logger,log, (logger, level, msg, msglen, file, line, func),
		  (void) 0);
}

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

void
l4sc_logerror(const char *fmt, ...)
{
	int len;
	va_list ap;
	char buf[200];

	va_start(ap, fmt);
	memcpy (buf, "ERROR> ", 8);
	len = 7 + vsnprintf(buf+7, sizeof(buf)-7, fmt, ap);
	if ((len < 7) || (len >= sizeof(buf))) {
		len = strlen(buf);
	}
	va_end(ap);

	VMETHCALL(&rootlogger, log, (&rootlogger, ERROR_LEVEL,
			buf, len, __FILE__, __LINE__, __FUNCTION__),
			(void) 0);
}

void
l4sc_loginfo(const char *fmt, ...)
{
	int len;
	va_list ap;
	char buf[200];

	if (VMETHCALL(&rootlogger, is_enabled, (&rootlogger, INFO_LEVEL), 0)) {
		va_start(ap, fmt);
		memcpy (buf, "INFO > ", 8);
		len = 7 + vsnprintf(buf+7, sizeof(buf)-7, fmt, ap);
		if ((len < 7) || (len >= sizeof(buf))) {
			len = strlen(buf);
		}
		va_end(ap);

		VMETHCALL(&rootlogger, log, (&rootlogger, INFO_LEVEL,
				buf, len, __FILE__, __LINE__, __FUNCTION__),
				(void) 0);
	}
}

int
l4sc_is_configured(void)
{
	return (l4sc_configured);
}

void
l4sc_set_configured(int newval)
{
	l4sc_configured = newval;
}

