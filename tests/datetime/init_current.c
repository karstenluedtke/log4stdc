
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

#include "barefootc/datetime.h"
#include "log4stdc.h"

static l4sc_logger_ptr_t logger;

void
test1(void)
{
	time_t t0, t1;
	struct tm tm0;
	bfc_datetime_t t;
	char buf[80];

	do {
		time(&t0);
		bfc_init_current_datetime(&t, sizeof(t));
		time(&t1);
	} while (t1 != t0);

	tm0 = *gmtime(&t0);
	strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tm0);
	L4SC_INFO(logger, "%s: current systime is %s (%ld)",
		__FUNCTION__, buf, (long) t0); 
	L4SC_INFO(logger,
		"%s: current bfc_datetime is %ldT%02u:%02u:%02uZ (%ld)",
		__FUNCTION__, (long) t.day,
		t.secs/3600, (t.secs/60) % 60, t.secs % 60,
		86400uL * t.day + t.secs);

	assert (t.secs/3600 == tm0.tm_hour);
	assert ((t.secs/60) % 60 == tm0.tm_min);
	assert (t.secs % 60 == tm0.tm_sec);
}

int
main (int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_DATETIME_LOGGER);

	test1();

	return (0);
}
