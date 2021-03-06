
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "compat.h"
#include "logobjs.h"

static int
init_json_stream_layout(void *, size_t, bfc_mempool_t);
static size_t
get_layout_size(l4sc_layout_cptr_t obj);

static void
set_layout_name(l4sc_layout_ptr_t obj, const char *name, int len);
static int
set_layout_option(l4sc_layout_ptr_t obj, const char *name, size_t namelen,
                  const char *value, size_t vallen);
static int
get_layout_option(l4sc_layout_cptr_t obj, const char *name, size_t namelen,
                  char *valbuf, size_t bufsize);
static void
apply_layout_options(l4sc_layout_ptr_t obj);

static size_t
format_json_message(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
                    char *buf, size_t bufsize);

static size_t
format_header(l4sc_layout_ptr_t layout, int kind, char *buf, size_t bufsize);

static size_t
estimate_json_size(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg);

const struct l4sc_layout_class l4sc_json_stream_layout_class = {
    /* .super        */ (l4sc_layout_class_ptr_t)&l4sc_object_class,
    /* .name         */ "JsonLayout",
    /* .spare2       */ NULL,
    /* .spare3       */ NULL,
    /* .init         */ init_json_stream_layout,
    /* .initrefc     */ (void *)l4sc_default_init_refcount,
    /* .incrrefc     */ (void *)l4sc_default_incr_refcount,
    /* .decrrefc     */ (void *)l4sc_default_decr_refcount,
    /* .destroy      */ NULL, /* inherit */
    /* .clone        */ (void *)l4sc_default_clone_object,
    /* .clonesize    */ get_layout_size,
    /* .compare      */ NULL, /* inherit */
    /* .hashcode     */ NULL, /* inherit */
    /* .length       */ NULL, /* inherit */
    /* .tostring     */ NULL, /* inherit */
    /* .dump         */ NULL, /* inherit */
    /* .set_name     */ set_layout_name,
    /* .set_opt      */ set_layout_option,
    /* .get_opt      */ get_layout_option,
    /* .apply        */ apply_layout_options,
    /* .close        */ NULL,
    /* .format       */ format_json_message,
    /* .header       */ format_header,
    /* .estimate     */ estimate_json_size};

static int
init_json_stream_layout(void *buf, size_t bufsize, bfc_mempool_t pool)
{
    BFC_INIT_PROLOGUE(l4sc_layout_class_ptr_t, l4sc_layout_ptr_t, layout, buf,
                      bufsize, pool, &l4sc_json_stream_layout_class);
    layout->name = "json stream layout";
    return (BFC_SUCCESS);
}

static size_t
get_layout_size(l4sc_layout_cptr_t obj)
{
    return (sizeof(struct l4sc_layout));
}

static void
set_layout_name(l4sc_layout_ptr_t obj, const char *name, int len)
{
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
    }
}

