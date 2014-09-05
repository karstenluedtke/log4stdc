
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "barefootc/datetime.h"
#include "log4stdc.h"

static l4sc_logger_ptr_t logger;

#define Y2000 ( 30*365 +  7) /* 72, 76, 80, 84, 88, 92, 96 */
#define Y2014 ( 44*365 + 11) /* 72, 76, 80, 84, 88, 92, 96, 2000, 04, 08, 12 */
#define Y2100 (Y2000 + (100*365 + 25))
#define Y2200 (Y2100 + (100*365 + 24))
#define Y2300 (Y2200 + (100*365 + 24))
#define Y2400 (Y2300 + (100*365 + 24))
#define Y2500 (Y2400 + (100*365 + 25))

void
t1(const char *s, long days, unsigned long secs)
{
	int rc;
	bfc_datetime_t t;

	rc = bfc_init_datetime_from_isotime(&t, sizeof(t), s, strlen(s));

	L4SC_DEBUG(logger, "%s: %s -> %ldd, %lus + %lu/2**32",
		__FUNCTION__, s, (long) t.day,
		(unsigned long) t.secs, (unsigned long) t.frac);
	assert (rc >= 0);
	assert (t.day  == days);
	assert (t.secs == secs);
	assert (t.frac == 0);
}

void
t2(const char *s, long days, unsigned long secs, unsigned long frac)
{
	int rc;
	bfc_datetime_t t;
	unsigned long tolerance = 30000; /* 7 ppm. */

	rc = bfc_init_datetime_from_isotime(&t, sizeof(t), s, strlen(s));

	L4SC_DEBUG(logger, "%s: %s -> %ldd, %lus + %lu/2**32",
		__FUNCTION__, s, (long) t.day,
		(unsigned long) t.secs, (unsigned long) t.frac);
	assert (rc >= 0);
	assert (t.day  == days);
	assert (t.secs == secs);
	assert ((t.frac >= frac - tolerance) && (t.frac <= frac + tolerance));
}

void
t3(const char *s, int long days, unsigned long secs, unsigned long frac)
{
	int rc;
	bfc_datetime_t t;
	unsigned long tolerance = 30000; /* 7 ppm. */

	rc = bfc_init_datetime_from_isotime(&t, sizeof(t), s, strlen(s));

	L4SC_DEBUG(logger, "%s: %s -> %ldd, %lus + %lu/2**32",
		__FUNCTION__, s, (long) t.day,
		(unsigned long) t.secs, (unsigned long) t.frac);
	assert (rc >= 0);
	assert (t.day  == days);
	assert (t.secs == secs);
	assert (t.frac >= frac - tolerance);
}

