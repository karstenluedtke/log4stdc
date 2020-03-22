
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#define _BSD_SOURCE 1
#include <time.h>

#include "compat.h"
#include "logobjs.h"

#if defined(HAVE_STDINT_H)
#include <stdint.h>
#endif

static const char *wday_short[8] = {"Sun", "Mon", "Tue", "Wed",
                                    "Thu", "Fri", "Sat", "Sun"};

static const char *wday_long[8] = {"Sunday",    "Monday",   "Tuesday",
                                   "Wednesday", "Thursday", "Friday",
                                   "Saturday",  "Sunday"};

static const char *month_short[16] = {"Dec", "Jan", "Feb", "Mar", "Apr", "May",
                                      "Jun", "Jul", "Aug", "Sep", "Oct", "Nov",
                                      "Dec", "Jan", "Feb", "Mar"};

static const char *month_long[16] = {
    "December", "January", "February", "March",     "April",   "May",
    "June",     "July",    "August",   "September", "October", "November",
    "December", "January", "February", "March"};

/**
 * Text is formatted left-aligned and without padding.
 * For formatting, if the number of pattern letters is 4 or more, the full
 * form is used; otherwise a short or abbreviated form is used if available.
 */
static int
put_text(char *buf, size_t bufsize, const char *s)
{
    int len = strlen(s);

    if (len < (int)bufsize) {
        memcpy(buf, s, len);
    } else if ((int)bufsize > 0) {
        memcpy(buf, s, bufsize);
    }
    return (len);
}

/**
 * Number:
 * For formatting, the number of pattern letters is the minimum number
 * of digits, and shorter numbers are zero-padded to this amount.
 */
static int
put_unsigned(char *buf, size_t bufsize, int width, unsigned val)
{
    int len = 0;
    unsigned divisor = 1;

    /* handle most common cases first */
    if ((val < 10) && (width + 1 < (int)bufsize)) {
        while (len + 1 < width) {
            buf[len++] = '0';
        }
        buf[len++] = (char)('0' + val);
        return (len);
    } else if ((val < 60) && (width + 2 < (int)bufsize)) {
        char c1, c0;
        /* clang-format off */
        if (val < 20) {
            c1 = '1'; c0 = (char)('0' + val - 10);
        } else if (val < 40) {
            if (val < 30) {
                c1 = '2'; c0 = (char)('0' + val - 20);
            } else /* val < 40 */ {
                c1 = '3'; c0 = (char)('0' + val - 30);
            }
        } else {
            if (val < 50) {
                c1 = '4'; c0 = (char)('0' + val - 40);
            } else /* val < 60 */ {
                c1 = '5'; c0 = (char)('0' + val - 50);
            }
        }
        /* clang-format on */
        while (len + 2 < width) {
            buf[len++] = '0';
        }
        buf[len++] = c1;
        buf[len++] = c0;
        return (len);
    }
    for (len = 1; len < width; ++len) {
        divisor *= 10;
    }
    if (divisor < val) {
        while (val / divisor > 10) {
            len += 1;
            divisor *= 10;
        }
    }
    if ((int)bufsize > 0) {
        unsigned q = val / divisor;
        unsigned remainder = val - (q * divisor);
        buf[0] = (char)('0' + q);
        divisor /= 10;
        for (len = 1; (divisor >= 1) && (len < (int)bufsize); len++) {
            q = remainder / divisor;
            remainder -= (q * divisor);
            divisor /= 10;
            buf[len] = (char)('0' + q);
        }
    }
    return (len);
}

static int
put_millis(char *buf, size_t bufsize, int width, long usec)
{
    int len = width;
    const char *limit = buf + bufsize;

    if ((int)bufsize > 0) {
        unsigned divisor = 10000;
#ifdef UINT32_C
        unsigned q = usec / UINT32_C(100000);
        unsigned remainder = usec - (q * UINT32_C(100000));
#else
        unsigned q = usec / 100000uL;
        unsigned remainder = usec - (q * 100000uL);
#endif
        buf[0] = (char)('0' + q);
        len = 1;
        while ((len < width) && (divisor > 0) && (buf + len < limit)) {
            q = remainder / divisor;
            remainder -= (q * divisor);
            divisor /= 10;
            buf[len++] = (char)('0' + q);
        }
        while ((len < width) && (buf + len < limit)) {
            buf[len++] = '0';
        }
    }
    return (len);
}

