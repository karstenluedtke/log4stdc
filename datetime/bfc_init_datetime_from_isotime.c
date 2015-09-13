/**
 * @file bfc_init_datetime_from_isotime.c
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "barefootc/datetime.h"
#include "log4stdc.h"

/**
 * @brief    bfc_init_datetime_from_isotime
 */
int
bfc_init_datetime_from_isotime(void *buf, size_t bufsize,
				const char *s, size_t len)
{
	bfc_dateptr_t date = (bfc_dateptr_t) buf;
	unsigned yr, m, d, hr, min, sec=0;
	unsigned long sub=0;
	const char *zone = NULL;
	char tee;
	int sublen=0;
	int consumed=0;
	int32_t days, secs;
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_DATETIME_LOGGER);

	if ((rc = bfc_init_datetime(buf, bufsize)) < 0) {
		L4SC_ERROR(logger, "%s: bfc_init_datetime error %d",
							__FUNCTION__, rc);
		return (rc);
	}

	if (sscanf(s, "%u-%u-%u%c%u:%u:%u.%lu%n",
			&yr,&m,&d, &tee, &hr,&min,&sec, &sub, &consumed) >= 8) {
		zone = s + consumed;
		for (sublen = 0; sublen < consumed; sublen++) {
			if (strchr("0123456789", s[consumed-sublen-1]) == NULL)
				break;
		}
	} else if (sscanf(s, "%u-%u-%u%c%u:%u:%u%n",
			&yr,&m,&d, &tee, &hr,&min,&sec, &consumed) >= 7) {
		zone = s + consumed;
	} else if (sscanf(s, "%u-%u-%u%c%u:%u%n",
			&yr,&m,&d, &tee, &hr,&min,      &consumed) >= 6) {
		zone = s + consumed;
	}

	if (zone == NULL) {
		L4SC_ERROR(logger, "%s: date not recognized", __FUNCTION__);
		return (-EINVAL);
	}

	L4SC_DEBUG(logger, "%s: found %d-%u-%u %u-%u-%u.%lu",
			__FUNCTION__, yr, m, d, hr, min, sec, sub);

	if (strchr("+-zZ", zone[0]) == NULL) {
		L4SC_ERROR(logger, "%s: invalid zone %s", __FUNCTION__, zone);
		return (-EINVAL);
	}

	if (yr < 100) {
		yr += (yr >= 70)? 1900: 2000; /* 1970 - 2069 */
	}

	days = bfc_datetime_days_to_date(yr, m, d);

	secs = (int32_t) 3600 * hr + 60 * min + sec;

	if (zone[0] == '+' /* Europe, Asia */ ) {
		hr = min = 0;
		if ((sscanf(zone+1, "%u:%u",    &hr, &min) == 2)
		 || (sscanf(zone+1, "%02u%02u", &hr, &min) >= 1)
		 || (sscanf(zone+1, "%u",       &hr)       == 1)) {
			secs -= (int32_t) 3600 * hr + 60 * min;
			if (secs < 0) {
				days -= 1;
				secs += (int32_t) 3600 * 24;
			}
		}
	} else if (zone[0] == '-' /* America */ ) {
		hr = min = 0;
		if ((sscanf(zone+1, "%u:%u",    &hr, &min) == 2)
		 || (sscanf(zone+1, "%02u%02u", &hr, &min) >= 1)
		 || (sscanf(zone+1, "%u",       &hr)       == 1)) {
			secs += (int32_t) 3600 * hr + 60 * min;
			if (secs >= (int32_t) 3600 * 24) {
				days += 1;
				secs -= (int32_t) 3600 * 24;
			}
		}
	}

	bfc_datetime_set_long(date, BFC_DATETIME_DAYS_SINCE_1970, days);
	bfc_datetime_set_long(date, BFC_DATETIME_SECOND_OF_DAY,   secs);
	bfc_datetime_set_long(date, BFC_DATETIME_FRACTION_OF_SEC,
		(sub > 0)? bfc_datetime_frac_from_decimal(sub, sublen): 0);

	L4SC_DEBUG(logger, "%s: %s -> %ldd, %lus + %lu/4294967296",
		__FUNCTION__, s,
		bfc_datetime_get_long(date, BFC_DATETIME_DAYS_SINCE_1970),
		bfc_datetime_get_long(date, BFC_DATETIME_SECOND_OF_DAY),
		bfc_datetime_get_long(date, BFC_DATETIME_FRACTION_OF_SEC));

	return (rc);
}

