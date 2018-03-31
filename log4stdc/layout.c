
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "compat.h"
#include "logobjs.h"

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#define L4SC_USE_WINDOWS_LOCALTIME 1
#define L4SC_USE_WINDOWS_STRFTIME 1
#endif

#if defined(L4SC_USE_WINDOWS_LOCALTIME)
#include <windows.h>
#if defined(HAVE__LOCALTIME64_S)
errno_t
_localtime64_s(struct tm *, const __time64_t *);
#else
struct tm *
_localtime64(const __time64_t *);
#endif
#endif

#if defined(HAVE_STDINT_H)
#include <stdint.h>
#endif

static int
init_patternlayout(void *, size_t, bfc_mempool_t);
static size_t
get_layout_size(l4sc_layout_cptr_t obj);
static unsigned
get_layout_hashcode(l4sc_layout_cptr_t obj, int hashlen);
static int
compare_layout(l4sc_layout_cptr_t obj, l4sc_layout_cptr_t other);
static size_t
get_layout_length(l4sc_layout_cptr_t obj);
static int
layout_tostring(l4sc_layout_cptr_t obj, char *buf, size_t bufsize,
                const char *fmt);

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
estimate_size(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg);

static size_t
format_by_pattern(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
                  char *buf, size_t bufsize);

static size_t
format_header(l4sc_layout_ptr_t layout, int kind, char *buf, size_t bufsize);

static int
format_logtime(char *buf, size_t bufsize, const char *fmt, const char *datefmt,
               size_t datefmtlen, l4sc_logmessage_cptr_t msg);

static int
format_datespec(char *buf, size_t bufsize, const char *fmt, const char *lim);

const struct l4sc_layout_class l4sc_patternlayout_class = {
    /* .super        */ (l4sc_layout_class_ptr_t)&l4sc_object_class,
    /* .name         */ "layout",
    /* .spare2       */ NULL,
    /* .spare3       */ NULL,
    /* .init         */ init_patternlayout,
    /* .initrefc     */ (void *)l4sc_default_init_refcount,
    /* .incrrefc     */ (void *)l4sc_default_incr_refcount,
    /* .decrrefc     */ (void *)l4sc_default_decr_refcount,
    /* .destroy      */ NULL, /* inherit */
    /* .clone        */ (void *)l4sc_default_clone_object,
    /* .clonesize    */ get_layout_size,
    /* .compare      */ compare_layout,
    /* .hashcode     */ get_layout_hashcode,
    /* .length       */ get_layout_length,
    /* .tostring     */ layout_tostring,
    /* .dump         */ NULL, /* inherit */
    /* .set_name     */ set_layout_name,
    /* .set_opt      */ set_layout_option,
    /* .get_opt      */ get_layout_option,
    /* .apply        */ apply_layout_options,
    /* .close        */ NULL,
    /* .format       */ format_by_pattern,
    /* .header       */ format_header,
    /* .estimate     */ estimate_size};

extern const struct l4sc_layout_class l4sc_json_stream_layout_class;
extern const struct l4sc_layout_class l4sc_log4j_stream_layout_class;
extern const struct l4sc_layout_class l4sc_log4j2_stream_layout_class;

static int
init_patternlayout(void *buf, size_t bufsize, bfc_mempool_t pool)
{
    l4sc_layout_ptr_t existing = (l4sc_layout_ptr_t)buf;
    if (BFC_CLASS(existing) != &l4sc_patternlayout_class) {
        BFC_INIT_PROLOGUE(l4sc_layout_class_ptr_t, l4sc_layout_ptr_t, layout,
                          buf, bufsize, pool, &l4sc_patternlayout_class);
        layout->name = "pattern layout";
        strncpy(layout->u.patternlayout.pattern, "%m%n",
                sizeof(layout->u.patternlayout.pattern));
        strncpy(layout->u.patternlayout.datefmt, "%Y-%m-%d %H:%M:%S",
                sizeof(layout->u.patternlayout.datefmt));
    }
    return (BFC_SUCCESS);
}

static size_t
get_layout_size(l4sc_layout_cptr_t obj)
{
    return (sizeof(struct l4sc_layout));
}