static int
set_layout_option(l4sc_layout_ptr_t obj, const char *name, size_t namelen,
                  const char *value, size_t vallen)
{
    LOGINFO(("%s: %.*s=\"%.*s\"", __FUNCTION__, (int)namelen, name,
             (int)vallen, value));

    if ((namelen == 5) && (strncasecmp(name, "class", 5) == 0)) {
        l4sc_set_layout_class_by_name(obj, value, vallen);
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
format_header(l4sc_layout_ptr_t layout, int kind, char *buf, size_t bufsize)
{
    return (0);
}

static size_t
estimate_json_size(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg)
{
    return (msg->msglen + (msg->msglen >> 2) + strlen(msg->logger->name) +
            strlen(msg->file) + strlen(msg->func) + 500);
}

/*
 * {
 *   "timeMillis" : 1459374920444,
 *   "thread" : "Thread-2",
 *   "level" : "DEBUG",
 *   "loggerName" : "MapiSession",
 *   "message" : "writing <transaction transid=\"1\"
 * userid=\"1\">\r\n\t<method>getAddressBookEntries</method>\r\n</transaction>\r\n",
 *   "endOfBatch" : false,
 *   "loggerFqcn" : "org.apache.logging.log4j.spi.AbstractLogger",
 *   "source" : {
 *     "class" : "MapiSession$Communicator",
 *     "method" : "run",
 *     "file" : "MapiSession.java",
 *     "line" : 99
 *   }
 * }
 */

static size_t
format_json_message(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
                    char *buf, size_t bufsize)
{
    char *dp = buf;
    const char *limit = buf + bufsize;
    int i, rc;
    const char *levelstr =
        IS_AT_LEAST_FATAL_LEVEL(msg->level)
            ? "FATAL"
            : IS_AT_LEAST_ERROR_LEVEL(msg->level)
                  ? "ERROR"
                  : IS_AT_LEAST_WARN_LEVEL(msg->level)
                        ? "WARN"
                        : IS_AT_LEAST_INFO_LEVEL(msg->level)
                              ? "INFO"
                              : IS_AT_LEAST_DEBUG_LEVEL(msg->level) ? "DEBUG"
                                                                    : "TRACE";

    /* This should work up to the year 2105 (for 32-bit unsigned long) */
    unsigned long timesecs = msg->time.tv_day * 86400uL + msg->time.tv_sec;
    unsigned long timefrac = msg->time.tv_usec / 1000;
    if (timefrac >= 1000) {
        timesecs += timefrac / 1000;
        timefrac = timefrac % 1000;
    }
    rc = l4sc_snprintf(dp, limit - dp,
                       "{ \"timeMillis\" : %lu%03u,"
                       " \"instant\" : "
                       "{ \"epochSecond\" : %lu,"
                       " \"nanoOfSecond\" : %lu },"
                       " \"thread\" : \"%s\","
                       " \"level\" : \"%s\","
                       " \"loggerName\" : \"%s\",\n",
                       timesecs, (unsigned)timefrac, timesecs,
                       1000uL * msg->time.tv_usec, msg->threadid, levelstr,
                       msg->logger->name);
    if ((rc > 0) && (dp + rc < limit)) {
        dp += rc;
    }
    rc = l4sc_snprintf(dp, limit - dp, "  \"message\" : \"");
    if ((rc > 0) && (dp + rc < limit)) {
        dp += rc;
    }
    for (i = 0; (i < msg->msglen) && (dp + 8 < limit); i++) {
        unsigned char c = (unsigned char)msg->msg[i];
        if ((c < ' ') || (c == '\"') || (c == '\\')) {
            *(dp++) = '\\';
            if ((c == '"') || (c == '\\')) {
                *(dp++) = c;
            } else if (c == '\n') {
                *(dp++) = 'n';
            } else if (c == '\r') {
                *(dp++) = 'r';
            } else if (c == '\t') {
                *(dp++) = 't';
            } else if (c == 8) {
                *(dp++) = 'b';
            } else if (c == 12) {
                *(dp++) = 'f';
            } else {
                dp += l4sc_snprintf(dp, limit - dp, "u%04lx", c);
            }
        } else {
            *(dp++) = c;
        }
    }
    rc = l4sc_snprintf(dp, limit - dp, "\",\n");
    if ((rc > 0) && (dp + rc < limit)) {
        dp += rc;
    }
    rc = l4sc_snprintf(dp, limit - dp,
                       "  \"endOfBatch\" : false, \"loggerFqcn\" : \"%s\",\n",
                       "org.apache.logging.log4j.spi.AbstractLogger");
    if ((rc > 0) && (dp + rc < limit)) {
        dp += rc;
    }
    rc = l4sc_snprintf(
        dp, limit - dp,
        "  \"source\" : { \"class\" : \"%s\", \"method\" : \"%s\","
        " \"file\" : \"%s\", \"line\" : %u }\n",
        "", msg->func, msg->file, msg->line);
    if ((rc > 0) && (dp + rc < limit)) {
        dp += rc;
    }
    rc = l4sc_snprintf(dp, limit - dp, "}\n");
    if ((rc > 0) && (dp + rc < limit)) {
        dp += rc;
    }
    return (dp - buf);
}
