/**
 * @file      logobjects.h
 *
 * @brief     Private log4stdc definitions.
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-05-10
 *
 * Copyright (c)  2014  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _LOGOBJECTS_H_
#define _LOGOBJECTS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "log4stdc.h"
#include "barefootc/object.h"

#if defined(_MSC_VER)
#include <stdint.h>
#else
#include <inttypes.h>
#endif

struct l4sc_object;
typedef struct l4sc_object *l4sc_objptr_t;
typedef const struct l4sc_object *l4sc_objcptr_t;

#define LOGOBJ_METHODS(objptrT,objcptrT) \
	void	(*set_name)(objptrT obj, const char *name, int len);	\
	int	(*set_opt)(objptrT obj,					\
			   const char *name, size_t namelen,		\
			   const char *value, size_t vallen);		\
	int	(*get_opt)(objcptrT obj,				\
			   const char *name, size_t namelen,		\
			   char *valbuf, size_t bufsize);		\
	void	(*apply)(objptrT obj);					\
	void	(*close)(objptrT obj);

struct l4sc_object_class;
typedef const struct l4sc_object_class *l4sc_class_ptr_t;

struct l4sc_object_class {
	BFC_CLASSHDR(l4sc_class_ptr_t,l4sc_objptr_t,l4sc_objcptr_t)
	LOGOBJ_METHODS(l4sc_objptr_t,l4sc_objcptr_t)
	void *spare[10];
};

extern const struct l4sc_object_class l4sc_object_class;

struct l4sc_logger_class;
typedef const struct l4sc_logger_class *l4sc_logger_class_ptr_t;

struct l4sc_logger_class {
	BFC_CLASSHDR(l4sc_logger_class_ptr_t,
		     l4sc_logger_ptr_t,l4sc_logger_cptr_t)
	LOGOBJ_METHODS(l4sc_logger_ptr_t,l4sc_logger_cptr_t)
	void	(*log)(l4sc_logger_ptr_t logger,
		       int level, const char *msg, size_t msglen,
		       const char *file, int line, const char *func);
	int	(*is_enabled)(l4sc_logger_cptr_t logger, int level);
	int	(*set_parent)(l4sc_logger_ptr_t logger,
			      l4sc_logger_ptr_t parent);
	int	(*set_appender)(l4sc_logger_ptr_t logger,
				l4sc_appender_ptr_t appender);
};

struct l4sc_logmessage;
typedef struct l4sc_logmessage l4sc_logmessage_t;
typedef struct l4sc_logmessage *l4sc_logmessage_ptr_t;
typedef const struct l4sc_logmessage *l4sc_logmessage_cptr_t;

struct l4sc_appender_class;
typedef const struct l4sc_appender_class *l4sc_appender_class_ptr_t;

struct l4sc_appender_class {
	BFC_CLASSHDR(l4sc_appender_class_ptr_t,
		   l4sc_appender_ptr_t,l4sc_appender_cptr_t)
	LOGOBJ_METHODS(l4sc_appender_ptr_t,l4sc_appender_cptr_t)
	void	(*append)(l4sc_appender_ptr_t appender,
			  l4sc_logmessage_cptr_t msg);
	int	(*set_layout)(l4sc_appender_ptr_t appender,
			      l4sc_layout_ptr_t layout);
	l4sc_layout_ptr_t (*ref_layout)(l4sc_appender_ptr_t appender);
};

extern const struct l4sc_appender_class l4sc_file_appender_class;
extern const struct l4sc_appender_class l4sc_sysout_appender_class;

struct l4sc_layout_class;
typedef const struct l4sc_layout_class *l4sc_layout_class_ptr_t;

struct l4sc_layout_class {
	BFC_CLASSHDR(l4sc_layout_class_ptr_t,
		   l4sc_layout_ptr_t,l4sc_layout_cptr_t)
	LOGOBJ_METHODS(l4sc_layout_ptr_t,l4sc_layout_cptr_t)
	size_t	(*format)(l4sc_layout_cptr_t layout,
			  l4sc_logmessage_cptr_t msg,
			  char *buf, size_t bufsize);
};

extern const struct l4sc_layout_class l4sc_patternlayout_class;

struct l4sc_layout {
	BFC_OBJHDR(l4sc_layout_class_ptr_t,l4sc_layout_ptr_t)
	char namebuf[40];
	char pattern[80];
};

struct l4sc_appender {
	BFC_OBJHDR(l4sc_appender_class_ptr_t,l4sc_appender_ptr_t)
	struct l4sc_layout layout;
	union {
		void *fh;
		int fd;
	} fu;
	unsigned long filesize;
	unsigned long maxfilesize;
	unsigned maxbackupindex;
	const char *filename;
	char namebuf[40];
	char pathbuf[160];
};

struct l4sc_logger {
	BFC_OBJHDR(l4sc_logger_class_ptr_t,l4sc_logger_ptr_t)
	int level;
	int additivity;
	l4sc_logger_ptr_t parent;
	l4sc_appender_ptr_t appenders[4];
	char namebuf[40];
	union {
		long l[32];
		void *p[32];
		char c[128];
	} cxxbuf;
};

struct l4sc_logmessage_class;
typedef const struct l4sc_logmessage_class *l4sc_logmessage_class_ptr_t;

struct l4sc_logmessage_class {
	BFC_CLASSHDR(l4sc_logmessage_class_ptr_t,
		   l4sc_logmessage_ptr_t, l4sc_logmessage_cptr_t)
};

struct l4sc_logmessage {
	BFC_OBJHDR(l4sc_logmessage_class_ptr_t,l4sc_logmessage_ptr_t)
	l4sc_logger_cptr_t logger;
	int level;
	size_t msglen;
	const char *msg;
	const char *func;
	const char *file;
	int line;
	struct l4sc_timeval {
		int64_t  tv_sec; /* seconds since Jan 1st, 1970 UTC */
		uint32_t tv_usec;
	} time;
	char threadid[32];
};

