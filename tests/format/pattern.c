
#include "compat.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

#include "log4stdc.h"
#include "logobjs.h"

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#define NEWLINE "\r\n"
#else
#define NEWLINE "\n"
#endif

void
test1(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg,
      const char *expectation, int expected_length)
{
    int bytes;
    char buf[3000];

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
test2(l4sc_layout_ptr_t layout, const char *pattern, unsigned day,
      unsigned sec, unsigned usec, unsigned line, const char *text,
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
    unsigned day = 0, sec = 0, usec = 0;
    struct tm *tm;
    time_t tt = 0;

    logger = l4sc_get_logger("testlogger", 0);

    printf("%s: layout @%p..%p\n", __FUNCTION__, &layout, (&layout) + 1);

    (*l4sc_patternlayout_class.init)(&layout, sizeof(layout), pool);

    l4sc_init_logmessage(&msg, sizeof(msg), logger, INFO_LEVEL, "one log line",
                         12, "sourcefile", 100, "testfunction");

    memset(&msg.time, 0, sizeof(msg.time));
    l4sc_snprintf(msg.threadid, sizeof(msg.threadid), "mainthread");

    test1(&layout, &msg, "one log line" NEWLINE, 12 + strlen(NEWLINE));

    day = 36 * 365 + 12 * 366 + 31 + 7; /* 8 Feb 2018 */
    sec = 13 * 3600 + 45 * 60 + 13;     /* 13:45:13 local time */
    usec = 455000;

    tt = 86400uL * day + sec;
    tm = localtime(&tt);
    sec -= 3600 * tm->tm_hour + 60 * tm->tm_min;
    tm = gmtime(&tt);
    sec += 3600 * tm->tm_hour + 60 * tm->tm_min; /* now UTC */

    /* clang-format off */
    test2(&layout, "%d{ABSOLUTE} %-5p> %m%n", 0, sec, usec, 101, "text",
          "13:45:13,455 INFO > text" NEWLINE);

    test2(&layout, "%d{ISO8601} %-5p> %m%n", day, sec, usec, 101, "text",
          "2018-02-08 13:45:13,455 INFO > text" NEWLINE);

    test2(&layout, "%d{DATE} %-5p> %m%n", day, sec, usec, 101, "text",
          "08 Feb 2018 13:45:13,455 INFO > text" NEWLINE);

    test2(&layout,
          "%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] %-5p %c[2] - %m%n",
          day, sec, usec, 102, "text",
          "2018-02-08 13:45:13.455 [mainthread] INFO  testlogger - text"
          NEWLINE);

    test2(&layout,
          "%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] %-5level %logger{2} - %msg%n",
          day, sec, usec, 102, "text",
          "2018-02-08 13:45:13.455 [mainthread] INFO  testlogger - text"
          NEWLINE);
    /* clang-format on */

    return (0);
}
