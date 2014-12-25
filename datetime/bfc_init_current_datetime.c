/**
 * @file bfc_init_current_datetime.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/datetime.h"

#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#define SECONDS_PER_DAY		((uint32_t) 24 * 3600)

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include "windows.h"
#else
#include <time.h>
#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#endif
#endif

/**
 * @brief    bfc_init_current_datetime
 */
int
bfc_init_current_datetime(void *buf, size_t bufsize)
{
	int rc = -ENOSYS;

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
	if ((rc = bfc_init_datetime(buf, bufsize)) >= 0) {
		bfc_dateptr_t date = (bfc_dateptr_t) buf;
		UINT64 ftim, secs;
		unsigned long frac;
		FILETIME ft;

		GetSystemTimeAsFileTime(&ft);
		ftim = ft.dwHighDateTime;
		ftim <<= 32;
		ftim |= ft.dwLowDateTime;
		secs  = ftim / 10000000u;
		frac  = (unsigned long) (ftim - (10000000u * secs));
		secs -= (UINT64)134774 * SECONDS_PER_DAY; /* Jan 1st, 1970 */

		date->day  = secs / SECONDS_PER_DAY;
		date->secs = secs - ((UINT64) date->day * SECONDS_PER_DAY);
		date->frac = bfc_datetime_frac_from_decimal(frac, 7);
	}

#elif defined(HAVE_CLOCK_GETTIME)
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
		return (bfc_init_datetime_from_timespec(buf, bufsize, &ts));
	} else if (errno != 0) {
		rc = -errno;
	}

#else
	struct timeval tv;
	if (gettimeofday(&tv, NULL) == 0) {
		return (bfc_init_datetime_from_timeval(buf, bufsize, &tv));
	} else if (errno != 0) {
		rc = -errno;
	}
#endif

	return (rc);
}