struct l4sc_configurator_class;
typedef const struct l4sc_configurator_class *l4sc_configurator_class_ptr_t;

struct l4sc_configurator_class {
	BFC_CLASSHDR(l4sc_configurator_class_ptr_t,
		   l4sc_configurator_ptr_t,l4sc_configurator_cptr_t)
	LOGOBJ_METHODS(l4sc_configurator_ptr_t,l4sc_configurator_cptr_t)
	int	(*configure)(l4sc_configurator_ptr_t configurator,
			     const char *buf, size_t len);
	int	(*configure_from_file)(l4sc_configurator_ptr_t configurator,
			     const char *path);
};

struct l4sc_configurator {
	BFC_OBJHDR(l4sc_configurator_class_ptr_t,l4sc_configurator_ptr_t)
};

int  l4sc_default_init_object(void *buf, size_t bufsize, struct mempool *pool);
void l4sc_default_destroy_object(l4sc_objptr_t obj);
int  l4sc_default_clone_object(l4sc_objcptr_t obj, void *buf, size_t bufsize);
size_t l4sc_default_get_object_size(l4sc_objcptr_t obj);
unsigned l4sc_default_get_object_hashcode(l4sc_objcptr_t obj);
int l4sc_default_is_equal_object(l4sc_objcptr_t obj, l4sc_objcptr_t other);
size_t l4sc_default_get_object_length(l4sc_objcptr_t obj);
int l4sc_default_object_tostring(l4sc_objcptr_t obj, char *buf, size_t bufsize);

void l4sc_default_set_object_name(l4sc_objptr_t obj, const char *name, int len);
int l4sc_default_set_object_option(l4sc_objptr_t obj,
				const char *name,size_t namelen,
				const char *value, size_t vallen);
int l4sc_default_get_object_option(l4sc_objcptr_t obj,
				const char *name,size_t namelen,
				char *valbuf, size_t bufsize);
void l4sc_default_apply_object_options(l4sc_objptr_t obj);
void l4sc_default_close_object(l4sc_objptr_t obj);

int l4sc_set_object_option(l4sc_objptr_t obj,
			const char *name, size_t namelen,
			const char *value, size_t vallen);

int l4sc_set_logger_appender(l4sc_logger_ptr_t logger,
			     l4sc_appender_ptr_t appender);

int l4sc_init_logmessage(void *buf, size_t bufsize,
		l4sc_logger_cptr_t, int level, const char *msg, size_t msglen,
		const char *file, int line, const char *func);
int l4sc_formatmsg(l4sc_layout_cptr_t layout,
	  	l4sc_logmessage_cptr_t msg, char *buf, size_t bufsize);

void l4sc_set_internal_logging(const char *value, int vallen);

#if defined(__GNUC__) && (__GNUC__ >= 3)
void l4sc_logerror(const char *fmt, ...) __attribute__((format(printf,1,2)));
void l4sc_logwarn (const char *fmt, ...) __attribute__((format(printf,1,2)));
void l4sc_loginfo (const char *fmt, ...) __attribute__((format(printf,1,2)));
void l4sc_logdebug(const char *fmt, ...) __attribute__((format(printf,1,2)));
#else
void l4sc_logerror(const char *fmt, ...);
void l4sc_logwarn (const char *fmt, ...);
void l4sc_loginfo (const char *fmt, ...);
void l4sc_logdebug(const char *fmt, ...);
#endif
#define LOGERROR(x)	l4sc_logerror x
#define LOGWARN(x)	l4sc_logwarn  x
#define LOGINFO(x)	l4sc_loginfo  x
#define LOGDEBUG(x)	l4sc_logdebug x

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _LOGOBJECTS_H_ */
