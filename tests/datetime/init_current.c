
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
	int rc;
	char buf[40], buf2[25];

	do {
		time(&t0);
		rc = bfc_init_current_datetime(&t, sizeof(t));
		time(&t1);
	} while (t1 != t0);

	tm0 = *gmtime(&t0);
	strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &tm0);
	L4SC_INFO(logger, "%s: current systime is %sZ (%ld)",
		__FUNCTION__, buf, (long) t0); 
	L4SC_INFO(logger,
		"%s: current bfc_datetime is %ldT%02u:%02u:%02uZ (%ld)",
		__FUNCTION__, (long) t.day,
		t.secs/3600, (t.secs/60) % 60, t.secs % 60,
		86400uL * t.day + t.secs);

	assert (rc >= 0);
	assert (t.secs/3600 == tm0.tm_hour);
	assert ((t.secs/60) % 60 == tm0.tm_min);
	assert (t.secs % 60 == tm0.tm_sec);

	rc = bfc_datetime_to_isodate(&t, buf2, sizeof(buf2));
	L4SC_INFO(logger, "%s: formatted bfc_datetime is %s",
		__FUNCTION__, buf2);
	assert (strncmp(buf2, buf, strlen(buf)) == 0);
	assert (rc == 24);
}

int
main (int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_DATETIME_LOGGER);

	test1();

	return (0);
}