#if defined(HAVE_TM_TM_GMTOFF) || defined(HAVE_TM___TM_GMTOFF)
static int
put_rfc822zone(char *buf, size_t bufsize, int width, int offs)
{
    int len = 5;
    unsigned hr, mn;

    if ((int)bufsize >= 5) {
        if (offs >= 0) {
            buf[0] = '+';
            hr = (unsigned)offs / 60;
            mn = (unsigned)offs - hr * 60;
        } else {
            buf[0] = '-';
            hr = (unsigned)(-offs) / 60;
            mn = (unsigned)(-offs) - hr * 60;
        }
        len = 1 + put_unsigned(buf + 1, bufsize - 1, 2, hr);
        if ((width == 6) && ((int)bufsize >= 6)) {
            buf[3] = ':';
            len += 1 + put_unsigned(buf + 4, bufsize - 4, 2, mn);
        } else {
            len += put_unsigned(buf + 3, bufsize - 3, 2, mn);
        }
    }
    return (len);
}
#endif

/**
 * The date conversion specifier may be followed by a set of braces containing
 * a date and time pattern string compatible with java.text.SimpleDateFormat,
 * ABSOLUTE, DATE or ISO8601.
 * For example, d{HH:mm:ss,SSS}, d{dd MMM yyyy HH:mm:ss,SSS} or d{DATE}.
 * If no date format specifier is given then ISO8601 format is assumed.
 */

int
l4sc_format_jtime(char *buf, size_t bufsize, const char *fmt, size_t fmtsize,
                  struct tm *tm, long usec)
{
    char *dp = buf;
    const char *ep = buf + bufsize;
    const char *cp = fmt;
    const char *lim = fmt + fmtsize;

