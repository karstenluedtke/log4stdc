
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#define _BSD_SOURCE 1
#include <time.h>

#include "compat.h"

#include "log4stdc.h"
#include "logobjs.h"

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#define NEWLINE "\r\n"
#else
#if defined(_MSC_VER)
#define NEWLINE "\r\n"
#else
#define NEWLINE "\n"
#endif
#endif

void
test1(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
      const char *expectation, int expected_length)
{
    int bytes;
    char buf[200];

    bytes = l4sc_formatmsg(layout, msg, buf, sizeof(buf));

    if (memcmp(buf, expectation, expected_length) != 0) {
        printf("expect: %.*s\n", expected_length, expectation);
        printf("result: %.*s\n", bytes, buf);
    }
    if (bytes != expected_length) {
        printf("%d bytes, expected %d\n", bytes, expected_length);
    }
    assert(bytes == expected_length);
    assert(memcmp(buf, expectation, expected_length) == 0);
    printf("ok: %.*s\n", bytes, buf);
}

void
test2(l4sc_layout_ptr_t layout, const char *pattern, unsigned long day,
      unsigned long sec, unsigned long usec, unsigned line, const char *text,
      const char *expectation)
{
    l4sc_logger_ptr_t logger = l4sc_get_logger("testlogger", 0);
    struct l4sc_logmessage msg;

    (*l4sc_patternlayout_class.set_opt)(layout, "ConversionPattern", 17,
                                        pattern, strlen(pattern));
    l4sc_init_logmessage(&msg, sizeof(msg), logger, INFO_LEVEL, text,
                         strlen(text), "sourcefile", line, "testfunction");
    msg.time.tv_day = day;
    msg.time.tv_sec = sec;
    msg.time.tv_usec = usec;
    l4sc_snprintf(msg.threadid, sizeof(msg.threadid), "mainthread");

    test1(layout, &msg, expectation, strlen(expectation));
}

