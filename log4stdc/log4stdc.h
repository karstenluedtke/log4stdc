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

#define OFF_LEVEL	 60000
#define FATAL_LEVEL	 50000
#define ERROR_LEVEL	 40000
#define WARN_LEVEL	 30000
#define INFO_LEVEL	 20000
#define DEBUG_LEVEL	 10000
#define TRACE_LEVEL	  5000
#define ALL_LEVEL	     0

#define IS_AT_LEAST_FATAL_LEVEL(lvl)	((lvl) >= FATAL_LEVEL)
#define IS_AT_LEAST_ERROR_LEVEL(lvl)	((lvl) >= ERROR_LEVEL)
#define IS_AT_LEAST_WARN_LEVEL(lvl)	((lvl) >= WARN_LEVEL)
#define IS_AT_LEAST_INFO_LEVEL(lvl)	((lvl) >= INFO_LEVEL)
#define IS_AT_LEAST_DEBUG_LEVEL(lvl)	((lvl) >= DEBUG_LEVEL)
#define IS_AT_LEAST_TRACE_LEVEL(lvl)	((lvl) >= TRACE_LEVEL)
#define IS_LEVEL_ENABLED(lvl,threshold)	((lvl) >= (threshold))

int l4sc_configure_from_xml_file(const char *path);
int l4sc_configure_from_xml_string(const char *buf, size_t len);
int l4sc_configure_from_property_file(const char *path);
int l4sc_configure_from_property_string(const char *buf, size_t len);

int l4sc_is_configured(void);
void l4sc_set_configured(int newval);
int l4sc_set_base_directory_name(const char *path);
int l4sc_merge_base_directory_path (char *buf, int bufsize,
	  			    const char *relpath, int rellen);

l4sc_logger_ptr_t l4sc_get_root_logger(void);
l4sc_logger_ptr_t l4sc_get_logger(const char *name, int namelen);

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

l4sc_appender_ptr_t l4sc_get_appender(const char *name, int nlen,
				      const char *kind, int klen);

l4sc_layout_ptr_t l4sc_get_appender_layout(l4sc_appender_ptr_t appender);

int l4sc_to_level(const char *value, int vallen, int defaultlevel);

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

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _LOG4STDC_H_ */