
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

#include "compat.h"
#include "logobjs.h"
#include "bareftc/object.h"
#include "bareftc/mempool.h"
#include "bareftc/lnkdlist.h"

#if defined(__ANDROID__)
#include <android/log.h>
#endif

#define MAX_LOGGER_RECURSIONS 20
#define MAX_APPENDERS_PER_LOGGER 4

static int
init_logger(void *, size_t, bfc_mempool_t);
static void
destroy_logger(l4sc_logger_ptr_t logger);
static void
do_not_destroy(l4sc_logger_ptr_t logger);
static size_t
get_logger_size(l4sc_logger_cptr_t obj);

static void
set_logger_name(l4sc_logger_ptr_t obj, const char *name, int len);
static int
set_logger_option(l4sc_logger_ptr_t obj, const char *name, size_t namelen,
                  const char *value, size_t vallen);
static int
get_logger_option(l4sc_logger_cptr_t obj, const char *name, size_t namelen,
                  char *valbuf, size_t bufsize);
static void
apply_logger_options(l4sc_logger_ptr_t obj);
static void
close_logger(l4sc_logger_ptr_t obj);

static void
logger_log(l4sc_logger_ptr_t logger, int level, const char *msg, size_t msglen,
           const char *file, int line, const char *func);
static void
logger_append(l4sc_logger_ptr_t logger, l4sc_logmessage_cptr_t msg,
              int recurse);

static int
is_logger_enabled(l4sc_logger_cptr_t logger, int level, int recurse);
static int
set_logger_parent(l4sc_logger_ptr_t logger, l4sc_logger_ptr_t parent);
static int
set_logger_appender(l4sc_logger_ptr_t logger, l4sc_appender_ptr_t appender);

static int
is_rootlogger_enabled(l4sc_logger_cptr_t logger, int level, int recurse);
static int
set_rootlogger_parent(l4sc_logger_ptr_t logger, l4sc_logger_ptr_t parent);

static void
rootlogger_log(l4sc_logger_ptr_t logger, int level, const char *msg,
               size_t msglen, const char *file, int line, const char *func);
static void
rootlogger_append(l4sc_logger_ptr_t logger, l4sc_logmessage_cptr_t msg,
                  int recurse);

static int
customlogger_is_enabled(l4sc_logger_cptr_t logger, int level, int recurse);
static void
customlogger_log(l4sc_logger_ptr_t logger, int level, const char *msg,
                 size_t msglen, const char *file, int line, const char *func);
static void
customlogger_append(l4sc_logger_ptr_t logger, l4sc_logmessage_cptr_t msg,
                    int recurse);

static const struct l4sc_logger_class loggercls = {
    /* .super        */ (const struct l4sc_logger_class *)&l4sc_object_class,
    /* .name         */ "logger",
    /* .spare2       */ NULL,
    /* .spare3       */ NULL,
    /* .init         */ init_logger,
    /* .initrefc     */ (void *)l4sc_default_init_refcount,
    /* .incrrefc     */ (void *)l4sc_default_incr_refcount,
    /* .decrrefc     */ (void *)l4sc_default_decr_refcount,
    /* .destroy      */ destroy_logger,
    /* .clone        */ (void *)l4sc_default_clone_object,
    /* .clonesize    */ get_logger_size,
    /* .compare      */ (void *)l4sc_default_compare_object,
    /* .hashcode     */ (void *)l4sc_default_get_object_hashcode,
    /* .length       */ (void *)l4sc_default_get_object_length,
    /* .tostring     */ (void *)l4sc_default_object_tostring,
    /* .dump         */ (void *)l4sc_default_dump_object,
    /* .set_name     */ set_logger_name,
    /* .set_opt      */ set_logger_option,
    /* .get_opt      */ get_logger_option,
    /* .apply        */ apply_logger_options,
    /* .close        */ close_logger,
    /* .log          */ logger_log,
    /* .is_enabled   */ is_logger_enabled,
    /* .set_parent   */ set_logger_parent,
    /* .set_appender */ set_logger_appender,
    /* .append       */ logger_append};

