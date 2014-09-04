
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "barefootc/datetime.h"
#include "log4stdc.h"

#define ALMOST_EQUAL(a,b) (((a) >= ((b)-((b)/100))) && ((a) <= ((b)+((b)/100))))

int
main (int argc, char *argv)
{
	bfc_datetime_t t;
	const char *s;
	int rc;
	l4sc_logger_ptr_t logger;

	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger("barefootc.datetime", 0);

	s = "1970-03-01T13:00Z";
	rc = bfc_init_datetime_from_isotime(&t, sizeof(t), s, strlen(s));
	L4SC_DEBUG(logger, "%s: %s -> %ldd, %lus + %lu/2**32",
		__FUNCTION__, s, (long) t.day,
		(unsigned long) t.secs, (unsigned long) t.frac);
	assert (rc >= 0);
	assert (t.day == 31+28);
	assert (t.secs == 13*3600);
	assert (t.frac == 0);

	s = "1972-03-01T13:00:14Z";
	rc = bfc_init_datetime_from_isotime(&t, sizeof(t), s, strlen(s));
	L4SC_DEBUG(logger, "%s: %s -> %ldd, %lus + %lu/2**32",
		__FUNCTION__, s, (long) t.day,
		(unsigned long) t.secs, (unsigned long) t.frac);
	assert (rc >= 0);
	assert (t.day == 365+365+31+29);
	assert (t.secs == 13*3600 + 14);
	assert (t.frac == 0);

	s = "1973-04-01T08:17:57.333Z";
	rc = bfc_init_datetime_from_isotime(&t, sizeof(t), s, strlen(s));
	L4SC_DEBUG(logger, "%s: %s -> %ldd, %lus + %lu/2**32",
		__FUNCTION__, s, (long) t.day,
		(unsigned long) t.secs, (unsigned long) t.frac);
	assert (rc >= 0);
	assert (t.day == 365+365+366+31+28+31);
	assert (t.secs == 8*3600 + 17*60 + 57);
	assert (ALMOST_EQUAL(t.frac, 1430224109));

	return (0);
}
