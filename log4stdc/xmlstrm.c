
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "compat.h"
#include "logobjs.h"
#include "bareftc/utf8.h"

static int
init_xml_stream_layout(void *, size_t, bfc_mempool_t);
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
format_xml_message(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
                   char *buf, size_t bufsize);

static size_t
format_header(l4sc_layout_ptr_t layout, int kind, char *buf, size_t bufsize);

static size_t
estimate_xml_size(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg);

const struct l4sc_layout_class l4sc_xml_stream_layout_class = {
    /* .super        */ (l4sc_layout_class_ptr_t)&l4sc_object_class,
    /* .name         */ "XmlLayout",
    /* .spare2       */ NULL,
    /* .spare3       */ NULL,
    /* .init         */ init_xml_stream_layout,
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
    /* .format       */ format_xml_message,
    /* .header       */ format_header,
    /* .estimate     */ estimate_xml_size};

static const unsigned short cp1252_80_9F[] = {
    0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
    0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0x008F,
    0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
    0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009D, 0x017E, 0x0178};

static int
init_xml_stream_layout(void *buf, size_t bufsize, bfc_mempool_t pool)
{
    BFC_INIT_PROLOGUE(l4sc_layout_class_ptr_t, l4sc_layout_ptr_t, layout, buf,
                      bufsize, pool, &l4sc_xml_stream_layout_class);
    layout->name = "xml stream layout";
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
estimate_xml_size(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg)
{
    return (msg->msglen + (msg->msglen >> 2) + strlen(msg->logger->name) +
            strlen(msg->file) + strlen(msg->func) + 400);
}

/*
 * <?xml version="1.0" encoding="UTF-8"?>      -- CONDITIONAL: complete="true"
 * <Events xmlns="http://logging.apache.org/log4j/2.0/events">
 *
 * <Event xmlns="http://logging.apache.org/log4j/2.0/events"
 *      timeMillis="1460302248157" thread="main" level="INFO"
 *      loggerName="Logtest" endOfBatch="false"
 *      loggerFqcn="org.apache.logging.log4j.spi.AbstractLogger">
 *   <Message>one log line</Message>
 *   <Source class="Logtest" method="main" file="Logtest.java" line="11"/>
 * </Event>
 *
 * </Events>                                   -- CONDITIONAL: complete="true"
 */

static size_t
format_xml_message(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
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
    rc = l4sc_snprintf(
        dp, limit - dp,
        "<Event xmlns=\"http://logging.apache.org/log4j/2.0/events\""
        " timeMillis=\"%lu%03u\" thread=\"%s\" level=\"%s\""
        " loggerName=\"%s\" endOfBatch=\"false\" loggerFqcn=\"%s\">\n",
        timesecs, (unsigned)timefrac, msg->threadid, levelstr,
        msg->logger->name, "org.apache.logging.log4j.spi.AbstractLogger");
    if ((rc > 0) && (dp + rc < limit)) {
        dp += rc;
    }
    rc = l4sc_snprintf(dp, limit - dp, "  <Message>");
    if ((rc > 0) && (dp + rc < limit)) {
        dp += rc;
    }
    for (i = 0; (i < msg->msglen) && (dp + 8 < limit); i++) {
        unsigned char c = (unsigned char)msg->msg[i];
        if (c & 0x80) {
            unsigned long unicode;
            unsigned char c1 = (unsigned char)msg->msg[i + 1];
            if ((c >= 0xC0) && (c1 >= 0x80) && (c1 < 0xC0)) {
                const char *uniptr = msg->msg + i;
                const char *unilim = msg->msg + msg->msglen;
                BFC_GET_UTF8(unicode, uniptr, unilim);
                i += uniptr - &msg->msg[i] - 1;
            } else if (c < 0xA0) {
                unicode = cp1252_80_9F[c & 0x1F];
            } else {
                unicode = c; /* assume latin-1 */
            }
            BFC_PUT_UTF8(dp, limit, unicode);
        } else if (c > '>') {
            *(dp++) = (char)c;
        } else if (c == '&') {
            memcpy(dp, "&amp;", 5);
            dp += 5;
        } else if (c == '<') {
            memcpy(dp, "&lt;", 4);
            dp += 4;
        } else if (c == '>') {
            memcpy(dp, "&gt;", 4);
            dp += 4;
        } else {
            *(dp++) = (char)c;
        }
    }
    rc = l4sc_snprintf(dp, limit - dp, "</Message>\n");
    if ((rc > 0) && (dp + rc < limit)) {
        dp += rc;
    }
    rc = l4sc_snprintf(
        dp, limit - dp,
        "  <Source class=\"%s\" method=\"%s\" file=\"%s\" line=\"%u\"/>\n", "",
        msg->func, msg->file, msg->line);
    if ((rc > 0) && (dp + rc < limit)) {
        dp += rc;
    }
    rc = l4sc_snprintf(dp, limit - dp, "</Event>\r\n");
    if ((rc > 0) && (dp + rc < limit)) {
        dp += rc;
    }
    return (dp - buf);
}