static const struct l4sc_logger_class rootloggercls = {
    /* .super        */ &loggercls,
    /* .name         */ "root logger",
    /* .spare2       */ NULL,
    /* .spare3       */ NULL,
    /* .init         */ init_logger,
    /* .initrefc     */ (void *)l4sc_default_init_refcount,
    /* .incrrefc     */ (void *)l4sc_default_incr_refcount,
    /* .decrrefc     */ (void *)l4sc_default_decr_refcount,
    /* .destroy      */ do_not_destroy,
    /* .clone        */ (void *)l4sc_default_clone_object,
    /* .clonesize    */ get_logger_size,
    /* .compare      */ (void *)l4sc_default_compare_object,
    /* .hashcode     */ (void *)l4sc_default_get_object_hashcode,
    /* .length       */ (void *)l4sc_default_get_object_length,
    /* .tostring     */ (void *)l4sc_default_object_tostring,
    /* .dump         */ (void *)l4sc_default_dump_object,
    /* .set_name     */ set_logger_name,
    /* .set_opt      */ set_logger_option,
    /* .get_opt      */ get_logger_option,
    /* .apply        */ apply_logger_options,
    /* .close        */ close_logger,
    /* .log          */ rootlogger_log,
    /* .is_enabled   */ is_rootlogger_enabled,
    /* .set_parent   */ set_rootlogger_parent,
    /* .set_appender */ set_logger_appender,
    /* .append       */ rootlogger_append};

static const struct l4sc_logger_class customloggercls = {
    /* .super        */ &loggercls,
    /* .name         */ "custom logger",
    /* .spare2       */ NULL,
    /* .spare3       */ NULL,
    /* .init         */ init_logger,
    /* .initrefc     */ (void *)l4sc_default_init_refcount,
    /* .incrrefc     */ (void *)l4sc_default_incr_refcount,
    /* .decrrefc     */ (void *)l4sc_default_decr_refcount,
    /* .destroy      */ destroy_logger,
    /* .clone        */ (void *)l4sc_default_clone_object,
    /* .clonesize    */ get_logger_size,
    /* .compare      */ (void *)l4sc_default_compare_object,
    /* .hashcode     */ (void *)l4sc_default_get_object_hashcode,
    /* .length       */ (void *)l4sc_default_get_object_length,
    /* .tostring     */ (void *)l4sc_default_object_tostring,
    /* .dump         */ (void *)l4sc_default_dump_object,

    /* .set_name     */ set_logger_name,
    /* .set_opt      */ set_logger_option,
    /* .get_opt      */ get_logger_option,
    /* .apply        */ apply_logger_options,
    /* .close        */ close_logger,

    /* .log          */ customlogger_log,
    /* .is_enabled   */ customlogger_is_enabled,
    /* .set_parent   */ set_logger_parent,
    /* .set_appender */ set_logger_appender,
    /* .append       */ customlogger_append};

#define rootlogger predefined_loggers[0]
#define l4sclogger predefined_loggers[1]

static struct l4sc_logger predefined_loggers[] = {
    {/* .vptr	*/ &rootloggercls,
     /* .name         */ "rootlogger",
     /* .refc	*/ 10000,
     /* .lock	*/ NULL,
     /* .parent_pool	*/ NULL,
     /* .next	*/ &predefined_loggers[1],
     /* .level	*/ ERROR_LEVEL,
     /* .additivity	*/ 0,
     /* .parent	*/ NULL,
     /* .appenders	*/ {NULL, NULL, NULL, NULL}},
    {/* .vptr	*/ &rootloggercls,
     /* .name         */ "l4sclog",
     /* .refc	*/ 10000,
     /* .lock	*/ NULL,
     /* .parent_pool	*/ NULL,
     /* .next	*/ NULL,
     /* .level	*/ WARN_LEVEL,
     /* .additivity	*/ 0,
     /* .parent	*/ &rootlogger,
     /* .appenders	*/ {NULL, NULL, NULL, NULL}}};

