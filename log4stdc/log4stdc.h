/**
 * @file      log4stdc.h
 *
 * @brief     Public log4stdc definitions and functions.
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-05-10
 *
 * Copyright (c)  2014  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _LOG4STDC_H_
#define _LOG4STDC_H_

#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

struct l4sc_logger;
typedef struct l4sc_logger *l4sc_logger_ptr_t;
typedef const struct l4sc_logger *l4sc_logger_cptr_t;

struct l4sc_appender;
typedef struct l4sc_appender *l4sc_appender_ptr_t;
typedef const struct l4sc_appender *l4sc_appender_cptr_t;

struct l4sc_layout;
typedef struct l4sc_layout *l4sc_layout_ptr_t;
typedef const struct l4sc_layout *l4sc_layout_cptr_t;

struct l4sc_configurator;
typedef struct l4sc_configurator *l4sc_configurator_ptr_t;
typedef const struct l4sc_configurator *l4sc_configurator_cptr_t;

struct l4sc_object_class;
struct l4sc_layout_class;
struct l4sc_appender_class;
struct l4sc_configurator_class;

#define OFF_LEVEL	 60000u
#define FATAL_LEVEL	 50000u
#define ERROR_LEVEL	 40000u
#define INHERIT_LEVEL	 33010u
#define WARN_LEVEL	 30000u
#define INFO_LEVEL	 20000u
#define DEBUG_LEVEL	 10000u
#define TRACE_LEVEL	  5000u
#define ALL_LEVEL	     0

#define IS_AT_LEAST_FATAL_LEVEL(lvl)	((unsigned)(lvl) >= FATAL_LEVEL)
#define IS_AT_LEAST_ERROR_LEVEL(lvl)	((unsigned)(lvl) >= ERROR_LEVEL)
#define IS_AT_LEAST_WARN_LEVEL(lvl)	((unsigned)(lvl) >= WARN_LEVEL)
#define IS_AT_LEAST_INFO_LEVEL(lvl)	((unsigned)(lvl) >= INFO_LEVEL)
#define IS_AT_LEAST_DEBUG_LEVEL(lvl)	((unsigned)(lvl) >= DEBUG_LEVEL)
#define IS_AT_LEAST_TRACE_LEVEL(lvl)	((unsigned)(lvl) >= TRACE_LEVEL)
#define IS_LEVEL_ENABLED(lvl,threshold)	((unsigned)(lvl) >= (unsigned)(threshold))

int l4sc_register_extra_class(const struct l4sc_object_class *cls);

int l4sc_configure_from_file(
	const struct l4sc_configurator_class *configurator,
	const char *path, ...);

int l4sc_configure_from_string(
	const struct l4sc_configurator_class *configurator,
	const char *buf, size_t len, ...);

extern const struct l4sc_configurator_class l4sc_xml_configurator_class;
extern const struct l4sc_configurator_class l4sc_property_configurator_class;

extern const struct l4sc_appender_class l4sc_socket_appender_class;

extern const struct l4sc_layout_class l4sc_log4j_stream_layout_class;
extern const struct l4sc_layout_class l4sc_log4j2_stream_layout_class;
extern const struct l4sc_layout_class l4sc_json_stream_layout_class;
extern const struct l4sc_layout_class l4sc_xml_stream_layout_class;

#ifndef L4SC_EXTRA_CLASSES
#define L4SC_EXTRA_CLASSES \
	&l4sc_socket_appender_class, \
	&l4sc_log4j_stream_layout_class, \
	&l4sc_log4j2_stream_layout_class, \
	&l4sc_json_stream_layout_class, \
	&l4sc_xml_stream_layout_class
#endif

#define l4sc_configure_from_xml_file(path) \
	l4sc_configure_from_file(&l4sc_xml_configurator_class, \
				 path, L4SC_EXTRA_CLASSES, NULL)

#define l4sc_configure_from_xml_string(buf,len) \
	l4sc_configure_from_string(&l4sc_xml_configurator_class, \
				   buf, len, L4SC_EXTRA_CLASSES, NULL)

#define l4sc_configure_from_property_file(path) \
	l4sc_configure_from_file(&l4sc_property_configurator_class, \
				 path, L4SC_EXTRA_CLASSES, NULL)

#define l4sc_configure_from_property_string(buf,len) \
	l4sc_configure_from_string(&l4sc_property_configurator_class, \
				   buf, len, L4SC_EXTRA_CLASSES, NULL)


int l4sc_is_configured(void);
void l4sc_set_configured(int newval);
int l4sc_set_base_directory_name(const char *path);
int l4sc_merge_base_directory_path (char *buf, int bufsize,
	  			    const char *relpath, int rellen);

int l4sc_insert_custom_logger(const char *name, void *cbarg,
	int (*enatest)(void *cbarg, l4sc_logger_cptr_t logger, int level),
	void (*logfunc)(void *cbarg, l4sc_logger_ptr_t logger,
		       int level, const char *msg, size_t msglen,
		       const char *file, int line, const char *func));

l4sc_logger_ptr_t l4sc_get_root_logger(void);
l4sc_logger_ptr_t l4sc_get_logger(const char *name, int namelen);

int l4sc_logger_enabled(l4sc_logger_cptr_t logger, int level);
int l4sc_error_enabled(l4sc_logger_cptr_t logger);
int l4sc_warn_enabled(l4sc_logger_cptr_t logger);
int l4sc_info_enabled(l4sc_logger_cptr_t logger);
int l4sc_debug_enabled(l4sc_logger_cptr_t logger);
int l4sc_trace_enabled(l4sc_logger_cptr_t logger);

#if !defined(L4SC_LOGPRINTF_ATTR)
#if defined(__GNUC__) && ( __GNUC__ >= 3 )
#define L4SC_LOGPRINTF_ATTR  __attribute__((format(printf,6,7)))
#else
#define L4SC_LOGPRINTF_ATTR
#endif
#endif

void l4sc_log(l4sc_logger_ptr_t logger, int level, const char *msg,
	      size_t msglen, const char *file, int line, const char *func);
int l4sc_vlog(l4sc_logger_ptr_t logger, int level, size_t maxbytes, int partial,
	      const char *file, int line, const char *func,
	      const char *fmt, va_list ap);
int l4sc_logprintf(l4sc_logger_ptr_t logger, int level,
	      const char *file, int line, const char *func,
	      const char *fmt, ...) L4SC_LOGPRINTF_ATTR;
int l4sc_snprintf(char *buf, size_t bufsize, const char *fmt, ...);
int l4sc_vsnprintf(char *buf, size_t bufsize, const char *fmt, va_list ap);

l4sc_appender_ptr_t l4sc_get_appender(const char *name, int nlen,
				      const char *kind, int klen);

l4sc_layout_ptr_t l4sc_get_appender_layout(l4sc_appender_ptr_t appender);

void l4sc_close_appenders(void);

int l4sc_to_level(const char *value, int vallen, int defaultlevel);

#ifdef __STDC__
#define L4SC_FATAL(logger,...) \
 l4sc_logprintf(logger,FATAL_LEVEL,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)

#define L4SC_CRITICAL(logger,...) \
 l4sc_logprintf(logger,FATAL_LEVEL,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)

#define L4SC_ERROR(logger,...) \
 l4sc_logprintf(logger,ERROR_LEVEL,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)

#define L4SC_WARN(logger,...) \
 l4sc_logprintf(logger,WARN_LEVEL,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)

#define L4SC_INFO(logger,...) \
 l4sc_logprintf(logger,INFO_LEVEL,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)

#define L4SC_DEBUG(logger,...) \
 l4sc_logprintf(logger,DEBUG_LEVEL,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)

#define L4SC_TRACE(logger,...) \
 l4sc_logprintf(logger,TRACE_LEVEL,__FILE__,__LINE__,__FUNCTION__,__VA_ARGS__)

#endif /* __STDC__ */

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _LOG4STDC_H_ */