static unsigned
get_layout_hashcode(l4sc_layout_cptr_t obj, int hashlen)
{
    unsigned x = 0;
    const unsigned char *cp;
    for (cp = (unsigned char *)obj->u.patternlayout.pattern; *cp; cp++) {
        x = (x << 7) ^ ((x >> (8 * sizeof(x) - 7)) & 0x7f) ^ *cp;
    }
    return (l4sc_reduce_hashcode(x, 8 * sizeof(x), hashlen));
}

static int
compare_layout(l4sc_layout_cptr_t obj, l4sc_layout_cptr_t other)
{
    if (other == obj) {
        return (0);
    } else if (BFC_CLASS(other) == BFC_CLASS(obj)) {
        return (strcmp(other->u.patternlayout.pattern,
                       obj->u.patternlayout.pattern));
    }
    return (1);
}

static size_t
get_layout_length(l4sc_layout_cptr_t obj)
{
    return (0);
}

static int
layout_tostring(l4sc_layout_cptr_t obj, char *buf, size_t bufsize,
                const char *fmt)
{
    if (obj && obj->u.patternlayout.pattern && buf) {
        size_t patlen = strlen(obj->u.patternlayout.pattern);
        if (bufsize > patlen) {
            memcpy(buf, obj->u.patternlayout.pattern, patlen);
            buf[patlen] = 0;
            return ((int)patlen);
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
            obj->namebuf[sizeof(obj->namebuf) - 1] = 0;
            obj->name = obj->namebuf;
        } else if (len > 0) {
            memcpy(obj->namebuf, name, len);
            obj->namebuf[len] = 0;
            obj->name = obj->namebuf;
        }
    }
}

static const char *
skip_width_and_prec(const char *s, const char *limit)
{
    const char *cp = s;
    if ((cp < limit) && (*cp == '-')) {
        cp++;
    }
    while ((cp < limit) && (((*cp >= '0') && (*cp <= '9')) || (*cp == '.'))) {
        cp++;
    }
    return (cp);
}

static int
set_layout_option(l4sc_layout_ptr_t obj, const char *name, size_t namelen,
                  const char *value, size_t vallen)
{
    LOGINFO(("%s: %.*s=\"%.*s\"", __FUNCTION__, (int)namelen, name,
             (int)vallen, value));

    if ((namelen == 17) && (strncasecmp(name, "ConversionPattern", 17) == 0)) {
        const char *cp, *limit = value + vallen;
        const size_t bufsize = sizeof(obj->u.patternlayout.pattern);
        if (vallen < bufsize) {
            memcpy(obj->u.patternlayout.pattern, value, vallen);
            obj->u.patternlayout.pattern[vallen] = 0;
        } else {
            memcpy(obj->u.patternlayout.pattern, value, bufsize - 1);
            obj->u.patternlayout.pattern[bufsize - 1] = 0;
        }
        for (cp = value; (cp + 2 < limit) && *cp; cp++) {
            if (cp[0] == '%') {
                cp = skip_width_and_prec(cp + 1, limit);
                if ((cp + 2 < limit) && (cp[0] == 'd') && (cp[1] == '{')) {
                    const char *ep = memchr(cp + 2, '}', limit - (cp + 2));
                    /* The date of the logging event.
                     * The date conversion specifier may be followed
                     * by a set of braces containing a pattern string.
                     */
                    format_datespec(obj->u.patternlayout.datefmt,
                                    sizeof(obj->u.patternlayout.datefmt),
                                    cp + 2, ep ? ep : limit);
                    break;
                }
            }
        }
    } else if ((namelen == 5) && (strncasecmp(name, "class", 5) == 0)) {
        l4sc_set_layout_class_by_name(obj, value, vallen);
    }
    return (0);
}