int l4sc_configured = 0;

struct loggerpool {
    BFC_LIST_HEAD(l4sc_logger_ptr_t)
};

struct loggerpool l4sc_loggers = {&rootlogger, &l4sclogger};

static int
init_logger(void *buf, size_t bufsize, bfc_mempool_t pool)
{
    BFC_INIT_PROLOGUE(l4sc_logger_class_ptr_t, l4sc_logger_ptr_t, logger, buf,
                      bufsize, pool, &loggercls);

    logger->name = "logger";
    logger->additivity = 1;
    logger->level = INHERIT_LEVEL;
    BFC_SLIST_APPEND(&l4sc_loggers, logger);
    return (BFC_SUCCESS);
}

static void
destroy_logger(l4sc_logger_ptr_t logger)
{
    BFC_SLIST_REMOVE(&l4sc_loggers, logger, l4sc_logger_ptr_t, next);

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
            obj->namebuf[sizeof(obj->namebuf) - 1] = 0;
            obj->name = obj->namebuf;
        } else if (len > 0) {
            memcpy(obj->namebuf, name, len);
            obj->namebuf[len] = 0;
            obj->name = obj->namebuf;
        }
        for (sep = name + len - 2; sep >= name; sep--) {
            if (*sep == '.') {
                break;
            }
        }
        if (sep > name) {
            obj->parent = l4sc_get_logger(name, sep - name);
        } else {
            obj->parent = &rootlogger;
        }
    }
}