int
main (int argc, char *argv)
{
	int i, j;
	char buf[100];

	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger("barefootc.datetime", 0);

	t1("1970-03-01T13:00Z",	    31+28,		13*3600);
	t1("1972-03-01T13:00:14Z",  365+365+31+29,	13*3600 + 14);
	t1("1973-04-01T08:17:57Z",  2*365+366+31+28+31,	 8*3600 + 17*60 + 57);
	t1("2014-09-01T23:59:59Z",  Y2014+5*31+2*30+28,	23*3600 + 59*60 + 59);
	t1("1967-03-30T01:01:51Z",  -3*365-1+31+28+29,	 1*3600 +  1*60 + 51);
	t1("2100-02-01T14:23:18Z",  Y2100+31,		14*3600 + 23*60 + 18);
	t1("2100-04-01T14:23:18Z",  Y2100+31+28+31,	14*3600 + 23*60 + 18);
	t1("2100-12-31T14:23:18Z",  Y2100+364,		14*3600 + 23*60 + 18);
	t1("2200-02-01T14:23:18Z",  Y2200+31,		14*3600 + 23*60 + 18);
	t1("2200-04-01T14:23:18Z",  Y2200+31+28+31,	14*3600 + 23*60 + 18);
	t1("2200-12-31T14:23:18Z",  Y2200+364,		14*3600 + 23*60 + 18);
	t1("2300-02-01T14:23:18Z",  Y2300+31,		14*3600 + 23*60 + 18);
	t1("2300-04-01T14:23:18Z",  Y2300+31+28+31,	14*3600 + 23*60 + 18);
	t1("2300-12-31T14:23:18Z",  Y2300+364,		14*3600 + 23*60 + 18);
	t1("2300-02-01T14:23:18Z",  Y2300+31,		14*3600 + 23*60 + 18);
	t1("2300-04-01T14:23:18Z",  Y2300+31+28+31,	14*3600 + 23*60 + 18);
	t1("2300-12-31T14:23:18Z",  Y2300+364,		14*3600 + 23*60 + 18);
	t1("2400-02-01T14:23:18Z",  Y2400+31,		14*3600 + 23*60 + 18);
	t1("2400-04-01T14:23:18Z",  Y2400+31+29+31,	14*3600 + 23*60 + 18);
	t1("2400-12-31T14:23:18Z",  Y2400+365,		14*3600 + 23*60 + 18);
	t1("2500-02-01T14:23:18Z",  Y2500+31,		14*3600 + 23*60 + 18);

	t1("1970-02-01T06:00+0000",	31,		 6*3600);
	t1("1970-02-01T06:00+0100",	31,		 5*3600);
	t1("1970-02-01T06:00+02",  	31,		 4*3600);
	t1("1970-02-01T06:00+03:00",	31,		 3*3600);
	t1("1970-02-01T06:00+03:30",	31,		 2*3600 + 30*60);
	t1("1970-02-01T06:00+0530",	31,		 0*3600 + 30*60);
	t1("1970-02-01T06:00+0700",	30,		23*3600);

	t1("1970-02-01T18:00-05",	31,		23*3600);
	t1("1970-02-01T18:00-0600",	32,		 0*3600);
	t1("1970-02-01T18:00-08:00",	32,		 2*3600);

	t2("1970-01-04T12:00:00.2Z",		3,  12*3600,	 858993459uL);
	t2("1970-01-04T12:00:00.3Z",		3,  12*3600,	1288490188uL);
	t2("1970-01-04T12:00:00.5Z",		3,  12*3600,	2147483648uL);
	t2("1970-01-04T12:00:00.7Z",		3,  12*3600,	3006477107uL);
	t3("1970-01-04T12:00:00.9Z",		3,  12*3600,	3865470566uL);

	t2("1970-01-05T12:00:00.25Z",		4,  12*3600,	1073741824uL);
	t2("1970-01-05T12:00:00.33Z",		4,  12*3600,	1417339207uL);
	t2("1970-01-05T12:00:00.50Z",		4,  12*3600,	2147483648uL);
	t2("1970-01-05T12:00:00.77Z",		4,  12*3600,	3307124817uL);
	t3("1970-01-05T12:00:00.99Z",		4,  12*3600,	4252017623uL);

	t2("1970-01-06T12:00:00.250Z",		5,  12*3600,	1073741824uL);
	t2("1970-01-06T12:00:00.333Z",		5,  12*3600,	1430224109uL);
	t2("1970-01-06T12:00:00.500Z",		5,  12*3600,	2147483648uL);
	t2("1970-01-06T12:00:00.777Z",		5,  12*3600,	3337189588uL);
	t3("1970-01-06T12:00:00.999Z",		5,  12*3600,	4290672328uL);

	t2("1970-01-07T12:00:00.2500Z",		6,  12*3600,	1073741824uL);
	t2("1970-01-07T12:00:00.3333Z",		6,  12*3600,	1431512599uL);
	t2("1970-01-07T12:00:00.5000Z",		6,  12*3600,	2147483648uL);
	t2("1970-01-07T12:00:00.7777Z",		6,  12*3600,	3340196066uL);
	t3("1970-01-07T12:00:00.9999Z",		6,  12*3600,	4294537799uL);

	t2("1970-01-08T12:00:00.25000Z",	7,  12*3600,	1073741824uL);
	t2("1970-01-08T12:00:00.33333Z",	7,  12*3600,	1431641448uL);
	t2("1970-01-08T12:00:00.50000Z",	7,  12*3600,	2147483648uL);
	t2("1970-01-08T12:00:00.77777Z",	7,  12*3600,	3340496713uL);
	t3("1970-01-08T12:00:00.99999Z",	7,  12*3600,	4294924346uL);

	t2("1970-01-09T12:00:00.250000Z",	8,  12*3600,	1073741824uL);
	t2("1970-01-09T12:00:00.333333Z",	8,  12*3600,	1431654333uL);
	t2("1970-01-09T12:00:00.500000Z",	8,  12*3600,	2147483648uL);
	t2("1970-01-09T12:00:00.777777Z",	8,  12*3600,	3340526778uL);
	t3("1970-01-09T12:00:00.999999Z",	8,  12*3600,	4294963001uL);

	t2("1970-01-10T12:00:00.2500000Z",	9,  12*3600,	1073741824uL);
	t2("1970-01-10T12:00:00.3333333Z",	9,  12*3600,	1431655622uL);
	t2("1970-01-10T12:00:00.5000000Z",	9,  12*3600,	2147483648uL);
	t2("1970-01-10T12:00:00.7777777Z",	9,  12*3600,	3340529785uL);
	t3("1970-01-10T12:00:00.9999999Z",	9,  12*3600,	4294966866uL);

	t2("1970-01-11T12:00:00.25000000Z",	10, 12*3600,	1073741824uL);
	t2("1970-01-11T12:00:00.33333333Z",	10, 12*3600,	1431655651uL);
	t2("1970-01-11T12:00:00.50000000Z",	10, 12*3600,	2147483648uL);
	t2("1970-01-11T12:00:00.77777777Z",	10, 12*3600,	3340530085uL);
	t3("1970-01-11T12:00:00.99999999Z",	10, 12*3600,	4294967253uL);

	t2("1970-01-12T12:00:00.250000000Z",	11, 12*3600,	1073741824uL);
	t2("1970-01-12T12:00:00.333333333Z",	11, 12*3600,	1431655763uL);
	t2("1970-01-12T12:00:00.500000000Z",	11, 12*3600,	2147483648uL);
	t2("1970-01-12T12:00:00.777777777Z",	11, 12*3600,	3340530115uL);
	t3("1970-01-12T12:00:00.999999999Z",	11, 12*3600,	4294967291uL);

	t2("1970-01-13T12:00:00.2500000000Z",	12, 12*3600,	1073741824uL);
	t2("1970-01-13T12:00:00.3333333333Z",	12, 12*3600,	1431655763uL);

	if (sizeof(long) > 4) {
	t2("1970-01-13T12:00:00.250000000000Z",	12, 12*3600,	1073741824uL);
	t2("1970-01-13T12:00:00.333333333333Z",	12, 12*3600,	1431655763uL);
	t2("1970-01-12T12:00:00.500000000000Z",	11, 12*3600,	2147483648uL);
	t2("1970-01-12T12:00:00.777777777777Z",	11, 12*3600,	3340530115uL);
	t3("1970-01-12T12:00:00.999999999999Z",	11, 12*3600,	4294967291uL);
	}

	return (0);
}