    while ((cp < lim) && *cp && (dp < ep)) {
        const char c = *cp++;
        int digits = 1;
        int val, len = 0;
        while ((cp < lim) && (*cp == c)) {
            digits++;
            cp++;
        }
        switch (c) {
        case 'A':
            if (strncmp(cp, /*A*/ "BSOLUTE", 7) == 0) {
                static const char absfmt[] = "HH:mm:ss,SSS";
                return l4sc_format_jtime(buf, bufsize, absfmt, 12, tm, usec);
            }
            break;
        case 'I':
            if (strncmp(cp, /*I*/ "SO8601", 6) == 0) {
                static const char isofmt[] = "yyyy-MM-dd HH:mm:ss,SSS";
                return l4sc_format_jtime(buf, bufsize, isofmt, 23, tm, usec);
            }
            break;
        case 'D':
            if (strncmp(cp, /*D*/ "ATE", 3) == 0) {
                static const char datfmt[] = "dd MMM yyyy HH:mm:ss,SSS";
                return l4sc_format_jtime(buf, bufsize, datfmt, 24, tm, usec);
            } else { /* Day in year: 189 */
                val = tm->tm_yday + 1;
                len = put_unsigned(dp, ep - dp, digits, val);
            }
            break;
        case 'G': /* Era designator: AD */
            len = put_text(dp, ep - dp, "AD");
            break;
        case 'y': /* Year: 1996; 96 */
            val = (digits > 2) ? (1900 + tm->tm_year) : (tm->tm_year % 100);
            len = put_unsigned(dp, ep - dp, digits, val);
            break;
        case 'M': /* Month in year: July; Jul; 07 */
            val = tm->tm_mon + 1;
            if (digits > 3) {
                len = put_text(dp, ep - dp, month_long[val & 15]);
            } else if (digits > 2) {
                len = put_text(dp, ep - dp, month_short[val & 15]);
            } else {
                len = put_unsigned(dp, ep - dp, digits, val);
            }
            break;
        case 'w': /* Week in year: 27 */
            // A decimal number [00,53]. Monday as the first day of the week.
            // All days in a new year preceding the first Monday are considered
            // to be in week 0 (non-ISO8601).
            val =
                (tm->tm_yday + 7 - (tm->tm_wday ? (tm->tm_wday - 1) : 6)) / 7;
            len = put_unsigned(dp, ep - dp, digits, val);
            break;
        case 'W': /* Week in month: 2 */
            break;
        case 'd': /* Day in month: 10 */
            val = tm->tm_mday;
            len = put_unsigned(dp, ep - dp, digits, val);
            break;
        case 'F': /* Day of week in month: 2 */
            break;
        case 'E': /* Day in week: Tuesday; Tue */
            val = tm->tm_wday;
            if (digits > 3) {
                len = put_text(dp, ep - dp, wday_long[val & 7]);
            } else {
                len = put_text(dp, ep - dp, wday_short[val & 7]);
            }
            break;
        case 'a': /* Am/pm marker:         PM    */
            len = put_text(dp, ep - dp, (tm->tm_hour >= 12) ? "PM" : "AM");
            break;
        case 'H': /* Hour in day (0-23):   0     */
            val = tm->tm_hour;
            len = put_unsigned(dp, ep - dp, digits, val);
            break;
        case 'k': /* Hour in day (1-24):   24    */
            val = tm->tm_hour;
            if (val == 0)
                val = 24;
            len = put_unsigned(dp, ep - dp, digits, val);
            break;
        case 'K': /* Hour in am/pm (0-11): 0     */
            val = tm->tm_hour;
            if (val >= 12)
                val -= 12;
            len = put_unsigned(dp, ep - dp, digits, val);
            break;
        case 'h': /* Hour in am/pm (1-12): 12    */
            val = tm->tm_hour;
            if (val == 0)
                val = 12;
            if (val > 12)
                val -= 12;
            len = put_unsigned(dp, ep - dp, digits, val);
            break;
        case 'm': /* Minute in hour:       30    */
            val = tm->tm_min;
            len = put_unsigned(dp, ep - dp, digits, val);
            break;
        case 's': /* Second in minute:     55    */
            val = tm->tm_sec;
            len = put_unsigned(dp, ep - dp, digits, val);
            break;
        case 'S': /* Millisecond:          978   */
            len = put_millis(dp, ep - dp, digits, usec);
            break;
        case 'z': /* Time zone: Pacific Standard Time; PST; GMT-08:00 */
#ifdef HAVE_TM_TM_ZONE
            len = put_text(dp, ep - dp, tm->tm_zone);
            break;
#endif
#ifdef HAVE_TM___TM_ZONE
            len = put_text(dp, ep - dp, tm->__tm_zone);
            break;
#endif
        case 'Z': /* RFC 822 time zone:    -0800 */
#ifdef HAVE_TM_TM_GMTOFF
            len = put_rfc822zone(dp, ep - dp, digits, tm->tm_gmtoff / 60);
#endif
#ifdef HAVE_TM___TM_GMTOFF
            len = put_rfc822zone(dp, ep - dp, digits, tm->__tm_gmtoff / 60);
#endif
            break;
        case '\'':
            for (len = 0; digits > 1; digits -= 2) {
                if (dp + len < ep) {
                    dp[len++] = '\'';
                }
            }
            if (digits == 1) {
                for (; (cp < lim) && *cp; ++cp) {
                    if (*cp == '\'') {
                        if (*++cp != '\'') {
                            break;
                        }
                    }
                    if (dp + len < ep) {
                        dp[len++] = *cp;
                    }
                }
            }
            break;
        default:
            len = (dp + digits < ep) ? digits : (int)(ep - dp - 1);
            if (len > 0) {
                memset(dp, c, digits);
            }
        }
        if (len > 0) {
            if (dp + len < ep) {
                dp += len;
            } else {
                break;
            }
        }
    }
    if (dp < buf + bufsize) {
        *dp = '\0';
    } else if (bufsize > 0) {
        buf[bufsize - 1] = 0;
    }
    return ((int)(dp - buf));
}