static int
set_logger_option(l4sc_logger_ptr_t obj, const char *name, size_t namelen,
                  const char *value, size_t vallen)
{
    static const char thisfunction[] = "set_logger_option";

    LOGINFO(("%s: %.*s=\"%.*s\"", thisfunction, (int)namelen, name,
             (int)vallen, value));
    if ((namelen == 5) && (strncasecmp(name, "level", 5) == 0)) {
        obj->level = l4sc_to_level(value, vallen, ERROR_LEVEL);
        LOGINFO(
            ("%s: %s level set to %u", thisfunction, obj->name, obj->level));
    } else if ((namelen == 10) && (strncasecmp(name, "additivity", 10) == 0)) {
        obj->additivity = (strchr("0FfNn", value[0]) == NULL);
        LOGINFO(("%s: %s additivity set to %d", thisfunction, obj->name,
                 obj->additivity));
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
logger_append(l4sc_logger_ptr_t logger, l4sc_logmessage_cptr_t msg,
              int recurse)
{
    int i;
    l4sc_logger_ptr_t p;
    l4sc_appender_ptr_t a;

    for (i = 0; i < MAX_APPENDERS_PER_LOGGER; i++) {
        if ((a = logger->appenders[i]) != NULL) {
            VOID_METHCALL(l4sc_appender_class_ptr_t, a, append, (a, msg));
        }
    }

    if (logger->additivity && (p = logger->parent) && (recurse > 0)) {
        VOID_METHCALL(l4sc_logger_class_ptr_t, p, append,
                      (p, msg, recurse - 1));
    }
}

static void
logger_log(l4sc_logger_ptr_t logger, int level, const char *msg, size_t msglen,
           const char *file, int line, const char *func)
{
    int rc;
    l4sc_logmessage_t mbuf;

    rc = l4sc_init_logmessage(&mbuf, sizeof(mbuf), logger, level, msg, msglen,
                              file, line, func);
    if (rc >= 0) {
        VOID_METHCALL(l4sc_logger_class_ptr_t, logger, append,
                      (logger, &mbuf, MAX_LOGGER_RECURSIONS));
    }
}

static int
is_logger_enabled(l4sc_logger_cptr_t logger, int level, int recurse)
{
    unsigned loglevel = (unsigned)level;

    if (!logger) {
        return (0);
    }
    if ((logger->level == INHERIT_LEVEL) && (recurse > 0)) {
        l4sc_logger_cptr_t p = logger->parent;
        int rc, loops = 0;
        while (p && (loops++ < MAX_LOGGER_RECURSIONS)) {
            RETVAR_METHCALL(rc, l4sc_logger_class_ptr_t, p, is_enabled,
                            (p, level, recurse - 1), -ENOSYS);
            if (rc != -ENOSYS) {
                return (rc);
            } else if (p->level != INHERIT_LEVEL) {
                return (IS_LEVEL_ENABLED(loglevel, p->level));
            }
            p = p->parent;
        }
    }
    return (IS_LEVEL_ENABLED(loglevel, logger->level));
}

static int
set_logger_parent(l4sc_logger_ptr_t logger, l4sc_logger_ptr_t parent)
{
    if (parent) {
        logger->parent = parent;
    } else {
        logger->parent = &rootlogger;
    }
    return (0);
}

static int
remove_logger_appender(l4sc_logger_ptr_t logger, l4sc_appender_ptr_t appender)
{
    int i, removed = 0;
    static const char thisfunction[] = "remove_logger_appender";

    for (i = 0; i < MAX_APPENDERS_PER_LOGGER; i++) {
        if (logger->appenders[i] &&
            ((appender == NULL) || (logger->appenders[i] == appender))) {
            LOGINFO(("%s: %s clearing %p (class %s) at #%d", thisfunction,
                     logger->name, logger->appenders[i],
                     BFC_CLASS(logger->appenders[i])->name, i));
            logger->appenders[i]->refc--;
            logger->appenders[i] = NULL;
            removed++;
        }
    }
    return (removed);
}

static int
set_logger_appender(l4sc_logger_ptr_t logger, l4sc_appender_ptr_t appender)
{
    int i;
    static const char thisfunction[] = "set_logger_appender";
    extern const char obsolete_appender_name[]; /* in appender.c */

    if (appender == NULL) {
        return remove_logger_appender(logger, NULL);
    }

    for (i = 0; i < MAX_APPENDERS_PER_LOGGER; i++) {
        if (logger->appenders[i] &&
            ((logger->appenders[i]->name == NULL) ||
             (logger->appenders[i]->name == obsolete_appender_name))) {
            LOGINFO(("%s: %s clearing %p (class %s) at #%d", thisfunction,
                     logger->name, logger->appenders[i],
                     BFC_CLASS(logger->appenders[i])->name, i));
            logger->appenders[i]->refc--;
            logger->appenders[i] = NULL;
        }
    }
    for (i = 0; i < MAX_APPENDERS_PER_LOGGER; i++) {
        if (logger->appenders[i] == appender) {
            LOGDEBUG(("%s: %s already appending to %s (class %s)",
                      thisfunction, logger->name, appender->name,
                      BFC_CLASS(appender)->name));
            return (0);
        }
    }
    for (i = 0; i < MAX_APPENDERS_PER_LOGGER; i++) {
        if (logger->appenders[i] == NULL) {
            logger->appenders[i] = appender;
            appender->refc++;
            LOGINFO(("%s: %s appending to %s (class %s) as #%d", thisfunction,
                     logger->name, appender->name, BFC_CLASS(appender)->name,
                     i));
            return (1);
        }
    }
    LOGINFO(("%s: %s has no free slot for appender %s (class %s)",
             thisfunction, logger->name, appender->name,
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

static int
set_rootlogger_parent(l4sc_logger_ptr_t logger, l4sc_logger_ptr_t parent)
{
    LOGERROR(("cannot set rootlogger parent"));
    return (-EINVAL);
}

static int
is_rootlogger_enabled(l4sc_logger_cptr_t logger, int level, int recurse)
{
    unsigned loglevel = (unsigned)level;

    if (!logger) {
        return (0);
    }
    return (IS_LEVEL_ENABLED(loglevel, logger->level));
}

static void
rootlogger_log(l4sc_logger_ptr_t logger, int level, const char *msg,
               size_t msglen, const char *file, int line, const char *func)
{
    if (msg && (msglen > 0)) {
#if defined(__ANDROID__)
        const char *tag = logger->name;
        android_LogPriority prio =
            IS_AT_LEAST_FATAL_LEVEL(level)   ? ANDROID_LOG_FATAL
            : IS_AT_LEAST_ERROR_LEVEL(level) ? ANDROID_LOG_ERROR
            : IS_AT_LEAST_WARN_LEVEL(level)  ? ANDROID_LOG_WARN
            : IS_AT_LEAST_INFO_LEVEL(level)  ? ANDROID_LOG_INFO
            : IS_AT_LEAST_DEBUG_LEVEL(level) ? ANDROID_LOG_DEBUG
                                             : ANDROID_LOG_VERBOSE;
        __android_log_print(prio, tag, "%.*s", (int)msglen, msg);
#else
        int rc = write(2, msg, msglen);
        if ((rc > 0) && (msg[msglen - 1] != '\n')) {
            rc = write(2, "\r\n", 2);
        }
#endif
    }
}

static void
rootlogger_append(l4sc_logger_ptr_t logger, l4sc_logmessage_cptr_t msg,
                  int recurse)
{
    if (msg && (msg->msglen > 0)) {
#if defined(__ANDROID__)
        const unsigned level = msg->level;
        const char *tag = msg->logger->name;
        android_LogPriority prio =
            IS_AT_LEAST_FATAL_LEVEL(level)   ? ANDROID_LOG_FATAL
            : IS_AT_LEAST_ERROR_LEVEL(level) ? ANDROID_LOG_ERROR
            : IS_AT_LEAST_WARN_LEVEL(level)  ? ANDROID_LOG_WARN
            : IS_AT_LEAST_INFO_LEVEL(level)  ? ANDROID_LOG_INFO
            : IS_AT_LEAST_DEBUG_LEVEL(level) ? ANDROID_LOG_DEBUG
                                             : ANDROID_LOG_VERBOSE;
        __android_log_print(prio, tag, "%.*s", (int)msg->msglen, msg->msg);
#else
        int rc = write(2, msg->msg, msg->msglen);
        if ((rc > 0) && (msg->msg[msg->msglen - 1] != '\n')) {
            rc = write(2, "\r\n", 2);
        }
#endif
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
    int rc, nlen = (namelen > 0) ? namelen : strlen(name);
    bfc_mempool_t pool = get_default_mempool();
    size_t size;
    static const char thisfunction[] = "l4sc_get_logger";

    BFC_LIST_FOREACH(logger, &l4sc_loggers, next)
    {
        if ((strncasecmp(logger->name, name, nlen) == 0) &&
            (logger->name[nlen] == '\0')) {
            return (logger);
        }
    }

    LOGINFO(
        ("%s: logger %.*s not found, creating ...", thisfunction, nlen, name));
    size = get_logger_size(NULL);
    logger = mempool_alloc(pool, size);
    if (logger != NULL) {
        rc = init_logger(logger, size, pool);
        if (rc >= 0) {
            logger->parent_pool = pool;
            VOID_METHCALL(l4sc_logger_class_ptr_t, logger, initrefc,
                          (logger, 1));
            set_logger_name(logger, name, nlen);
            LOGINFO(("%s: created %s (class %s).", thisfunction, logger->name,
                     loggercls.name));
        }
    }
    return (logger);
}

l4sc_logger_ptr_t
l4sc_get_sublogger(l4sc_logger_ptr_t parent, const char *name, int namelen)
{
    int len = strlen(parent->name);
    int nlen = (namelen > 0) ? namelen : strlen(name);
    int bufsize = len + nlen + 2;
    char *buf;
    l4sc_logger_ptr_t logger = NULL;

    buf = alloca(bufsize);

    memcpy(buf, parent->name, len);
    if (buf[len - 1] != '.') {
        buf[len++] = '.';
    }

    memcpy(buf + len, name, nlen);
    len += nlen;
    if (len < bufsize) {
        buf[len] = '\0';
    } else {
        LOGERROR(
            ("l4sc_get_sublogger: name buffer overrun %d/%d", len, bufsize));
    }

    logger = l4sc_get_logger(buf, len);
    if (logger) {
        logger->parent = parent;
    }

    return (logger);
}

int
l4sc_insert_custom_logger(
    const char *name, void *cbarg,
    int (*enatest)(void *cbarg, l4sc_logger_cptr_t logger, int level),
    void (*logfunc)(void *cbarg, l4sc_logger_ptr_t logger, int level,
                    const char *msg, size_t msglen, const char *file, int line,
                    const char *func))
{
    l4sc_logger_ptr_t logger = l4sc_get_logger(name, 0);
    if (logger) {
        logger->vptr = &customloggercls;
        logger->cxxbuf.p[0] = cbarg;
        logger->cxxbuf.p[1] = enatest;
        logger->cxxbuf.p[2] = logfunc;
    }
    return (BFC_SUCCESS);
}

static int
customlogger_is_enabled(l4sc_logger_cptr_t logger, int level, int recurse)
{
    int (*enatest)(void *cbarg, l4sc_logger_cptr_t logger, int level);
    int enabled = 1;

    if ((enatest = logger->cxxbuf.p[1]) != NULL) {
        enabled = (*enatest)(logger->cxxbuf.p[0], logger, level);
    }
    return (enabled);
}

static void
customlogger_log(l4sc_logger_ptr_t logger, int level, const char *msg,
                 size_t msglen, const char *file, int line, const char *func)
{
    void (*logfunc)(void *cbarg, l4sc_logger_ptr_t logger, int level,
                    const char *msg, size_t msglen, const char *file, int line,
                    const char *func);

    if ((logfunc = logger->cxxbuf.p[2]) && msg && (msglen > 0)) {
        (*logfunc)(logger->cxxbuf.p[0], logger, level, msg, msglen, file, line,
                   func);
    }
}

static void
customlogger_append(l4sc_logger_ptr_t logger, l4sc_logmessage_cptr_t msg,
                    int recurse)
{
    void (*logfunc)(void *cbarg, l4sc_logger_cptr_t logger, int level,
                    const char *msg, size_t msglen, const char *file, int line,
                    const char *func);

    if ((logfunc = logger->cxxbuf.p[2]) && msg && (msg->msglen > 0)) {
        (*logfunc)(logger->cxxbuf.p[0], msg->logger, msg->level, msg->msg,
                   msg->msglen, msg->file, msg->line, msg->func);
    }
}

int
l4sc_is_logger_enabled(l4sc_logger_cptr_t logger, int level)
{
    RETURN_METHCALL(l4sc_logger_class_ptr_t, logger, is_enabled,
                    (logger, level, MAX_LOGGER_RECURSIONS), 0);
}

int
l4sc_to_level(const char *value, int vallen, int defaultlevel)
{
    if (value && (vallen > 0)) {
        switch (value[0]) {
        case 'F':
        case 'f':
        case 'C':
        case 'c':
            return (FATAL_LEVEL);
        case 'E':
        case 'e':
            return (ERROR_LEVEL);
        case 'W':
        case 'w':
            return (WARN_LEVEL);
        case 'I':
        case 'i':
            if ((vallen >= 3) && (strncasecmp(value, "inherit", 3) == 0)) {
                return (INHERIT_LEVEL);
            } else {
                return (INFO_LEVEL);
            }
        case 'D':
        case 'd':
            return (DEBUG_LEVEL);
        case 'T':
        case 't':
            return (TRACE_LEVEL);
        case 'O':
        case 'o':
            return (OFF_LEVEL);
        default:;
        }
    }
    return (defaultlevel);
}

void
l4sc_set_internal_logging(const char *value, int vallen)
{
    if (value && (vallen > 0)) {
        switch (value[0]) {
        case 'T':
        case 't':
        case '1': /* TRUE */
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            l4sclogger.level = DEBUG_LEVEL;
            break;
        case 'F':
        case 'f':
        case '0': /* FALSE */
            l4sclogger.level = WARN_LEVEL;
            break;
        default:
            l4sclogger.level = l4sc_to_level(value, vallen, l4sclogger.level);
        }
    }
}

#if !defined(__STDC__) && !defined(_WIN32) && !defined(HAVE_VSNPRINTF)
#define vsnprintf l4sc_vsnprintf
#endif

void
l4sc_logerror(const char *fmt, ...)
{
    int len;
    va_list ap;
    char buf[200];
    static const char thisfunction[] = "l4sc_logerror";

    va_start(ap, fmt);
    memcpy(buf, "ERROR> ", 8);
    len = 7 + vsnprintf(buf + 7, sizeof(buf) - 7, fmt, ap);
    if ((len < 7) || (len >= sizeof(buf))) {
        len = strlen(buf);
    }
    va_end(ap);

    (*l4sclogger.vptr->log)(&l4sclogger, ERROR_LEVEL, buf, len, __FILE__,
                            __LINE__, thisfunction);
}

void
l4sc_logwarn(const char *fmt, ...)
{
    int len;
    va_list ap;
    char buf[200];
    static const char thisfunction[] = "l4sc_logwarn";

    if (IS_LEVEL_ENABLED(WARN_LEVEL, l4sclogger.level)) {
        va_start(ap, fmt);
        memcpy(buf, "WARN > ", 8);
        len = 7 + vsnprintf(buf + 7, sizeof(buf) - 7, fmt, ap);
        if ((len < 7) || (len >= sizeof(buf))) {
            len = strlen(buf);
        }
        va_end(ap);

        (*l4sclogger.vptr->log)(&l4sclogger, WARN_LEVEL, buf, len, __FILE__,
                                __LINE__, thisfunction);
    }
}

void
l4sc_loginfo(const char *fmt, ...)
{
    int len;
    va_list ap;
    char buf[200];
    static const char thisfunction[] = "l4sc_loginfo";

    if (IS_LEVEL_ENABLED(INFO_LEVEL, l4sclogger.level)) {
        va_start(ap, fmt);
        memcpy(buf, "INFO > ", 8);
        len = 7 + vsnprintf(buf + 7, sizeof(buf) - 7, fmt, ap);
        if ((len < 7) || (len >= sizeof(buf))) {
            len = strlen(buf);
        }
        va_end(ap);

        (*l4sclogger.vptr->log)(&l4sclogger, INFO_LEVEL, buf, len, __FILE__,
                                __LINE__, thisfunction);
    }
}

void
l4sc_logdebug(const char *fmt, ...)
{
    int len;
    va_list ap;
    char buf[200];
    static const char thisfunction[] = "l4sc_logdebug";

    if (IS_LEVEL_ENABLED(DEBUG_LEVEL, l4sclogger.level)) {
        va_start(ap, fmt);
        memcpy(buf, "DEBUG> ", 8);
        len = 7 + vsnprintf(buf + 7, sizeof(buf) - 7, fmt, ap);
        if ((len < 7) || (len >= sizeof(buf))) {
            len = strlen(buf);
        }
        va_end(ap);

        (*l4sclogger.vptr->log)(&l4sclogger, DEBUG_LEVEL, buf, len, __FILE__,
                                __LINE__, thisfunction);
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