int
l4sc_set_layout_class_by_name(l4sc_layout_ptr_t obj, const char *value,
                              size_t vallen)
{
    l4sc_class_ptr_t cls;
    static const char thisfunction[] = "set_layout_class_by_name";

    if ((vallen >= 13) &&
        (strncasecmp(value + vallen - 13, "PatternLayout", 13) == 0)) {
        obj->vptr = &l4sc_patternlayout_class;
    } else if ((cls = l4sc_get_extra_class(value, vallen)) != NULL) {
        obj->vptr = (l4sc_layout_class_ptr_t)cls;
    } else {
        LOGERROR(("%s: unknown layout class %.*s", thisfunction, (int)vallen,
                  value));
        return (-ENOENT);
    }
    LOGINFO(("%s: set layout class %s", thisfunction, obj->vptr->name));
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
estimate_size(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg)
{
    return (msg->msglen + 200);
}

static size_t
format_by_pattern(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
                  char *buf, size_t bufsize)
{
    char *dp = buf;
    const char *limit = buf + bufsize;
    const char *cp = layout->u.patternlayout.pattern;
    const char *percent, *datefmt;
    char c;
    size_t len, datefmtlen;
    int level = msg ? msg->level : 0;
    char fmt[12];

    while ((c = *(cp++)) && (dp < limit)) {
        if (c == '%') {
            percent = cp - 1;
            cp = skip_width_and_prec(cp, cp + 9);
            if (cp < percent + sizeof(fmt) - 2) {
                memcpy(fmt, percent, cp - percent);
                fmt[cp - percent] = 's';
                fmt[cp - percent + 1] = 0;
            } else {
                fmt[0] = '%';
                fmt[1] = 's';
                fmt[2] = 0;
            }
            len = 0;
            c = *cp++;
            if (c == 'l') {
                if (memcmp(cp, /*l*/ "evel", 4) == 0) {
                    c = 'p';
                    cp += 4;
                } else if (memcmp(cp, /*l*/ "ogger", 5) == 0) {
                    c = 'c';
                    cp += 5;
                }
            }
            switch (c) {
            case 0:
                --cp; /* let cp point to '\0' again */
                break;
            case 'c':
                /* The logger of the logging event.
                 * The logger conversion specifier can be
                 * optionally followed by precision specifier,
                 * that is a decimal constant in brackets.
                 */
                len = l4sc_snprintf(dp, limit - dp, fmt, msg->logger->name);
                if (*cp == '[') {
                    const char *sep = strchr(cp, ']');
                    if (sep) {
                        cp = sep + 1;
                    }
                } else if (*cp == '{') {
                    const char *sep = strchr(cp, '}');
                    if (sep) {
                        cp = sep + 1;
                    }
                }
                break;
            case 'd':
                /* The date of the logging event.
                 * The date conversion specifier may be followed
                 * by a set of braces containing a pattern string.
                 */
                datefmt = "yyyy-MM-dd HH-mm-ss";
                datefmtlen = 19;
                if (*cp == '{') {
                    const char *sep = strchr(cp, '}');
                    if (sep) {
                        datefmt = cp + 1;
                        datefmtlen = sep - datefmt;
                        cp = sep + 1;
                    }
                }
                len = format_logtime(dp, limit - dp, fmt, datefmt, datefmtlen,
                                     msg);
                break;
            case 'F':
                len = l4sc_snprintf(dp, limit - dp, fmt, msg->file);
                break;
            case 'l':
                len = l4sc_snprintf(dp, limit - dp, "%s(%s:%d)", msg->func,
                                    msg->file, msg->line);
                break;
            case 'L':
                len = l4sc_snprintf(dp, limit - dp, "%d", msg->line);
                break;
            case 'm':
                if (memcmp(cp, /*m*/ "sg", 2) == 0) {
                    cp += 2;
                }
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
                if ((dp < buf + 1) || (dp[-1] != '\n')) {
#if defined(_MSC_VER) || defined(_WIN32) || defined(__MINGW32__)
                    if (dp + 2 < limit) {
                        dp[0] = '\r';
                        dp[1] = '\n';
                        len = 2;
                    }
#else
                    if (dp < limit) {
                        *dp = '\n';
                        len = 1;
                    }
#endif
                }
                break;
            case 'p':
                /* clang-format off */
                len = l4sc_snprintf(dp, limit - dp, fmt,
                        IS_AT_LEAST_FATAL_LEVEL(level)? "FATAL":
                        IS_AT_LEAST_ERROR_LEVEL(level)? "ERROR":
                        IS_AT_LEAST_WARN_LEVEL(level)?  "WARN":
                        IS_AT_LEAST_INFO_LEVEL(level)?  "INFO":
                        IS_AT_LEAST_DEBUG_LEVEL(level)? "DEBUG": "TRACE");
                /* clang-format on */
                break;
            case 'r':
                len = l4sc_snprintf(dp, limit - dp, "%lu",
                                    1000 * (unsigned long)msg->time.tv_sec +
                                        msg->time.tv_usec / 1000);
                break;
            case 't':
                len = l4sc_snprintf(dp, limit - dp, fmt, msg->threadid);
                break;
            default:
                *dp = c;
                len = 1;
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

#define MILLISEC_PLACEHOLDER 0x1a /* ASCII SUB -- substitute */

/*
 * The date conversion specifier may be followed by a set of braces containing
 * a date and time pattern string compatible with java.text.SimpleDateFormat,
 * ABSOLUTE, DATE or ISO8601.
 * For example, d{HH:mm:ss,SSS}, d{dd MMM yyyy HH:mm:ss,SSS} or d{DATE}.
 * If no date format specifier is given then ISO8601 format is assumed.
 */

static int
format_datespec(char *buf, size_t bufsize, const char *fmt, const char *lim)
{
    char *dp = buf;
    const char *cp = fmt;

    while ((cp < lim) && *cp && (dp + 3 < buf + bufsize)) {
        const char c = *cp++;
        int digits = 1;
        while ((cp < lim) && (*cp == c)) {
            digits++;
            cp++;
        }
        switch (c) {
        case 'A':
            if (strncmp(cp, /*A*/ "BSOLUTE", 7) == 0) {
                cp += 7;
                if (dp + 13 < buf + bufsize) {
                    memcpy(dp, "%H:%M:%S,", 9);
                    memset(dp + 9, MILLISEC_PLACEHOLDER, 3);
                    dp += 12;
                }
            }
            break;
        case 'I':
            if (strncmp(cp, /*I*/ "SO8601", 6) == 0) {
                cp += 6;
                if (dp + 22 < buf + bufsize) {
                    memcpy(dp, "%Y-%m-%d %H:%M:%S,", 18);
                    memset(dp + 18, MILLISEC_PLACEHOLDER, 3);
                    dp += 21;
                } else if (dp + 16 < buf + bufsize) {
                    memcpy(dp, "%F %H:%M:%S,", 12);
                    memset(dp + 12, MILLISEC_PLACEHOLDER, 3);
                    dp += 15;
                }
            }
            break;
        case 'D':
            if (strncmp(cp, /*D*/ "ATE", 3) == 0) {
                cp += 3;
                if (dp + 22 < buf + bufsize) {
#ifdef L4SC_USE_WINDOWS_STRFTIME
                    memcpy(dp, "%d %m %Y %H:%M:%S,", 18);
#else
                    memcpy(dp, "%d %b %Y %H:%M:%S,", 18);
#endif
                    memset(dp + 18, MILLISEC_PLACEHOLDER, 3);
                    dp += 21;
                }
            } else { /* Day in year: 189 */
                *(dp++) = '%';
                *(dp++) = 'j';
            }
            break;
        case 'G': /* Era designator: AD */
            *(dp++) = 'A';
            *(dp++) = 'D';
            break;
        case 'y': /* Year: 1996; 96 */
            *(dp++) = '%';
            *(dp++) = (digits > 2) ? 'Y' : 'y';
            break;
        case 'M': /* Month in year: July; Jul; 07 */
            *(dp++) = '%';
            *(dp++) = (digits > 3) ? 'B' : (digits > 2) ? 'b' : 'm';
            break;
        case 'w': /* Week in year: 27 */
#ifdef L4SC_USE_WINDOWS_STRFTIME
#else
            *(dp++) = '%';
            *(dp++) = 'V';
#endif
            break;
        case 'W': /* Week in month: 2 */
            break;
        case 'd': /* Day in month: 10 */
            *(dp++) = '%';
            *(dp++) = 'd';
            break;
        case 'F': /* Day of week in month: 2 */
            break;
        case 'E': /* Day in week: Tuesday; Tue */
            *(dp++) = '%';
            *(dp++) = (digits > 3) ? 'A' : 'a';
            break;
        case 'a': /* Am/pm marker:         PM    */
            *(dp++) = '%';
            *(dp++) = 'p';
            break;
        case 'H': /* Hour in day (0-23):   0     */
        case 'k': /* Hour in day (1-24):   24    */
            *(dp++) = '%';
            *(dp++) = 'H';
            break;
        case 'K': /* Hour in am/pm (0-11): 0     */
        case 'h': /* Hour in am/pm (1-12): 12    */
            *(dp++) = '%';
            *(dp++) = 'I';
            break;
        case 'm': /* Minute in hour:       30    */
            *(dp++) = '%';
            *(dp++) = 'M';
            break;
        case 's': /* Second in minute:     55    */
            *(dp++) = '%';
            *(dp++) = 'S';
            break;
        case 'S': /* Millisecond:          978   */
            if (dp + digits + 1 < buf + bufsize) {
                memset(dp, MILLISEC_PLACEHOLDER, digits);
                dp += digits;
            } else {
                *(dp++) = MILLISEC_PLACEHOLDER;
                *(dp++) = MILLISEC_PLACEHOLDER;
            }
            break;
        case 'z': /* Time zone: Pacific Standard Time; PST; GMT-08:00 */
            *(dp++) = '%';
            *(dp++) = 'Z';
            break;
        case 'Z': /* RFC 822 time zone:    -0800 */
            *(dp++) = '%';
            *(dp++) = 'z';
            break;
        default:
            *(dp++) = c;
        }
    }
    if (dp < buf + bufsize) {
        *dp = '\0';
    } else if (bufsize > 0) {
        buf[bufsize - 1] = 0;
    }
    LOGDEBUG(("%s(%.*s): %s\n", __FUNCTION__, (int)(lim - fmt), fmt, buf));
    return ((int)(dp - buf));
}

static int
format_logtime(char *buf, size_t bufsize, const char *fmt, const char *datefmt,
               size_t datefmtlen, l4sc_logmessage_cptr_t msg)
{
    char *dp = buf;
    const char *limit = buf + bufsize;
    int n;
    struct tm tmbuf;
    char tmp[40];

    if ((fmt[1] != 's') || (fmt[2] != 0) || (bufsize < 9)) {
        dp = tmp;
        limit = tmp + sizeof(tmp);
    }
#if defined(L4SC_USE_WINDOWS_LOCALTIME)
    do {
        __time64_t t;
        t = msg->time.tv_day;
        t = (t * 86400L) + msg->time.tv_sec;
#if defined(HAVE__LOCALTIME64_S)
        _localtime64_s(&tmbuf, &t);
#else
        const struct tm *tm2 = _localtime64(&t);
        if (tm2 != NULL) {
            tmbuf = *tm2;
        } else {
            memset(&tmbuf, 0, sizeof(tmbuf));
        }
#endif
    } while (0 /* just once */);

#else /* not L4SC_USE_WINDOWS_LOCALTIME */
    do {
        time_t t;
        t = (time_t)msg->time.tv_day;
        if ((t > 24000 /* after 2035 */) && (sizeof(t) == 4)) {
            do {
                t -= (20 * 365 + 5); /* subtract 20 years */
            } while (t > 24000);
        }
        t = (t * 86400L) + msg->time.tv_sec;
#ifdef HAVE_LOCALTIME_R
        localtime_r(&t, &tmbuf);
#else
        memcpy(&tmbuf, localtime(&t), sizeof(tmbuf));
#endif
    } while (0 /* just once */);
#endif
    n = l4sc_format_jtime(dp, limit - dp, datefmt, datefmtlen, &tmbuf,
                          msg->time.tv_usec);
    if ((n > 0) && (dp + n < limit)) {
        dp += n;
    }
    if (limit == tmp + sizeof(tmp)) {
        return (l4sc_snprintf(buf, bufsize, fmt, tmp));
    }
    return ((int)(dp - buf));
}

size_t
l4sc_layout_estimate(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg)
{
    RETURN_METHCALL(l4sc_layout_class_ptr_t, layout, estimate, (layout, msg),
                    estimate_size(layout, msg));
}
