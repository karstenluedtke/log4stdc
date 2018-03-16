
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "log4stdc.h"

#define LF "\n"

/* clang-format off */
static const char configstring[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" LF
"<log4j:configuration xmlns:log4j=\"http://jakarta.apache.org/log4j/\""
							" debug=\"0\">" LF
"	<appender class=\"org.apache.log4j.FileAppender\" name=\"testlog\">" LF
"		<param name=\"File\" value=\"test.log\"/>" LF
"		<layout class=\"org.apache.log4j.PatternLayout\">" LF
"		    <param name=\"ConversionPattern\""
			 " value=\"%d{ABSOLUTE} %-5p&gt; %m%n\"/>" LF
"		</layout>" LF
"	</appender>" LF
"	<logger additivity=\"false\" name=\"testlogger\">" LF
"		<level value=\"DEBUG\"/>" LF
"		<appender-ref ref=\"testlog\"/>" LF
"	</logger>" LF
"</log4j:configuration>" LF;
/* clang-format on */

int
main(int argc, char *argv[])
{
    l4sc_logger_ptr_t logger;
    time_t t0, t1;
    FILE *fp;
    char buf[80], line[80];

    unlink("test.log");
    l4sc_configure_from_xml_string(configstring, 0);

    logger = l4sc_get_logger("testlogger", 0);
    assert(logger != NULL);

    time(&t0);
    l4sc_log(logger, DEBUG_LEVEL, "test", 4, __FILE__, __LINE__, "main");
    time(&t1);

    line[0] = '\0';

    fp = fopen("test.log", "r");
    assert(fp != NULL);
    while (fgets(buf, sizeof(buf), fp)) {
        if (strlen(buf) > 2) {
            strncpy(line, buf, sizeof(line));
        }
    }
    fclose(fp);

    assert(strstr(line, "DEBUG>") != NULL);
    assert(strstr(line, "test") != NULL);

    strftime(buf, sizeof(buf), "%H:%M:%S.000 DEBUG> test", localtime(&t0));
    l4sc_logprintf(logger, DEBUG_LEVEL, __FILE__, __LINE__, "main",
                   "testing above line >= \"%s\"", buf);
    assert(strcmp(line, buf) >= 0);

    strftime(buf, sizeof(buf), "%H:%M:%S.999 ERROR> test", localtime(&t1));
    l4sc_logprintf(logger, DEBUG_LEVEL, __FILE__, __LINE__, "main",
                   "testing above line <= \"%s\"", buf);
    assert(strcmp(line, buf) <= 0);

    return (0);
}