int
main(int argc, char *argv[])
{
    l4sc_logger_ptr_t logger;
    struct l4sc_layout layout;
    struct l4sc_logmessage msg;
    struct mempool *pool = get_default_mempool();
    unsigned long day = 0, sec = 0, usec = 0;
    struct tm *tm;
    struct tm localtm;
    time_t tt = 0;
    char expectation[80];
    char zonestring[40];
    char zonerfc822[20];

    logger = l4sc_get_logger("testlogger", 0);

    printf("%s: layout @%p..%p\n", __FUNCTION__, &layout, (&layout) + 1);

    (*l4sc_patternlayout_class.init)(&layout, sizeof(layout), pool);

    l4sc_init_logmessage(&msg, sizeof(msg), logger, INFO_LEVEL, "one log line",
                         12, "sourcefile", 100, "testfunction");

    memset(&msg.time, 0, sizeof(msg.time));
    l4sc_snprintf(msg.threadid, sizeof(msg.threadid), "mainthread");

    test1(&layout, &msg, "one log line" NEWLINE, 12 + strlen(NEWLINE));

    day = 36uL * 365 + 12 * 366 + 31 + 7; /* 8 Feb 2018 */
    sec = 13uL * 3600 + 45 * 60 + 13;     /* 13:45:13 local time */
    usec = 455123uL;

    for (sec = 13; sec < 86400uL; sec += 60) {
        tt = 86400uL * day + sec;
        tm = localtime(&tt);
        if ((tm->tm_mday >= 8) && (tm->tm_hour >= 13) && (tm->tm_min >= 45)) {
            memcpy(&localtm, tm, sizeof(localtm));
            break;
        }
    }

    strftime(expectation, sizeof(expectation), "%a %Y-%m-%d %H:%M:%S", tm);
    printf("sec %lu: %s\n", sec, expectation);

    /* clang-format off */
    test2(&layout, "%d{yyyy-MM-dd} %-5p> %m%n",
          day, sec, usec, 100, "text",
          "2018-02-08 INFO > text" NEWLINE);

    test2(&layout, "%d{ABSOLUTE} %-5p> %m%n", day, sec, usec, 101, "text",
          "13:45:13,455 INFO > text" NEWLINE);

    test2(&layout, "%d{ISO8601} %-5p> %m%n", day, sec, usec, 101, "text",
          "2018-02-08T13:45:13,455 INFO > text" NEWLINE);

    test2(&layout, "%d{DATE} %-5p> %m%n", day, sec, usec, 101, "text",
          "08 Feb 2018 13:45:13,455 INFO > text" NEWLINE);

    test2(&layout, "%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] %-5p %c[2] - %m%n",
          day, sec, usec, 102, "text",
          "2018-02-08 13:45:13.455 [mainthread] INFO  testlogger - text"
          NEWLINE);

    test2(&layout,
          "%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] %-5level %logger{2} - %msg%n",
          day, sec, usec, 102, "text",
          "2018-02-08 13:45:13.455 [mainthread] INFO  testlogger - text"
          NEWLINE);

    test2(&layout, "%d{EEEww HH:mm:ss.SSSSSSSSS} %-5p> %m%n",
          day, sec, usec, 103, "text",
          "Thu06 13:45:13.455123000 INFO > text" NEWLINE);

    test2(&layout, "%d{dd.MMM.yy HH:mm:ss.S} [%l] %-5p> %m%n",
          day, sec, usec, 104, "text",
          "08.Feb.18 13:45:13.4 [testfunction(sourcefile:104)] INFO > text"
          NEWLINE);

#ifdef HAVE_TM_TM_ZONE
    snprintf(expectation, sizeof(expectation), 
          "2018-02-08 13:45:13.455 %s INFO  - text" NEWLINE, localtm.tm_zone);
    test2(&layout, "%d{yyyy-MM-dd HH:mm:ss.SSS z} %-5p - %m%n",
          day, sec, usec, 105, "text", expectation);
#endif

    if (sec == 45913) /* we are in central european time zone */ {
        test2(&layout, "%d{yyyy-MM-dd HH:mm:ss.SSSZ} %-5p - %m%n",
              day, sec, usec, 105, "text",
              "2018-02-08 13:45:13.455+0100 INFO  - text" NEWLINE);

        test2(&layout, "%d{yyyy-MM-dd'T'HH:mm:ss.SSSZZZZZZ} %-5p - %m%n",
              day, sec, usec, 105, "text",
              "2018-02-08T13:45:13.455+01:00 INFO  - text" NEWLINE);
    }

    /* clang-format on */
    /* Tests derived from examples in Java SimpleDateFormat class doc */
    /* 2001-07-04 12:08:56.235 local time */
    day = 23uL * 365 + 8 * 366 + 31 + 28 + 31 + 30 + 31 + 30 + 3;
    sec = 12uL * 3600 + 8 * 60 + 56;
    usec = 235000;

    for (sec = 56; sec < 86400uL; sec += 60) {
        tt = 86400uL * day + sec;
        tm = localtime(&tt);
        if ((tm->tm_mday >= 4) && (tm->tm_hour >= 12) && (tm->tm_min >= 8)) {
            memcpy(&localtm, tm, sizeof(localtm));
            break;
        }
    }

    strftime(expectation, sizeof(expectation), "%a %Y-%m-%d %H:%M:%S", tm);
    printf("sec %lu: %s\n", sec, expectation);

    l4sc_format_jtime(zonestring, sizeof(zonestring), "z", 1, &localtm, 0);
    l4sc_format_jtime(zonerfc822, sizeof(zonerfc822), "Z", 1, &localtm, 0);

#ifdef HAVE_TM_TM_ZONE
#define zonestring localtm.tm_zone
#else
#ifdef HAVE_TM___TM_ZONE
#define zonestring localtm.__tm_zone
#endif
#endif

    /* clang-format off */
    snprintf(expectation, sizeof(expectation), 
          "2001.07.04 AD at 12:08:56 %s INFO > text" NEWLINE, zonestring);
    test2(&layout, "%d{yyyy.MM.dd G 'at' HH:mm:ss z} %-5p> %m%n",
          day, sec, usec, 201, "text", expectation);

    test2(&layout, "%d{EEE, MMM d, ''yy} %-5p> %m%n",
          day, sec, usec, 202, "text",
          "Wed, Jul 4, '01 INFO > text" NEWLINE);

    test2(&layout, "%d{h:mm a} %-5p> %m%n",
          day, sec, usec, 203, "text",
          "12:08 PM INFO > text" NEWLINE);

    snprintf(expectation, sizeof(expectation), 
          "12 o'clock PM, %s INFO > text" NEWLINE, zonestring);
    test2(&layout, "%d{hh 'o''clock' a, zzzz} %-5p> %m%n",
          day, sec, usec, 204, "text", expectation);

    snprintf(expectation, sizeof(expectation), 
          "0:08 PM, %s INFO > text" NEWLINE, zonestring);
    test2(&layout, "%d{K:mm a, z} %-5p> %m%n",
          day, sec, usec, 205, "text", expectation);

    test2(&layout, "%d{yyyyy.MMMMM.dd GGG hh:mm aaa} %-5p> %m%n",
          day, sec, usec, 206, "text",
          "02001.July.04 AD 12:08 PM INFO > text" NEWLINE);

    snprintf(expectation, sizeof(expectation), 
          "Wed, 4 Jul 2001 12:08:56 %s INFO > text" NEWLINE, zonerfc822);
    test2(&layout, "%d{EEE, d MMM yyyy HH:mm:ss Z} %-5p> %m%n",
          day, sec, usec, 207, "text", expectation);

    snprintf(expectation, sizeof(expectation), 
          "010704120856%s INFO > text" NEWLINE, zonerfc822);
    test2(&layout, "%d{yyMMddHHmmssZ} %-5p> %m%n",
          day, sec, usec, 208, "text", expectation);

    snprintf(expectation, sizeof(expectation), 
          "2001-07-04T12:08:56.235%s INFO > text" NEWLINE, zonerfc822);
    test2(&layout, "%d{yyyy-MM-dd'T'HH:mm:ss.SSSZ} %-5p> %m%n",
          day, sec, usec, 209, "text", expectation);

    /* clang-format on */

    return (0);
}
