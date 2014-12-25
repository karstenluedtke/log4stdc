
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
test1(const char *s)
{
	time_t t0;
	struct tm tm0;
	bfc_datetime_t t;
	int rc;
	char buf[40], buf2[25];

	rc = bfc_init_datetime_from_isotime(&t, sizeof(t), s, strlen(s));
	assert (rc >= 0);

	t0 = (time_t) 86400 * t.day + t.secs;
	tm0 = *gmtime(&t0);
	strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &tm0);

	assert (t.secs/3600 == tm0.tm_hour);
	assert ((t.secs/60) % 60 == tm0.tm_min);
	assert (t.secs % 60 == tm0.tm_sec);

	rc = bfc_datetime_to_isodate(&t, buf2, sizeof(buf2));
	L4SC_DEBUG(logger, "%s: %s -> %s", __FUNCTION__, s, buf2);
	assert (rc == 24);
	assert (strncmp(buf2, s, strlen(buf)) == 0);
	assert (strncmp(buf2, buf, strlen(buf)) == 0);
}

void
test2(const char *s, int offs)
{
	bfc_datetime_t t, t0;
	int rc;
	char buf[40], buf1[25], buf2[25];

	rc = bfc_init_datetime_from_isotime(&t, sizeof(t), s, strlen(s));
	assert (rc >= 0);

	rc = bfc_datetime_to_world_isodate(&t, offs, buf1, sizeof(buf1));
	L4SC_DEBUG(logger, "%s: %s -> %s", __FUNCTION__, s, buf1);
	assert (rc == 24);
	assert (buf1[rc-5] == ((offs >= 0)? '+': '-'));

	rc = bfc_init_datetime_from_isotime(&t0, sizeof(t0), buf1, rc);
	assert (rc >= 0);

	rc = bfc_datetime_to_isodate(&t0, buf2, sizeof(buf2));
	assert (rc == 24);
	assert (strncmp(buf2, s, strlen(buf)) == 0);
}

void
test(const char *s)
{
	test1(s);
	test2(s,   1);
	test2(s, 530);
	test2(s,  -8);
}

int
main (int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_DATETIME_LOGGER);

	test("2014-12-25T16:26:58Z");
	test("2014-12-25T16:47:17Z");

	return (0);
}
