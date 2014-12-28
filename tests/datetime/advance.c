
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
test(const char *s, signed long d, int unit, const char *expected)
{
	bfc_datetime_t t0, t1;
	int rc;
	size_t bufsize = strlen(expected)+1;
	char *buf;

	rc = bfc_init_datetime_from_isotime(&t0, sizeof(t0), s, strlen(s));
	assert (rc >= 0);

	rc = bfc_init_datetime_copy(&t1, sizeof(t1), &t0);
	assert (rc >= 0);

	switch (unit) {
	case 'n':
		rc = bfc_datetime_advance_nsecs(&t1, d);
		break;
	case 'u':
		rc = bfc_datetime_advance_usecs(&t1, d);
		break;
	case 'm':
		rc = bfc_datetime_advance_msecs(&t1, d);
		break;
	default:
		rc = bfc_datetime_advance_secs(&t1, d);
	}
	assert (rc >= 0);

	buf = alloca(bufsize);
	bfc_datetime_to_isodate(&t1, buf, bufsize);
	L4SC_DEBUG(logger, "%s: %s+%ld%c -> %s", __FUNCTION__, s, d, unit, buf);
	assert (strcmp(buf, expected) == 0);
}

int
main (int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_DATETIME_LOGGER);

	test("2014-12-26T12:00:00Z", 1, 's', "2014-12-26T12:00:01.000Z");
	test("2014-12-26T12:00:00Z", 2, 's', "2014-12-26T12:00:02.000Z");
	test("2014-12-26T12:00:00Z", 5, 's', "2014-12-26T12:00:05.000Z");
	test("2014-12-26T12:00:00Z", 3600, 's', "2014-12-26T13:00:00.000Z");
	test("2014-12-26T12:00:00Z", 12*3600L-1,'s', "2014-12-26T23:59:59Z");
	test("2014-12-26T12:00:00Z", 12*3600L+0,'s', "2014-12-27T00:00:00Z");
	test("2014-12-26T12:00:00Z", 12*3600L+1,'s', "2014-12-27T00:00:01Z");
	test("2014-12-26T12:00:00Z", 36*3600L-1,'s', "2014-12-27T23:59:59Z");
	test("2014-12-26T12:00:00Z", 36*3600L+0,'s', "2014-12-28T00:00:00Z");
	test("2014-12-26T12:00:00Z", 36*3600L+1,'s', "2014-12-28T00:00:01Z");

	test("2014-12-26T12:00:00Z", -1, 's', "2014-12-26T11:59:59.000Z");
	test("2014-12-26T12:00:00Z", -2, 's', "2014-12-26T11:59:58.000Z");
	test("2014-12-26T12:00:00Z", -5, 's', "2014-12-26T11:59:55.000Z");
	test("2014-12-26T12:00:00Z", -3600, 's', "2014-12-26T11:00:00.000Z");
	test("2014-12-26T12:00:00Z", -12*3600L-1,'s', "2014-12-25T23:59:59Z");
	test("2014-12-26T12:00:00Z", -12*3600L+0,'s', "2014-12-26T00:00:00Z");
	test("2014-12-26T12:00:00Z", -12*3600L+1,'s', "2014-12-26T00:00:01Z");
	test("2014-12-26T12:00:00Z", -36*3600L-1,'s', "2014-12-24T23:59:59Z");
	test("2014-12-26T12:00:00Z", -36*3600L+0,'s', "2014-12-25T00:00:00Z");
	test("2014-12-26T12:00:00Z", -36*3600L+1,'s', "2014-12-25T00:00:01Z");

	test("2014-12-26T12:00:00Z", 1, 'm', "2014-12-26T12:00:00.001Z");
	test("2014-12-26T12:00:00Z", 2, 'm', "2014-12-26T12:00:00.002Z");
	test("2014-12-26T12:00:00Z", 5, 'm', "2014-12-26T12:00:00.005Z");
	test("2014-12-26T12:00:00Z", 999, 'm', "2014-12-26T12:00:00.999Z");
	test("2014-12-26T12:00:00Z", 1001, 'm', "2014-12-26T12:00:01.001Z");
	test("2014-12-26T12:00:00Z", 5001, 'm', "2014-12-26T12:00:05.001Z");
	test("2014-12-26T12:00:00Z", 12*3600000L-5,'m',"2014-12-26T23:59:59Z");
	test("2014-12-26T12:00:00Z", 12*3600000L+5,'m',"2014-12-27T00:00:00Z");
	test("2014-12-26T12:00:00Z", 36*3600000L-9,'m',"2014-12-27T23:59:59Z");
	test("2014-12-26T12:00:00Z", 36*3600000L+9,'m',"2014-12-28T00:00:00Z");

	test("2014-12-26T12:00:00Z", -1, 'm', "2014-12-26T11:59:59.999Z");
	test("2014-12-26T12:00:00Z", -2, 'm', "2014-12-26T11:59:59.998Z");
	test("2014-12-26T12:00:00Z", -5, 'm', "2014-12-26T11:59:59.995Z");
	test("2014-12-26T12:00:00Z", -999, 'm', "2014-12-26T11:59:59.001Z");
	test("2014-12-26T12:00:00Z", -1001, 'm', "2014-12-26T11:59:58.999Z");
	test("2014-12-26T12:00:00Z", -5001, 'm', "2014-12-26T11:59:54.999Z");
	test("2014-12-26T12:00:00Z",-12*3600000L-5,'m',"2014-12-25T23:59:59Z");
	test("2014-12-26T12:00:00Z",-12*3600000L+5,'m',"2014-12-26T00:00:00Z");
	test("2014-12-26T12:00:00Z",-36*3600000L-9,'m',"2014-12-24T23:59:59Z");
	test("2014-12-26T12:00:00Z",-36*3600000L+9,'m',"2014-12-25T00:00:00Z");

	test("2014-12-26T12:00:00Z", 1, 'u', "2014-12-26T12:00:00.000001Z");
	test("2014-12-26T12:00:00Z", 2, 'u', "2014-12-26T12:00:00.000002Z");
	test("2014-12-26T12:00:00Z", 5, 'u', "2014-12-26T12:00:00.000005Z");
	test("2014-12-26T12:00:00Z", 999000L, 'u', "2014-12-26T12:00:00.999Z");
	test("2014-12-26T12:00:00Z", 1001000L,'u', "2014-12-26T12:00:01.001Z");
	test("2014-12-26T12:00:00Z", 5001000L,'u', "2014-12-26T12:00:05.001Z");

	test("2014-12-26T12:00:00Z", -1, 'u', "2014-12-26T11:59:59.999999Z");
	test("2014-12-26T12:00:00Z", -2, 'u', "2014-12-26T11:59:59.999998Z");
	test("2014-12-26T12:00:00Z", -5, 'u', "2014-12-26T11:59:59.999995Z");
	test("2014-12-26T12:00:00Z", -999000L, 'u',"2014-12-26T11:59:59.001Z");
	test("2014-12-26T12:00:00Z", -1001000L,'u',"2014-12-26T11:59:58.999Z");
	test("2014-12-26T12:00:00Z", -5001000L,'u',"2014-12-26T11:59:54.999Z");

	test("2014-12-26T12:00:00Z", 1000, 'n', "2014-12-26T12:00:00.000001Z");
	test("2014-12-26T12:00:00Z", 2000, 'n', "2014-12-26T12:00:00.000002Z");
	test("2014-12-26T12:00:00Z", 5000, 'n', "2014-12-26T12:00:00.000005Z");
	test("2014-12-26T12:00:00Z", 1330000000L, 'n', "2014-12-26T12:00:01Z");

	test("2014-12-26T12:00:00Z", -1000,'n', "2014-12-26T11:59:59.999999Z");
	test("2014-12-26T12:00:00Z", -2000,'n', "2014-12-26T11:59:59.999998Z");
	test("2014-12-26T12:00:00Z", -5000,'n', "2014-12-26T11:59:59.999995Z");
	test("2014-12-26T12:00:00Z", -1990000000L,'n', "2014-12-26T11:59:58Z");

	return (0);
}