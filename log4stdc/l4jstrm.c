
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
init_log4j_stream_layout(void *, size_t, bfc_mempool_t);
static int
init_log4j2_stream_layout(void *, size_t, bfc_mempool_t);
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
format_log4j_message(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
                     char *buf, size_t bufsize);

static size_t
format_log4j2_message(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
                      char *buf, size_t bufsize);

static size_t
format_log4j_header(l4sc_layout_ptr_t layout, int kind, char *buf,
                    size_t bufsize);

static size_t
estimate_log4j_size(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg);

static size_t
estimate_log4j2_size(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg);

const struct l4sc_layout_class l4sc_log4j_stream_layout_class = {
    /* .super        */ (l4sc_layout_class_ptr_t)&l4sc_object_class,
    /* .name         */ "log4j_stream_layout",
    /* .spare2       */ NULL,
    /* .spare3       */ NULL,
    /* .init         */ init_log4j_stream_layout,
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
    /* .format       */ format_log4j_message,
    /* .header       */ format_log4j_header,
    /* .estimate     */ estimate_log4j_size};

const struct l4sc_layout_class l4sc_log4j2_stream_layout_class = {
    /* .super        */ (l4sc_layout_class_ptr_t)&l4sc_object_class,
    /* .name         */ "SerializedLayout",
    /* .spare2       */ NULL,
    /* .spare3       */ NULL,
    /* .init         */ init_log4j2_stream_layout,
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
    /* .format       */ format_log4j2_message,
    /* .header       */ format_log4j_header,
    /* .estimate     */ estimate_log4j2_size};

static int
init_log4j_stream_layout(void *buf, size_t bufsize, bfc_mempool_t pool)
{
    BFC_INIT_PROLOGUE(l4sc_layout_class_ptr_t, l4sc_layout_ptr_t, layout, buf,
                      bufsize, pool, &l4sc_log4j_stream_layout_class);
    layout->name = "log4j stream layout";
    layout->u.jstrm.object_handle = 0x7E0000uL;
    return (BFC_SUCCESS);
}

