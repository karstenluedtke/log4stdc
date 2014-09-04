/**
 * @file bfc_datetime_days_to_date.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/datetime.h"


long
bfc_datetime_days_to_date(unsigned year, unsigned month, unsigned day)
{
	long days;
	int feb29 = 0;

	days = 365 * (year-1970) + (year-1)/4 - 1970/4; /* 1901 - 2100 */

	if (year > 2100) {
		/* no Feb 29 every 100 years, at the beginning of a century */
		days -= (year-1)/100 - 1970/100;
		if (year > 2400) {
			/* but again a Feb 29 every 400 years, e.g. 2000 */
			days += (year-1)/400 - 1970/400;
		} else {
			days += 1; /* adjust for the extra day in 2000 */
		}
	} else if (year < 1901) {
		/* no Feb 29 every 100 years, at the beginning of a century */
		days -= (year-1)/100 - 1970/100;
		if (year < 1601) {
			/* but again a Feb 29 every 400 years */
			days += (year-1)/400 - 1970/400;
		}
	}

	if ((year > 1900) && (year < 2100)) {
		feb29 = (((year) & 3) == 0);
	} else {
		feb29 = ((((year) & 3) == 0)
		      && (((year) % 100 != 0) || ((year) % 400 == 0)));
	}

	switch (month) {
	case 12: days += 30; /* and fall */
	case 11: days += 31; /* and fall */
	case 10: days += 30; /* and fall */
	case  9: days += 31; /* and fall */
	case  8: days += 31; /* and fall */
	case  7: days += 30; /* and fall */
	case  6: days += 31; /* and fall */
	case  5: days += 30; /* and fall */
	case  4: days += 31; /* and fall */
	case  3: days += feb29? 29: 28;
	case  2: days += 31; /* and fall */
	default: ;
	}
	days += day - 1;

	return (days);
}

