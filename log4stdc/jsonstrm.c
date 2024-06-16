
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "compat.h"
#include "logobjs.h"

#if defined(__STDC__) || defined(HAVE_STDINT_H)
#include <stdint.h>
#endif

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
init_json_ws_stream_layout(void *, size_t, bfc_mempool_t);

static size_t
format_json_ws_message(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
                       char *buf, size_t bufsize);
static size_t
estimate_json_ws_size(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg);

const struct l4sc_layout_class l4sc_json_ws_stream_layout_class = {
    /* .super        */ (l4sc_layout_class_ptr_t)&l4sc_object_class,
    /* .name         */ "JsonWsLayout",
    /* .spare2       */ NULL,
    /* .spare3       */ NULL,
    /* .init         */ init_json_ws_stream_layout,
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
    /* .format       */ format_json_ws_message,
    /* .header       */ format_header,
    /* .estimate     */ estimate_json_ws_size};

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
    const char *levelstr = IS_AT_LEAST_FATAL_LEVEL(msg->level)   ? "FATAL"
                           : IS_AT_LEAST_ERROR_LEVEL(msg->level) ? "ERROR"
                           : IS_AT_LEAST_WARN_LEVEL(msg->level)  ? "WARN"
                           : IS_AT_LEAST_INFO_LEVEL(msg->level)  ? "INFO"
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

#define WEBSOCKET_F0_FIN 0x80
#define WEBSOCKET_F0_OPCODE 0x0F
#define WEBSOCKET_F0_CONT 0x00
#define WEBSOCKET_F0_TEXT 0x01
#define WEBSOCKET_F0_BINARY 0x02
#define WEBSOCKET_F0_CLOSE 0x08
#define WEBSOCKET_F0_PING 0x09
#define WEBSOCKET_F0_PONG 0x0A

#define WEBSOCKET_F1_MASK 0x80
#define WEBSOCKET_F1_LENGTH 0x7F
#define WEBSOCKET_F1_LEN2B 126
#define WEBSOCKET_F1_LEN8B 127

#define WEBSOCKET_NO_MASK 256 /*or'ed into opcode in websocket_send*/

static int
init_json_ws_stream_layout(void *buf, size_t bufsize, bfc_mempool_t pool)
{
    BFC_INIT_PROLOGUE(l4sc_layout_class_ptr_t, l4sc_layout_ptr_t, layout, buf,
                      bufsize, pool, &l4sc_json_ws_stream_layout_class);
    layout->name = "json websocket stream layout";
    LOGDEBUG(("init_json_ws_stream_layout(%p, bufsize %ld, pool %p): %s %s",
              buf, (long)bufsize, pool, BFC_CLASS(layout)->name,
              layout->name));
    return (BFC_SUCCESS);
}

static size_t
estimate_json_ws_size(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg)
{
    return (16 + estimate_json_size(layout, msg));
}

static void
generate_mask(unsigned char *mask, const void *buf, size_t len);
static void
mask_in_place(unsigned char *payload, unsigned char *mask, size_t len);
static void
copy_and_mask(unsigned char *dest, unsigned char *payload, unsigned char *mask,
              size_t len);
static void
copy_reverse_and_mask(unsigned char *dest, unsigned char *payload,
                      unsigned char *mask, size_t len);

static size_t
format_json_ws_message(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
                       char *buf, size_t bufsize)
{
    const unsigned char opcode = WEBSOCKET_F0_TEXT;
    const unsigned char f1_mask = WEBSOCKET_F1_MASK;
    unsigned char *frame = (unsigned char *)buf;
    /* initialize with estimated values, corrected later ... */
    unsigned char *maskp = frame + ((msg->msglen < 60000) ? 4 : 10);
    unsigned char *payload = maskp + (f1_mask ? 4 : 0);
    size_t len;

    if (bufsize < 40) {
        return 0;
    }

    len = format_json_message(layout, msg, (char *)payload,
                              buf + bufsize - (char *)payload);

    if (len < WEBSOCKET_F1_LEN2B) {
        maskp = frame + 2;
    } else if ((len >> 8) < 256) {
        maskp = frame + 4;
    } else {
        maskp = frame + 10;
    }

    if (f1_mask) {
        unsigned char *dest = maskp + 4;
        if (payload == dest) {
            generate_mask(maskp, buf, len);
            mask_in_place(payload, maskp, len);
        } else if (payload > dest) {
            generate_mask(maskp, buf, len);
            copy_and_mask(dest, payload, maskp, len);
        } else {
            unsigned char maskbuf[4];
            generate_mask(maskbuf, buf, len);
            copy_reverse_and_mask(dest, payload, maskbuf, len);
            memcpy(maskp, maskbuf, 4);
        }
        payload = dest;

    } else if (payload != maskp) {
        memmove(maskp, payload, len);
        payload = maskp;
    }

    frame[0] = WEBSOCKET_F0_FIN | opcode;
    if (maskp == frame + 2) {
        frame[1] = f1_mask | len;
    } else if (maskp == frame + 4) {
        frame[1] = f1_mask | WEBSOCKET_F1_LEN2B;
        frame[2] = (unsigned char)(len >> 8);
        frame[3] = (unsigned char)len;
    } else if (maskp == frame + 10) {
        unsigned i;
        size_t shiftval = len;
        frame[1] = f1_mask | WEBSOCKET_F1_LEN8B;
        memset(frame + 2, 0, 8);
        for (i = 9; (shiftval != 0) && (i > 1); i--, shiftval >>= 8) {
            frame[i] = (unsigned char)shiftval;
        }
    }

    return (payload + len - frame);
}

static void
generate_mask(unsigned char *mask, const void *buf, size_t len)
{
    size_t rndval = (size_t)buf + (size_t)&rndval + len;
    int i;

    for (i = 0; i < 4; i++, rndval >>= 3) {
        mask[i] = (unsigned char)rndval;
    }
}

static void
mask_in_place(unsigned char *payload, unsigned char *mask, size_t len)
{
    size_t i = 0;

    if ((((intptr_t)payload | (intptr_t)mask) & 0x03) == 0) {
#if defined(__STDC__) || defined(HAVE_STDINT_H) || defined(UINT32_MAX)
        uint32_t mask32 = *mask;
        uint32_t *dst32 = (uint32_t *)payload;
        for (; i < len; i += 4, dst32++) {
            *dst32 ^= mask32;
        }
#else
        if (sizeof(unsigned) == 4) {
            unsigned mask32 = *mask;
            unsigned *dst32 = (unsigned *)payload;
            for (; i < len; i += 4, dst32++) {
                *dst32 ^= mask32;
            }
        }
#endif
    }

    for (; i < len; i++) {
        payload[i] ^= mask[i & 3];
    }
}

static void
copy_and_mask(unsigned char *dest, unsigned char *payload, unsigned char *mask,
              size_t len)
{
    size_t i;

    for (i = 0; i < len; i++) {
        dest[i] = payload[i] ^ mask[i & 3];
    }
}

static void
copy_reverse_and_mask(unsigned char *dest, unsigned char *payload,
                      unsigned char *mask, size_t len)
{
    size_t i;

    if (len > 0) {
        for (i = len - 1;; i--) {
            dest[i] = payload[i] ^ mask[i & 3];
            if (i == 0) {
                break;
            }
        }
    }
}
