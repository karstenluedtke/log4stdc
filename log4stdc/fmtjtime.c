
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

/*
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
        const char *nam;
        int val, len = 0;
        unsigned divisor;
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
                len = l4sc_snprintf(dp, ep - dp, "%0*d", digits, val);
            }
            break;
        case 'G': /* Era designator: AD */
            len = l4sc_snprintf(dp, ep - dp, "%-s", "AD");
            break;
        case 'y': /* Year: 1996; 96 */
            val = (digits > 2) ? (1900 + tm->tm_year) : (tm->tm_year % 100);
            len = l4sc_snprintf(dp, ep - dp, "%0*d", digits, val);
            break;
        case 'M': /* Month in year: July; Jul; 07 */
            val = tm->tm_mon + 1;
            if (digits > 3) {
                nam = month_long[val & 15];
                len = l4sc_snprintf(dp, ep - dp, "%-s", nam);
            } else if (digits > 2) {
                nam = month_short[val & 15];
                len = l4sc_snprintf(dp, ep - dp, "%-s", nam);
            } else {
                len = l4sc_snprintf(dp, ep - dp, "%0*d", digits, val);
            }
            break;
        case 'w': /* Week in year: 27 */
            // A decimal number [00,53]. Monday as the first day of the week.
            // All days in a new year preceding the first Monday are considered
            // to be in week 0 (non-ISO8601).
            val = (tm->tm_yday + 7 - (tm->tm_wday ? (tm->tm_wday - 1) : 6)) / 7;
            len = l4sc_snprintf(dp, ep - dp, "%0*d", digits, val);
            break;
        case 'W': /* Week in month: 2 */
            break;
        case 'd': /* Day in month: 10 */
            val = tm->tm_mday;
            len = l4sc_snprintf(dp, ep - dp, "%0*d", digits, val);
            break;
        case 'F': /* Day of week in month: 2 */
            break;
        case 'E': /* Day in week: Tuesday; Tue */
            val = tm->tm_wday;
            if (digits > 3) {
                nam = wday_long[val & 7];
                len = l4sc_snprintf(dp, ep - dp, "%-s", nam);
            } else {
                nam = wday_short[val & 7];
                len = l4sc_snprintf(dp, ep - dp, "%-s", nam);
            }
            break;
        case 'a': /* Am/pm marker:         PM    */
            nam = (tm->tm_hour >= 12) ? "PM" : "AM";
            len = l4sc_snprintf(dp, ep - dp, "%-s", nam);
            break;
        case 'H': /* Hour in day (0-23):   0     */
            val = tm->tm_hour;
            len = l4sc_snprintf(dp, ep - dp, "%0*d", digits, val);
            break;
        case 'k': /* Hour in day (1-24):   24    */
            val = tm->tm_hour;
            if (val == 0)
                val = 24;
            len = l4sc_snprintf(dp, ep - dp, "%0*d", digits, val);
            break;
        case 'K': /* Hour in am/pm (0-11): 0     */
            val = tm->tm_hour;
            if (val >= 12)
                val -= 12;
            len = l4sc_snprintf(dp, ep - dp, "%0*d", digits, val);
            break;
        case 'h': /* Hour in am/pm (1-12): 12    */
            val = tm->tm_hour;
            if (val == 0)
                val = 12;
            if (val > 12)
                val -= 12;
            len = l4sc_snprintf(dp, ep - dp, "%0*d", digits, val);
            break;
        case 'm': /* Minute in hour:       30    */
            val = tm->tm_min;
            len = l4sc_snprintf(dp, ep - dp, "%0*d", digits, val);
            break;
        case 's': /* Second in minute:     55    */
            val = tm->tm_sec;
            len = l4sc_snprintf(dp, ep - dp, "%0*d", digits, val);
            break;
        case 'S': /* Millisecond:          978   */
#ifdef UINT32_C
            dp[0] = (char)('0' + (usec / UINT32_C(100000)));
#else
            dp[0] = (char)('0' + (usec / 100000uL));
#endif
            len = 1;
            divisor = 10000;
            while ((len < digits) && (dp + len < ep)) {
                if (divisor > 0) {
                    dp[len] = (char)('0' + (usec / divisor) % 10);
                } else {
                    dp[len] = '0';
                }
                len++;
                divisor /= 10;
            }
            break;
        case 'z': /* Time zone: Pacific Standard Time; PST; GMT-08:00 */
#ifdef HAVE_TM_TM_ZONE
            nam = tm->tm_zone;
            len = l4sc_snprintf(dp, ep - dp, "%-s", nam);
            break;
#endif
#ifdef HAVE_TM___TM_ZONE
            nam = tm->__tm_zone;
            len = l4sc_snprintf(dp, ep - dp, "%-s", nam);
            break;
#endif
        case 'Z': /* RFC 822 time zone:    -0800 */
#ifdef HAVE_TM_TM_GMTOFF
            val = 100 * (tm->tm_gmtoff / 3600) + (tm->tm_gmtoff / 60) % 60;
            len = l4sc_snprintf(dp, ep - dp, "%+05d", val);
#endif
#ifdef HAVE_TM___TM_GMTOFF
            val = 100 * (tm->__tm_gmtoff / 3600) + (tm->__tm_gmtoff / 60) % 60;
            len = l4sc_snprintf(dp, ep - dp, "%+05d", val);
#endif
            break;
        case '\'':
            for (len=0; digits > 1; digits -= 2) {
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