static int
init_log4j2_stream_layout(void *buf, size_t bufsize, bfc_mempool_t pool)
{
    BFC_INIT_PROLOGUE(l4sc_layout_class_ptr_t, l4sc_layout_ptr_t, layout, buf,
                      bufsize, pool, &l4sc_log4j2_stream_layout_class);
    layout->name = "log4j2 stream layout";
    layout->u.jstrm.object_handle = 0x7E0000uL;
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

/*
 * 6.4.2 Terminal Symbols and Constants
 *
 * The following symbols in java.io.ObjectStreamConstants define the terminal
 * and constant values expected in a stream.
 */
#define STREAM_MAGIC (short)0xaced
#define STREAM_VERSION (short)5
#define TC_NULL 0x70
#define TC_REFERENCE 0x71
#define TC_CLASSDESC 0x72
#define TC_OBJECT 0x73
#define TC_STRING 0x74
#define TC_ARRAY 0x75
#define TC_CLASS 0x76
#define TC_BLOCKDATA 0x77
#define TC_ENDBLOCKDATA 0x78
#define TC_RESET 0x79
#define TC_BLOCKDATALONG 0x7A
#define TC_EXCEPTION 0x7B
#define TC_LONGSTRING 0x7C
#define TC_PROXYCLASSDESC 0x7D
#define TC_ENUM 0x7E
#define baseWireHandle 0x7E0000uL
/*
 * The flag byte classDescFlags may include values of
 */
#define SC_WRITE_METHOD 0x01 /* if SC_SERIALIZABLE */
#define SC_BLOCK_DATA 0x08   /* if SC_EXTERNALIZABLE */
#define SC_SERIALIZABLE 0x02
#define SC_EXTERNALIZABLE 0x04
#define SC_ENUM 0x10

#define PUTNEXTBYTE(ptr,c,limit)                                            \
    if ((ptr) < (limit)) {                                                    \
        *((ptr)++) = (c);                                                     \
    } else {                                                                  \
        (ptr)++;                                                              \
    }

#define PUTNEXTINT(ptr,v,limit)                                             \
    if ((ptr) + 4 < (limit)) {                                                \
        *((ptr)++) = (char)((v) >> 24);                                       \
        *((ptr)++) = (char)((v) >> 16);                                       \
        *((ptr)++) = (char)((v) >> 8);                                        \
        *((ptr)++) = (char)((v) & 255);                                       \
    } else {                                                                  \
        (ptr) += 4;                                                           \
    }

#define PUTNEXTSHORT(ptr,v,limit)                                           \
    if ((ptr) + 2 < (limit)) {                                                \
        *((ptr)++) = (char)((v) >> 8);                                        \
        *((ptr)++) = (char)((v) & 255);                                       \
    } else {                                                                  \
        (ptr) += 2;                                                           \
    }

#define PUTNEXTLONG(ptr,v,limit)                                            \
    if ((ptr) + 8 < (limit)) {                                                \
        *((ptr)++) = (char)((v) >> 56);                                       \
        *((ptr)++) = (char)((v) >> 48);                                       \
        *((ptr)++) = (char)((v) >> 40);                                       \
        *((ptr)++) = (char)((v) >> 32);                                       \
        *((ptr)++) = (char)((v) >> 24);                                       \
        *((ptr)++) = (char)((v) >> 16);                                       \
        *((ptr)++) = (char)((v) >> 8);                                        \
        *((ptr)++) = (char)((v) & 255);                                       \
    } else {                                                                  \
        (ptr) += 8;                                                           \
    }

#define PUTNEXTSTRING(ptr,s,len,limit)                                     \
    if ((ptr) + (len) < (limit)) {                                            \
        memcpy(ptr, s, len);                                                  \
        (ptr) += (len);                                                       \
    } else if ((ptr) < (limit)) {                                             \
        memcpy(ptr, s, (limit) - (ptr));                                      \
        (ptr) += (len);                                                       \
    } else {                                                                  \
        (ptr) += (len);                                                       \
    }

/* This should work up to the year 2105 (for 32-bit unsigned long) */
#define PUTMSGTIMESTAMP(ptr,msg,limit)                                      \
    if ((ptr) + 8 < limit) { /* 84375 = 24*60*60*1000 >> 10 */                \
        unsigned long hi = 84375uL * msg->time.tv_day;                        \
        unsigned long lo =                                                    \
            1000 * msg->time.tv_sec + msg->time.tv_usec / 1000;               \
        (ptr)[7] = (char)lo;                                                  \
        lo >>= 8;                                                             \
        (ptr)[6] = (char)(lo + (hi << 2));                                    \
        lo >>= 2;                                                             \
        hi += lo;                                                             \
        hi >>= 6;                                                             \
        (ptr)[5] = (char)hi;                                                  \
        hi >>= 8;                                                             \
        (ptr)[4] = (char)hi;                                                  \
        hi >>= 8;                                                             \
        (ptr)[3] = (char)hi;                                                  \
        hi >>= 8;                                                             \
        (ptr)[2] = (char)hi;                                                  \
        hi >>= 8;                                                             \
        (ptr)[1] = (char)hi;                                                  \
        hi >>= 8;                                                             \
        (ptr)[0] = (char)hi;                                                  \
        hi >>= 8;                                                             \
        (ptr) += 8;                                                           \
    } else {                                                                  \
        (ptr) += 8;                                                           \
    }

static void
reset_l4j_stream(l4sc_layout_ptr_t layout)
{
    memset(&layout->u, 0, sizeof(layout->u));
    layout->u.jstrm.object_handle = baseWireHandle;
}

static size_t
format_log4j_header(l4sc_layout_ptr_t layout, int kind, char *buf,
                    size_t bufsize)
{
    char *dp = buf;
    const char *limit = dp + bufsize;
    if (kind == L4SC_FIRST_HEADER) {
        reset_l4j_stream(layout);
        PUTNEXTSHORT(dp, STREAM_MAGIC, limit);
        PUTNEXTSHORT(dp, STREAM_VERSION, limit);
    }
    return (dp - buf);
}

static size_t
raw_message_len(l4sc_logmessage_cptr_t msg)
{
    return (msg->msglen + strlen(msg->logger->name) + strlen(msg->file) +
            strlen(msg->func));
}

static size_t
estimate_log4j_size(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg)
{
    size_t estimate = raw_message_len(msg) + 200;

    if ((layout->u.jstrm.loggingevent_reference[0] != TC_OBJECT) ||
        (layout->u.jstrm.object_handle > baseWireHandle + 500)) {
        estimate += 400;
    }
    return (estimate);
}

static int
write_prolog(l4sc_layout_ptr_t layout, char *buf, const char *limit,
             const char *classname, char *refbytes, int classobjincrement,
             const char *classdesc, size_t desclen)
{
    char *dp = buf;

    if (refbytes[0] == TC_OBJECT) {
        PUTNEXTSTRING(dp, refbytes, 6, limit);
        layout->u.jstrm.object_handle++;
    } else {
        unsigned long handle = layout->u.jstrm.object_handle;
        PUTNEXTBYTE(dp, TC_OBJECT, limit);
        PUTNEXTSTRING(dp, classdesc, desclen, limit);
        refbytes[0] = TC_OBJECT;
        refbytes[1] = TC_REFERENCE;
        refbytes[2] = (char)((handle >> 24) & 0xFF);
        refbytes[3] = (char)((handle >> 16) & 0xFF);
        refbytes[4] = (char)((handle >> 8) & 0xFF);
        refbytes[5] = (char)(handle & 0xFF);
        layout->u.jstrm.object_handle += (classobjincrement + 1);
    }
    return (dp - buf);
}

static int
write_loggingevent_prolog(l4sc_layout_ptr_t layout, char *buf,
                          const char *limit)
{
    /* clang-format off */
    static const char classDesc[] = {
        0x72, 0x00, 0x21,
        0x6F, 0x72, 0x67, 0x2E, 0x61, 0x70, 0x61, 0x63,
        0x68, 0x65, 0x2E, 0x6C, 0x6F, 0x67, 0x34, 0x6A,
        0x2E, 0x73, 0x70, 0x69, 0x2E, 0x4C, 0x6F, 0x67,
        0x67, 0x69, 0x6E, 0x67, 0x45, 0x76, 0x65, 0x6E,
        0x74, 0xF3, 0xF2, 0xB9, 0x23, 0x74, 0x0B, 0xB5,
        0x3F, 0x03, 0x00, 0x0A, 0x5A, 0x00, 0x15, 0x6D,
        0x64, 0x63, 0x43, 0x6F, 0x70, 0x79, 0x4C, 0x6F,
        0x6F, 0x6B, 0x75, 0x70, 0x52, 0x65, 0x71, 0x75,
        0x69, 0x72, 0x65, 0x64, 0x5A, 0x00, 0x11, 0x6E,
        0x64, 0x63, 0x4C, 0x6F, 0x6F, 0x6B, 0x75, 0x70,
        0x52, 0x65, 0x71, 0x75, 0x69, 0x72, 0x65, 0x64,
        0x4A, 0x00, 0x09, 0x74, 0x69, 0x6D, 0x65, 0x53,
        0x74, 0x61, 0x6D, 0x70, 0x4C, 0x00, 0x0C, 0x63,
        0x61, 0x74, 0x65, 0x67, 0x6F, 0x72, 0x79, 0x4E,
        0x61, 0x6D, 0x65, 0x74, 0x00, 0x12, 0x4C, 0x6A,
        0x61, 0x76, 0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67,
        0x2F, 0x53, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3B,
        0x4C, 0x00, 0x0C, 0x6C, 0x6F, 0x63, 0x61, 0x74,
        0x69, 0x6F, 0x6E, 0x49, 0x6E, 0x66, 0x6F, 0x74,
        0x00, 0x23, 0x4C, 0x6F, 0x72, 0x67, 0x2F, 0x61,
        0x70, 0x61, 0x63, 0x68, 0x65, 0x2F, 0x6C, 0x6F,
        0x67, 0x34, 0x6A, 0x2F, 0x73, 0x70, 0x69, 0x2F,
        0x4C, 0x6F, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E,
        0x49, 0x6E, 0x66, 0x6F, 0x3B, 0x4C, 0x00, 0x07,
        0x6D, 0x64, 0x63, 0x43, 0x6F, 0x70, 0x79, 0x74,
        0x00, 0x15, 0x4C, 0x6A, 0x61, 0x76, 0x61, 0x2F,
        0x75, 0x74, 0x69, 0x6C, 0x2F, 0x48, 0x61, 0x73,
        0x68, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x3B, 0x4C,
        0x00, 0x03, 0x6E, 0x64, 0x63,
        0x74, 0x00, 0x12, 0x4C, 0x6A,
        0x61, 0x76, 0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67,
        0x2F, 0x53, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3B,
        0x4C, 0x00, 0x0F, 0x72, 0x65, 0x6E,
        0x64, 0x65, 0x72, 0x65, 0x64, 0x4D, 0x65, 0x73,
        0x73, 0x61, 0x67, 0x65,
        0x74, 0x00, 0x12, 0x4C, 0x6A,
        0x61, 0x76, 0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67,
        0x2F, 0x53, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3B,
        0x4C, 0x00, 0x0A, 0x74, 0x68, 0x72, 0x65,
        0x61, 0x64, 0x4E, 0x61, 0x6D, 0x65,
        0x74, 0x00, 0x12, 0x4C, 0x6A,
        0x61, 0x76, 0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67,
        0x2F, 0x53, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3B,
        0x4C, 0x00, 0x0D, 0x74, 0x68,
        0x72, 0x6F, 0x77, 0x61, 0x62, 0x6C, 0x65, 0x49,
        0x6E, 0x66, 0x6F, 0x74, 0x00, 0x2B, 0x4C, 0x6F,
        0x72, 0x67, 0x2F, 0x61, 0x70, 0x61, 0x63, 0x68,
        0x65, 0x2F, 0x6C, 0x6F, 0x67, 0x34, 0x6A, 0x2F,
        0x73, 0x70, 0x69, 0x2F, 0x54, 0x68, 0x72, 0x6F,
        0x77, 0x61, 0x62, 0x6C, 0x65, 0x49, 0x6E, 0x66,
        0x6F, 0x72, 0x6D, 0x61, 0x74, 0x69, 0x6F, 0x6E,
        0x3B, 0x78, 0x70 };
    /* clang-format on */

    return (write_prolog(layout, buf, limit,
                         "org.apache.log4j.spi.LoggingEvent",
                         layout->u.jstrm.loggingevent_reference, 8, classDesc,
                         sizeof(classDesc)));
}

static int
write_string_object(l4sc_layout_ptr_t layout, char *buf, const char *limit,
                    const char *s, size_t len)
{
    size_t i;
    char *lenp, *dp = buf;
    static const unsigned short cp1252_80_9F[] = {
        0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
        0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0x008F,
        0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
        0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009D, 0x017E, 0x0178};

    layout->u.jstrm.object_handle++;

    PUTNEXTBYTE(dp, TC_STRING, limit);

    lenp = dp;
    PUTNEXTSHORT(dp, len, limit);

    for (i = 0; i < len; i++) {
        unsigned char c = (unsigned char)s[i];
        if (c & 0x80) {
            unsigned long unicode;
            unsigned char c1 = (unsigned char)s[i + 1];
            if ((c >= 0xC0) && (c1 >= 0x80) && (c1 < 0xC0)) {
                const char *uniptr = s + i;
                const char *unilim = s + len;
                BFC_GET_UTF8(unicode, uniptr, unilim);
                i += uniptr - &s[i] - 1;
            } else if (c < 0xA0) {
                unicode = cp1252_80_9F[c & 0x1F];
            } else {
                unicode = c; /* assume latin-1 */
            }
            if (unicode < 0x10000uL) {
                BFC_PUT_UTF8(dp, limit, unicode);
            } else { /* special java encoding */
                unsigned short wbuf[4];
                unsigned short *wp = wbuf;
                BFC_PUT_UTF16(wp, wbuf + 4, unicode);
                BFC_PUT_UTF8(dp, limit, wbuf[0]);
                BFC_PUT_UTF8(dp, limit, wbuf[1]);
            }
        } else {
            PUTNEXTBYTE(dp, c, limit);
        }
    }
    if (dp > lenp + 2 + len) {
        size_t utflen = dp - lenp - 2;
        PUTNEXTSHORT(lenp, utflen, limit);
    }
    return (dp - buf);
}

static int
write_locationinfo(l4sc_layout_ptr_t layout, char *buf, const char *limit,
                   const char *file, int line, const char *function)
{
    int rc;
    char *hdr, *sep, *dp = buf;
    /* clang-format off */
    static const char prolog[] = {
         0x72, 0x00, 0x21, 0x6F, 0x72, 0x67, 0x2E,
         0x61, 0x70, 0x61, 0x63, 0x68, 0x65, 0x2E, 0x6C,
         0x6F, 0x67, 0x34, 0x6A, 0x2E, 0x73, 0x70, 0x69,
         0x2E, 0x4C, 0x6F, 0x63, 0x61, 0x74, 0x69, 0x6F,
         0x6E, 0x49, 0x6E, 0x66, 0x6F, 0xED, 0x99, 0xBB,
         0xE1, 0x4A, 0x91, 0xA5, 0x7C, 0x02, 0x00, 0x01,
         0x4C, 0x00, 0x08, 0x66, 0x75, 0x6C, 0x6C, 0x49,
         0x6E, 0x66, 0x6F,
            0x74, 0x00, 0x12, 0x4C, 0x6A,
                0x61, 0x76, 0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67,
                0x2F, 0x53, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x3B,
         0x78, 0x70 };
    /* clang-format on */

    if ((line == -1) && (file == NULL) && (function == NULL)) {
        PUTNEXTBYTE(dp, TC_NULL, limit);
    } else {
        if ((rc = write_prolog(layout, dp, limit,
                               "org.apache.log4j.spi.LocationInfo",
                               layout->u.jstrm.locationinfo_reference, 2,
                               prolog, sizeof(prolog))) > 0) {
            dp += rc;
        }
        hdr = dp;
        PUTNEXTBYTE(dp, TC_STRING, limit);
        PUTNEXTSHORT(dp, 0, limit);
        if ((sep = strstr(function, "::")) != NULL) {
            int clslen = sep - function;
            rc = l4sc_snprintf(dp, limit - dp, "%.*s.%s(%s:%u)", clslen,
                               function, sep + 2, file, line);
        } else {
            rc = l4sc_snprintf(dp, limit - dp, ".%s(%s:%u)", function, file,
                               line);
        }
        if (rc > 0) {
            layout->u.jstrm.object_handle++;
            PUTNEXTBYTE(hdr, TC_STRING, limit);
            PUTNEXTSHORT(hdr, rc, limit);
            dp += rc;
        } else {
            dp = hdr;
            PUTNEXTBYTE(dp, TC_NULL, limit);
        }
    }
    return (dp - buf);
}

static size_t
format_log4j_message(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
                     char *buf, size_t bufsize)
{
    char *dp = buf;
    const char *limit = buf + bufsize;
    l4sc_layout_ptr_t state = layout;
    int rc;
    struct l4sc_layout statebuf;

    if (layout->u.jstrm.loggingevent_reference[0] != TC_OBJECT) {
        memcpy(&statebuf, layout, offsetof(struct l4sc_layout, u));
        reset_l4j_stream(&statebuf);
        state = &statebuf;
    } else if ((layout->u.jstrm.object_handle > baseWireHandle + 500) &&
               (bufsize > raw_message_len(msg) + 580)) {
        memcpy(&statebuf, layout, offsetof(struct l4sc_layout, u));
        reset_l4j_stream(&statebuf);
        state = &statebuf;
        PUTNEXTBYTE(dp, TC_RESET, limit);
    }
    if ((rc = write_loggingevent_prolog(state, dp, limit)) > 0) {
        dp += rc;
    }
    /* mdc and ndc lookup required should always be false */
    PUTNEXTBYTE(dp, 0, limit);
    PUTNEXTBYTE(dp, 0, limit);
    PUTMSGTIMESTAMP(dp, msg, limit);
    if ((rc = write_string_object(state, dp, limit, msg->logger->name,
                                  strlen(msg->logger->name))) > 0) {
        dp += rc;
    }
    if ((rc = write_locationinfo(state, dp, limit, msg->file, msg->line,
                                 msg->func)) > 0) {
        dp += rc;
    }
    PUTNEXTBYTE(dp, TC_NULL, limit); /* mdc omitted */
    PUTNEXTBYTE(dp, TC_NULL, limit); /* ndc omitted */
    if ((rc = write_string_object(state, dp, limit, msg->msg, msg->msglen)) >
        0) {
        dp += rc;
    }
    if ((rc = write_string_object(state, dp, limit, msg->threadid,
                                  strlen(msg->threadid))) > 0) {
        dp += rc;
    }
    PUTNEXTBYTE(dp, TC_NULL, limit); /* throwable */
    PUTNEXTBYTE(dp, TC_BLOCKDATA, limit);
    PUTNEXTBYTE(dp, 0x04, limit);
    PUTNEXTINT(dp, msg->level, limit);
    PUTNEXTBYTE(dp, TC_NULL, limit);
    PUTNEXTBYTE(dp, TC_ENDBLOCKDATA, limit);

    if (state == &statebuf) {
        memcpy(&layout->u.jstrm, &state->u.jstrm, sizeof(layout->u.jstrm));
    }

    return (dp - buf);
}

/*
 * log4j2
 */

static size_t
estimate_log4j2_size(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg)
{
    size_t estimate = raw_message_len(msg) + 300;

    if ((layout->u.jstrm.simple_message_reference[0] != TC_OBJECT) ||
        (layout->u.jstrm.object_handle > baseWireHandle + 500)) {
        estimate += 1000;
    }
    return (estimate);
}

static int
write_logevent_prolog(l4sc_layout_ptr_t layout, char *buf, const char *limit)
{
    int len;
    /* clang-format off */
    static const char classDesc[] = {
    TC_CLASSDESC, 0x00, 0x3e, 'o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','c','o','r','e','.','i','m','p','l','.','L','o','g','4','j','L','o','g','E','v','e','n','t','$','L','o','g','E','v','e','n','t','P','r','o','x','y',
     0x9c, 0xed, 0x0f, 0x5d, 0x70, 0xda, 0x2a, 0x16, /* serialVersionUID */
     SC_SERIALIZABLE, 0x00, 0x0d, /* 13 fields will follow */
     'Z', 0x00, 0x0c, 'i','s','E','n','d','O','f','B','a','t','c','h',
     'Z', 0x00, 0x12, 'i','s','L','o','c','a','t','i','o','n','R','e','q','u','i','r','e','d',
     'J', 0x00, 0x0a, 't','i','m','e','M','i','l','l','i','s',
     'L', 0x00, 0x0a, 'c','o','n','t','e','x','t','M','a','p',
          TC_STRING, 0x00, 0x0f, 'L','j','a','v','a','/','u','t','i','l','/','M','a','p',';',
     'L', 0x00, 0x0c, 'c','o','n','t','e','x','t','S','t','a','c','k',
          TC_STRING, 0x00, 0x35, 'L','o','r','g','/','a','p','a','c','h','e','/','l','o','g','g','i','n','g','/','l','o','g','4','j','/','T','h','r','e','a','d','C','o','n','t','e','x','t','$','C','o','n','t','e','x','t','S','t','a','c','k',';',
     'L', 0x00, 0x05, 'l','e','v','e','l',
          TC_STRING, 0x00, 0x20, 'L','o','r','g','/','a','p','a','c','h','e','/','l','o','g','g','i','n','g','/','l','o','g','4','j','/','L','e','v','e','l',';',
     'L', 0x00, 0x0a, 'l','o','g','g','e','r','F','Q','C','N',
          TC_STRING, 0x00, 0x12, 'L','j','a','v','a','/','l','a','n','g','/','S','t','r','i','n','g',';',
     'L', 0x00, 0x0a, 'l','o','g','g','e','r','N','a','m','e',
          TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
     'L', 0x00, 0x06, 'm','a','r','k','e','r',
          TC_STRING, 0x00, 0x21, 'L','o','r','g','/','a','p','a','c','h','e','/','l','o','g','g','i','n','g','/','l','o','g','4','j','/','M','a','r','k','e','r',';',
     'L', 0x00, 0x07, 'm','e','s','s','a','g','e',
          TC_STRING, 0x00, 0x2a, 'L','o','r','g','/','a','p','a','c','h','e','/','l','o','g','g','i','n','g','/','l','o','g','4','j','/','m','e','s','s','a','g','e','/','M','e','s','s','a','g','e',';',
     'L', 0x00, 0x06, 's','o','u','r','c','e',
          TC_STRING, 0x00, 0x1d, 'L','j','a','v','a','/','l','a','n','g','/','S','t','a','c','k','T','r','a','c','e','E','l','e','m','e','n','t',';',
     'L', 0x00, 0x0a, 't','h','r','e','a','d','N','a','m','e',
          TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
     'L', 0x00, 0x0b, 't','h','r','o','w','n','P','r','o','x','y',
          TC_STRING, 0x00, 0x33, 'L','o','r','g','/','a','p','a','c','h','e','/','l','o','g','g','i','n','g','/','l','o','g','4','j','/','c','o','r','e','/','i','m','p','l','/','T','h','r','o','w','a','b','l','e','P','r','o','x','y',';',
                                     TC_ENDBLOCKDATA,
    TC_NULL /* superClassDesc */ };
    /* clang-format on */

    len = write_prolog(layout, buf, limit, "org.apache.log4j.spi.LoggingEvent",
                       layout->u.jstrm.loggingevent_reference, 9, classDesc,
                       sizeof(classDesc));

    if (len >= sizeof(classDesc)) {
        memcpy(layout->u.jstrm.javastring_reference,
               layout->u.jstrm.loggingevent_reference, 6);
        layout->u.jstrm.javastring_reference[5] =
            layout->u.jstrm.loggingevent_reference[5] + 4;
    }
    return (len);
}

static int
write_context_map(l4sc_layout_ptr_t layout, char *buf, const char *limit,
                  const void *map)
{
    int rc;
    char *dp = buf;
    /* clang-format off */
    static const char classDesc[] = {
    TC_CLASSDESC, 0x00, 0x1e, 'j','a','v','a','.','u','t','i','l','.','C','o','l','l','e','c','t','i','o','n','s','$','E','m','p','t','y','M','a','p',
     'Y', 0x36, 0x14, 0x85, 'Z', 0xdc, 0xe7, 0xd0,
     SC_SERIALIZABLE, 0x00, 0x00,
    TC_ENDBLOCKDATA,
    TC_NULL /* superClassDesc */ };
    /* clang-format on */

    if ((rc = write_prolog(layout, dp, limit, "java.util.Collections$EmptyMap",
                           layout->u.jstrm.context_map_reference, 1, classDesc,
                           sizeof(classDesc))) > 0) {
        dp += rc;
    }
    return (dp - buf);
}

static int
write_context_stack(l4sc_layout_ptr_t layout, char *buf, const char *limit,
                    const void *stack)
{
    int rc;
    char *dp = buf;
    /* clang-format off */
    static const char classDesc[] = {
    TC_CLASSDESC, 0x00, 0x3e, 'o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','T','h','r','e','a','d','C','o','n','t','e','x','t','$','E','m','p','t','y','T','h','r','e','a','d','C','o','n','t','e','x','t','S','t','a','c','k',
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    SC_SERIALIZABLE, 0x00, 0x00,
    TC_ENDBLOCKDATA,
    TC_NULL /* superClassDesc */ };
    /* clang-format on */

    if ((rc = write_prolog(
             layout, dp, limit,
             "org.apache.logging.log4j.ThreadContext$EmptyThreadContextStack",
             layout->u.jstrm.context_stack_reference, 1, classDesc,
             sizeof(classDesc))) > 0) {
        dp += rc;
    }
    return (dp - buf);
}

static int
write_level_object(l4sc_layout_ptr_t layout, char *buf, const char *limit,
                   unsigned level)
{
    int rc, intlevel, levellen;
    char *dp = buf;
    const char *levelname;

    /* clang-format off */
    static const char classDesc[] = {
    TC_CLASSDESC, 0x00, 0x1e, 'o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','L','e','v','e','l',
     0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x20, 0x1a,
     SC_SERIALIZABLE, 0x00, 0x03,
     'I', 0x00, 0x08, 'i', 'n', 't', 'L', 'e', 'v', 'e', 'l',
     'L', 0x00, 0x04, 'n', 'a', 'm', 'e',
          TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
     'L', 0x00, 0x0d, 's', 't', 'a', 'n', 'd', 'a', 'r', 'd', 'L', 'e', 'v', 'e', 'l',
          TC_STRING, 0x00, 0x2c, 'L','o','r','g','/','a','p','a','c','h','e','/','l','o','g','g','i','n','g','/','l','o','g','4','j','/','s','p','i','/','S','t','a','n','d','a','r','d','L','e','v','e','l',';',
    TC_ENDBLOCKDATA,
    TC_NULL /* superClassDesc */ };

    static const char enumDesc[] = {
    TC_CLASSDESC, 0x00, 0x2a, 'o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','s','p','i','.','S','t','a','n','d','a','r','d','L','e','v','e','l',
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00,
    TC_ENDBLOCKDATA,
    TC_CLASSDESC, 0x00, 0x0e, 'j','a','v','a','.','l','a','n','g','.','E','n','u','m',
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00,
    TC_ENDBLOCKDATA,
    TC_NULL /* superClassDesc */ };
    /* clang-format on */

    if ((rc = write_prolog(layout, dp, limit, "org.apache.logging.log4j.Level",
                           layout->u.jstrm.level_reference, 2, classDesc,
                           sizeof(classDesc))) > 0) {
        dp += rc;
        if (IS_AT_LEAST_FATAL_LEVEL(level)) {
            intlevel = 100;
            levelname = "FATAL";
            levellen = 5;
        } else if (IS_AT_LEAST_ERROR_LEVEL(level)) {
            intlevel = 200;
            levelname = "ERROR";
            levellen = 5;
        } else if (IS_AT_LEAST_WARN_LEVEL(level)) {
            intlevel = 300;
            levelname = "WARN";
            levellen = 4;
        } else if (IS_AT_LEAST_INFO_LEVEL(level)) {
            intlevel = 400;
            levelname = "INFO";
            levellen = 4;
        } else if (IS_AT_LEAST_DEBUG_LEVEL(level)) {
            intlevel = 500;
            levelname = "DEBUG";
            levellen = 5;
        } else {
            intlevel = 600;
            levelname = "TRACE";
            levellen = 5;
        }
        PUTNEXTINT(dp, intlevel, limit);
        if ((rc = write_string_object(layout, dp, limit, levelname,
                                      levellen)) > 0) {
            dp += rc;
        }
        if ((rc = write_prolog(layout, dp, limit,
                               "org.apache.logging.log4j.spi.StandardLevel",
                               layout->u.jstrm.standardlevel_reference, 2,
                               enumDesc, sizeof(enumDesc))) > 0) {
            dp[0] = TC_ENUM; /* instead of TC_OBJECT */
            dp += rc;
            if ((rc = write_string_object(layout, dp, limit, levelname,
                                          levellen)) > 0) {
                dp += rc;
            }
        }
    }
    return (dp - buf);
}

static int
write_message_object(l4sc_layout_ptr_t layout, char *buf, const char *limit,
                     const char *msg, size_t msglen)
{
    int rc;
    char *dp = buf;
    /* clang-format off */
    static const char classDesc[] = {
    TC_CLASSDESC, 0x00, '.','o','r','g','.','a','p','a','c','h','e','.','l','o','g','g','i','n','g','.','l','o','g','4','j','.','m','e','s','s','a','g','e','.','S','i','m','p','l','e','M','e','s','s','a','g','e',
     0x8b, 't','M','0', 0x60, 0xb7, 0xa2, 0xa8,
     SC_SERIALIZABLE, 0x00, 0x01,
     'L', 0x00, 0x07, 'm','e','s','s','a','g','e',
          TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
    TC_ENDBLOCKDATA,
    TC_NULL /* superClassDesc */ };
    /* clang-format on */

    if ((rc = write_prolog(layout, dp, limit,
                           "org.apache.logging.log4j.message.SimpleMessage",
                           layout->u.jstrm.simple_message_reference, 1,
                           classDesc, sizeof(classDesc))) > 0) {
        dp += rc;
        if ((rc = write_string_object(layout, dp, limit, msg, msglen)) > 0) {
            dp += rc;
        }
    }
    return (dp - buf);
}

static int
write_source_object(l4sc_layout_ptr_t layout, char *buf, const char *limit,
                    const char *file, int line, const char *function)
{
    int rc;
    char *dp = buf;
    /* clang-format off */
    static const char classDesc[] = {
    TC_CLASSDESC, 0x00, 0x1b, 'j','a','v','a','.','l','a','n','g','.','S','t','a','c','k','T','r','a','c','e','E','l','e','m','e','n','t',
     'a', 0x09, 0xc5, 0x9a, 0x26, '6', 0xdd, 0x85,
     SC_SERIALIZABLE, 0x00, 0x04,
     'I', 0x00, 0x0a, 'l','i','n','e','N','u','m','b','e','r',
     'L', 0x00, 0x0e, 'd','e','c','l','a','r','i','n','g','C','l','a','s','s',
          TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
     'L', 0x00, 0x08, 'f','i','l','e','N','a','m','e',
          TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
     'L', 0x00, 0x0a, 'm','e','t','h','o','d','N','a','m','e',
          TC_REFERENCE, 0x00, 0x7e, 0x00, 0x04,
    TC_ENDBLOCKDATA,
    TC_NULL /* superClassDesc */ };
    /* clang-format on */

    if ((rc = write_prolog(layout, dp, limit, "java.lang.StackTraceElement",
                           layout->u.jstrm.stack_trace_element_reference, 1,
                           classDesc, sizeof(classDesc))) > 0) {
        dp += rc;
        PUTNEXTINT(dp, line, limit);
        if ((rc = write_string_object(layout, dp, limit, "", 0)) > 0) {
            dp += rc;
        }
        if ((rc = write_string_object(layout, dp, limit, file, strlen(file))) >
            0) {
            dp += rc;
        }
        if ((rc = write_string_object(layout, dp, limit, function,
                                      strlen(function))) > 0) {
            dp += rc;
        }
    }
    return (dp - buf);
}

static size_t
format_log4j2_message(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
                      char *buf, size_t bufsize)
{
    char *dp = buf;
    const char *limit = buf + bufsize;
    l4sc_layout_ptr_t state = layout;
    int rc;
    struct l4sc_layout statebuf;

    if (layout->u.jstrm.simple_message_reference[0] != TC_OBJECT) {
        memcpy(&statebuf, layout, offsetof(struct l4sc_layout, u));
        reset_l4j_stream(&statebuf);
        state = &statebuf;
    } else if ((layout->u.jstrm.object_handle > baseWireHandle + 500) &&
               (bufsize > raw_message_len(msg) + 1250)) {
        memcpy(&statebuf, layout, offsetof(struct l4sc_layout, u));
        reset_l4j_stream(&statebuf);
        state = &statebuf;
        PUTNEXTBYTE(dp, TC_RESET, limit);
    }
    if ((rc = write_logevent_prolog(state, dp, limit)) > 0) {
        dp += rc;
    }
    PUTNEXTBYTE(dp, 0, limit); /* isEndOfBatch */
    PUTNEXTBYTE(dp, 1, limit); /* isLocationRequired */
    PUTMSGTIMESTAMP(dp, msg, limit);
    if ((rc = write_context_map(state, dp, limit, NULL)) > 0) {
        dp += rc;
    }
    if ((rc = write_context_stack(state, dp, limit, NULL)) > 0) {
        dp += rc;
    }
    if ((rc = write_level_object(state, dp, limit, msg->level)) > 0) {
        dp += rc;
    }
    if ((rc = write_string_object(
             state, dp, limit, "org.apache.logging.log4j.spi.AbstractLogger",
             43)) > 0) {
        dp += rc;
    }
    if ((rc = write_string_object(state, dp, limit, msg->logger->name,
                                  strlen(msg->logger->name))) > 0) {
        dp += rc;
    }
    PUTNEXTBYTE(dp, TC_NULL, limit); /* marker */
    if ((rc = write_message_object(state, dp, limit, msg->msg, msg->msglen)) >
        0) {
        dp += rc;
    }
    if ((rc = write_source_object(state, dp, limit, msg->file, msg->line,
                                  msg->func)) > 0) {
        dp += rc;
    }
    if ((rc = write_string_object(state, dp, limit, msg->threadid,
                                  strlen(msg->threadid))) > 0) {
        dp += rc;
    }
    PUTNEXTBYTE(dp, TC_NULL, limit); /* thrownProxy */

    if (state == &statebuf) {
        memcpy(&layout->u.jstrm, &state->u.jstrm, sizeof(layout->u.jstrm));
    }

    return (dp - buf);
}
