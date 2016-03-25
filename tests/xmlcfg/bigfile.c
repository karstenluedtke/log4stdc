
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "log4stdc.h"

#define LF "\n"

static const char *configlines[] = {
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>",
"<log4j:configuration xmlns:log4j=\"http://jakarta.apache.org/log4j/\""
							" debug=\"0\">",
"	<appender class=\"org.apache.log4j.ConsoleAppender\""
		" name=\"Console\">",
"		<layout class=\"org.apache.log4j.PatternLayout\">",
"		    <param name=\"ConversionPattern\""
			 " value=\"%d{ABSOLUTE} [%t] %-5p %c - %m%n\"/>",
"		</layout>",
"	</appender>",
"	<logger additivity=\"false\" name=\"barefootc\">",
"		<level value=\"WARN\"/>",
"		<appender-ref ref=\"Console\"/>",
"	</logger>",
"	<logger name=\"barefootc.string\">",
"		<level value=\"ERROR\"/>",
"	</logger>",
"	<logger name=\"barefootc.vector\">",
"		<level value=\"DEBUG\"/>",
"	</logger>",
"	<logger name=\"barefootc.vector.set\">",
"		<level value=\"INHERIT\"/>",
"	</logger>",
"</log4j:configuration>",
NULL };

static const char *emptyline = 
"                                                                        ";

int
main (int argc, char *argv[])
{
	FILE *fp;
	int i, j, rc;
	l4sc_logger_ptr_t logger;
	int emptylines = (sizeof(void *) == 2)? 10: 100;

	fp = fopen("test.xml", "w");
	assert (fp != NULL);

	for (i=0; configlines[i] != NULL; i++) {
		for (j=0; j < emptylines; j++) {
			fprintf(fp, "%s\n", emptyline);
		}
		fprintf(fp, "%s\n", configlines[i]);
	}
	fclose(fp);

	rc = l4sc_configure_from_xml_file("test.xml");
	assert (rc >= 0);

	logger = l4sc_get_logger("barefootc", 0);
	assert (logger != NULL);
	assert (l4sc_error_enabled(logger));
	assert (l4sc_warn_enabled(logger));
	assert (!l4sc_info_enabled(logger));
	assert (!l4sc_debug_enabled(logger));
	assert (!l4sc_trace_enabled(logger));

	logger = l4sc_get_logger("barefootc.string", 0);
	assert (logger != NULL);
	assert (l4sc_error_enabled(logger));
	assert (!l4sc_warn_enabled(logger));
	assert (!l4sc_info_enabled(logger));
	assert (!l4sc_debug_enabled(logger));
	assert (!l4sc_trace_enabled(logger));

	exit(0);
}

