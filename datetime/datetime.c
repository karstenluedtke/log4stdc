
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <inttypes.h>

#if defined(_MSC_VER) || defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#define L4SC_USE_WINDOWS_LOCALTIME 1
#endif

#if defined(L4SC_USE_WINDOWS_LOCALTIME)
#include <windows.h>
#if defined(HAVE__LOCALTIME64_S)
errno_t _gmtime64_s(struct tm *, const __time64_t *);
errno_t _localtime64_s(struct tm *, const __time64_t *);
#else
struct tm *_gmtime64(const __time64_t *);
struct tm *_localtime64(const __time64_t *);
#endif
#endif

#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#if defined(_MSC_VER)
#define snprintf _snprintf
#endif

#include "barefootc/object.h"
#include "barefootc/datetime.h"
#include "log4stdc.h"
#include "umul32_hiword.h"

static int init_datetime(void *buf, size_t bufsize, struct mempool *pool);
static int datetime_equals(bfc_cdateptr_t date, bfc_cdateptr_t other);
static unsigned bfc_datetime_hashcode(bfc_cdateptr_t date);
static void dump_datetime(bfc_cdateptr_t date,int depth,struct l4sc_logger*log);
static int datetime_tostring(bfc_cdateptr_t date, char *buf, size_t bufsize);

static int to_gmtime(bfc_cdateptr_t date, struct tm *tm);
static int to_localtime(bfc_cdateptr_t date, struct tm *tm);
static int to_worldtime(bfc_cdateptr_t date, int offs, struct tm *tm);
static int to_isodate(bfc_cdateptr_t date, char *buf, size_t bufsize);
static int to_local_isodate(bfc_cdateptr_t date, char *buf, size_t bufsize);
static int to_world_isodate(bfc_cdateptr_t date, int offs,
					char *buf, size_t bufsize);

static void last_method(void) { }

struct bfc_datetime_class bfc_datetime_class = {
	/* intentionally not using selective initialization for base class: */
	/* I want the compiler to complain if something is missing.         */
	/* .super 	*/ NULL,
	/* .name 	*/ "forward datetime",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ init_datetime,
	/* .destroy 	*/ bfc_destroy_datetime,
	/* .clone 	*/ bfc_clone_datetime,
	/* .clonesize 	*/ bfc_datetime_objsize,
	/* .hashcode 	*/ bfc_datetime_hashcode,
	/* .equals 	*/ datetime_equals,
	/* .length 	*/ bfc_datetime_length,
	/* .tostring 	*/ datetime_tostring,
	/* .dump 	*/ dump_datetime,
	/* Element access */
	/* .first	*/ NULL,
	/* .index	*/ NULL,
	/* .getl	*/ bfc_datetime_get_long,
	/* .setl	*/ bfc_datetime_set_long,
	/* .spare17 	*/ NULL,
	/* .ibegin	*/ NULL,
	/* .ilimit	*/ NULL,
	/* .rbegin	*/ NULL,
	/* .rlimit	*/ NULL,
	/* .max_size	*/ NULL,
	/* .element_size*/ NULL,
	/* .capacity	*/ NULL,
	/* .reserve	*/ NULL,
	/* .spare26 	*/ NULL,
	/* .spare27 	*/ NULL,
	/* comparision */
	/* .secs_between	*/ bfc_datetime_secs_between,
	/* .msecs_between	*/ bfc_datetime_msecs_between,
	/* .usecs_between	*/ bfc_datetime_usecs_between,
	/* .nsecs_between	*/ bfc_datetime_nsecs_between,
	/* conversion and formatting */
	/* .get_secs		*/ bfc_datetime_secs,
	/* .get_msecs		*/ bfc_datetime_msecs,
	/* .get_usecs		*/ bfc_datetime_usecs,
	/* .get_nsecs		*/ bfc_datetime_nsecs,
	/* .to_gmtime		*/ to_gmtime,
	/* .to_localtime	*/ to_localtime,
	/* .to_worldtime	*/ to_worldtime,
	/* .to_isodate		*/ to_isodate,
	/* .to_local_isodate	*/ to_local_isodate,
	/* .to_world_isodate	*/ to_world_isodate,
	/* .format		*/ NULL,
	/* modifiers */	
	/* .advance_secs	*/ bfc_datetime_advance_secs,
	/* .advance_msecs	*/ bfc_datetime_advance_msecs,
	/* .advance_usecs	*/ bfc_datetime_advance_usecs,
	/* .advance_nsecs	*/ bfc_datetime_advance_nsecs,
	/* .last_method	*/ last_method
};

static int
init_datetime(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_dateptr_t date = (bfc_dateptr_t) buf;
	if (bufsize < sizeof(*date)) {
		return (-ENOSPC);
	}
	memset(date, 0, sizeof(*date));
	date->vptr = &bfc_datetime_class;
	return (BFC_SUCCESS);
}

int
bfc_init_datetime(void *buf, size_t bufsize)
{
	bfc_dateptr_t date = (bfc_dateptr_t) buf;
	if (bufsize < sizeof(*date)) {
		return (-ENOSPC);
	}
	memset(date, 0, sizeof(*date));
	date->vptr = &bfc_datetime_class;
	return (BFC_SUCCESS);
}

int
bfc_init_datetime_copy(void *buf, size_t bufsize, const void *src)
{
	bfc_dateptr_t date = (bfc_dateptr_t) buf;
	if (bufsize < sizeof(*date)) {
		return (-ENOSPC);
	}
	memcpy(date, src, sizeof(*date));
	date->vptr = &bfc_datetime_class;
	return (BFC_SUCCESS);
}

#define SECONDS_PER_DAY		((uint32_t) 24 * 3600)

int
bfc_init_datetime_from_time_t(void *buf, size_t bufsize, time_t secs)
{
	bfc_dateptr_t date = (bfc_dateptr_t) buf;
	int rc;
	
	if ((rc = bfc_init_datetime(buf, bufsize)) < 0) {
		return (rc);
	}
	if (secs > 0) {
		date->day  = secs / SECONDS_PER_DAY;
		date->secs = secs - ((time_t) date->day * SECONDS_PER_DAY);
	} else if (secs < 0) {
		date->day  = (secs - SECONDS_PER_DAY + 1) / SECONDS_PER_DAY;
		date->secs = secs - ((time_t) date->day * SECONDS_PER_DAY);
	}
	return (rc);
}

int
bfc_init_datetime_precise(void *buf, size_t bufsize,
			  time_t secs, unsigned long nsecs)
{
	bfc_dateptr_t date = (bfc_dateptr_t) buf;
	int rc;

	if ((rc = bfc_init_datetime_from_time_t(buf, bufsize, secs)) >= 0) {
		/*
		 * frac =   2**32 * nsecs / 10**9
		 *      =   nsecs * (2**32 / 10**9) 
		 *      =  (nsecs * (2**61 / 10**9)) >> 29
		 *     ~= ((nsecs * 2305843010) >> 32) << 3
		 */
		date->frac = (umul32_hiword(nsecs, 2305843010uL) << 3);
	}
	return (rc);
}

#if defined(HAVE_STRUCT_TIMESPEC)
int
bfc_init_datetime_from_timespec(void *buf, size_t bufsize,
				const struct timespec *ts)
{
	bfc_dateptr_t date = (bfc_dateptr_t) buf;
	int rc;

	if ((rc = bfc_init_datetime_from_time_t(buf,bufsize,ts->tv_sec)) >= 0) {
		/*
		 * frac =   2**32 * nsecs / 10**9
		 *      =   nsecs * (2**32 / 10**9) 
		 *      =  (nsecs * (2**61 / 10**9)) >> 29
		 *     ~= ((nsecs * 2305843010) >> 32) << 3
		 */
		date->frac = (umul32_hiword(ts->tv_nsec, 2305843010uL) << 3);
	}
	return (rc);
}
#endif /* HAVE_STRUCT_TIMESPEC */

int
bfc_init_datetime_from_timeval(void *buf, size_t bufsize,
				const struct timeval *tv)
{
	bfc_dateptr_t date = (bfc_dateptr_t) buf;
	int rc;

	if ((rc = bfc_init_datetime_from_time_t(buf,bufsize,tv->tv_sec)) >= 0) {
		/*
		 * frac =   2**32 * usecs / 10**6
		 *      =   usecs * (2**32 / 10**6) 
		 *      =  (usecs * (2**51 / 10**6)) >> 19
		 *     ~= ((usecs * 2251799814) >> 32) << 13
		 */
		date->frac = (umul32_hiword(tv->tv_usec, 2251799814uL) << 13);
	}
	return (rc);
}

void
bfc_destroy_datetime(bfc_dateptr_t date)
{
	date->vptr = NULL;
}

int
bfc_clone_datetime(bfc_cdateptr_t date, void *buf, size_t bufsize)
{
	size_t size = bfc_object_size(date);
	if (bufsize < size) {
		return (-ENOSPC);
	}
	memcpy(buf, date, size);
	return (BFC_SUCCESS);
}

size_t
bfc_datetime_objsize(bfc_cdateptr_t date)
{
	return (sizeof(struct bfc_datetime));
}

static unsigned  
bfc_datetime_hashcode(bfc_cdateptr_t date)
{
	uint32_t x;
	x  = date->day;
	x ^= date->secs;
	x ^= (date->frac << 10) ^ (date->frac >> 22);
	return ((unsigned) x);
}

static int
datetime_equals(bfc_cdateptr_t date, bfc_cdateptr_t other)
{
	if (date == other) {
		return (1);
	}
	return ((date->day == other->day)
	     && (date->secs == other->secs)
	     && (date->frac == other->frac));
}

size_t
bfc_datetime_length(bfc_cdateptr_t date)
{
	return (1);
}

static int
datetime_tostring(bfc_cdateptr_t date, char *buf, size_t bufsize)
{
	if (date && BFC_CLASS(date)) {
		snprintf(buf, bufsize, "%s @%p: %ld %ld.%lx",
			 BFC_CLASS(date)->name, date, (long) date->day,
			 (long) date->secs, (unsigned long) date->frac);
	}
	return (0);
}

static void
dump_datetime(bfc_cdateptr_t date, int depth, struct l4sc_logger *log)
{
	if (date && BFC_CLASS(date)) {
		L4SC_DEBUG(log, "%s @%p: %ld %ld.%lx",
			 BFC_CLASS(date)->name, date, (long) date->day,
			 (long) date->secs, (unsigned long) date->frac);
	}
}

int
bfc_datetime_set_long(bfc_dateptr_t date, size_t pos, long val)
{
	if (pos != 0) {
		switch (pos) {
		case 1: date->day  = (int32_t)  val;
			return (BFC_SUCCESS);
		case 2: date->secs = (uint32_t) val;
			return (BFC_SUCCESS);
		case 3: date->frac = (uint32_t) val;
			return (BFC_SUCCESS);
		default: ;
		}
	}
	if (val > 0) {
		date->day  = val / SECONDS_PER_DAY;
		date->secs = val - ((time_t) date->day * SECONDS_PER_DAY);
		date->frac = 0;
	} else if (val < 0) {
		date->day  = (val - SECONDS_PER_DAY + 1) / SECONDS_PER_DAY;
		date->secs = val - ((time_t) date->day * SECONDS_PER_DAY);
		date->frac = 0;
	} else {
		date->day  = 0;
		date->secs = 0;
		date->frac = 0;
	}
	return (BFC_SUCCESS);
}

long
bfc_datetime_get_long(bfc_cdateptr_t date, size_t pos)
{
	if (pos != 0) {
		switch (pos) {
		case 1: return ((long) date->day);
		case 2: return ((long) date->secs);
		case 3: return ((long) date->frac);
		default: ;
		}
	}
	return ((long) 24 * 3600 * date->day + date->secs);
}

time_t
bfc_datetime_secs(bfc_cdateptr_t date)
{
	return ((time_t) 24 * 3600 * date->day + date->secs);
}

int
bfc_datetime_msecs(bfc_cdateptr_t date)
{
	if (date->frac >= 4290000000uL) {
		return (999);
	}
	return ((int) umul32_hiword(date->frac + (uint32_t) 2147483uL, 1000u));
}

long
bfc_datetime_usecs(bfc_cdateptr_t date)
{
	if (date->frac >= 4294963000uL) {
		return (999999L);
	}
	return ((long) umul32_hiword(date->frac + 2147, (uint32_t) 1000000uL));
}

long
bfc_datetime_nsecs(bfc_cdateptr_t date)
{
	if (date->frac >= 4294967290uL) {
		return (999999999L);
	}
	return ((long) umul32_hiword(date->frac + 2, (uint32_t) 1000000000uL));
}

long
bfc_datetime_secs_between(bfc_cdateptr_t first, bfc_cdateptr_t last)
{
	int32_t days = last->day - first->day;
	int32_t secs = last->secs - first->secs;

	return ((days == 0)? (long)secs: (long)days * SECONDS_PER_DAY + secs);
}

long
bfc_datetime_msecs_between(bfc_cdateptr_t first, bfc_cdateptr_t last)
{
	uint32_t diff;
	long msecs = bfc_datetime_secs_between(first, last) * 1000u;

	if (last->frac > first->frac) {
		diff = last->frac - first->frac;
		msecs += umul32_hiword(diff + (uint32_t) 2147483uL, 1000u);
	} else if (first->frac > last->frac) {
		diff = first->frac - last->frac;
		msecs -= umul32_hiword(diff + (uint32_t) 2147483uL, 1000u);
	}

	return (msecs);
}

long
bfc_datetime_usecs_between(bfc_cdateptr_t first, bfc_cdateptr_t last)
{
	uint32_t diff;
	long usecs = bfc_datetime_secs_between(first, last) * 1000000uL;

	if (last->frac > first->frac) {
		diff = last->frac - first->frac;
		usecs += umul32_hiword(diff + 2147 /*round*/, 1000000uL);
	} else if (first->frac > last->frac) {
		diff = first->frac - last->frac;
		usecs -= umul32_hiword(diff + 2147 /*round*/, 1000000uL);
	}

	return (usecs);
}

long
bfc_datetime_nsecs_between(bfc_cdateptr_t first, bfc_cdateptr_t last)
{
	uint32_t diff;
	long nsecs = bfc_datetime_secs_between(first, last) * 1000000000uL;

	if (last->frac > first->frac) {
		diff = last->frac - first->frac;
		nsecs += umul32_hiword(diff + 2 /*round*/, 1000000000uL);
	} else if (first->frac > last->frac) {
		diff = first->frac - last->frac;
		nsecs -= umul32_hiword(diff + 2 /*round*/, 1000000000uL);
	}

	return (nsecs);
}

int
bfc_datetime_advance_secs(bfc_dateptr_t date, signed long secs)
{
	signed long s = date->secs + secs;
	unsigned long days;

	if (s >= 86400L) {
		days = (s < 2*86400L)? 1uL: (unsigned long) s / 86400uL;
		date->day += (uint32_t) days;
		date->secs = (uint32_t)(s - (days * 86400uL));
	} else if (s < 0) {
		days = (s >= -86400L)? 1uL: (unsigned long)(86399L-s)/86400uL;
		date->day -= (uint32_t) days;
		date->secs = (uint32_t)(s + (days * 86400uL));
	} else {
		date->secs = (uint32_t) s;
	}

	return (BFC_SUCCESS);
}

int
bfc_datetime_advance_msecs(bfc_dateptr_t date, signed long msecs)
{
	int64_t sum  = (int64_t) 4294967L * msecs + date->frac;
	int32_t secs = (int32_t) (sum >> 32);

	if (secs == 0) {
		date->frac = (uint32_t) sum;
	} else {
		bfc_datetime_advance_secs(date, secs);
		date->frac = (uint32_t) sum;
	}

	return (BFC_SUCCESS);
}

int
bfc_datetime_advance_usecs(bfc_dateptr_t date, signed long usecs)
{
	int64_t sum  = (int64_t) 4295 * usecs + date->frac;
	int32_t secs = (int32_t) (sum >> 32);

	if (secs == 0) {
		date->frac = (uint32_t) sum;
	} else {
		bfc_datetime_advance_secs(date, secs);
		date->frac = (uint32_t) sum;
	}

	return (BFC_SUCCESS);
}

int
bfc_datetime_advance_nsecs(bfc_dateptr_t date, signed long nsecs)
{
	int64_t sum  = (int64_t) 4 * nsecs + date->frac;
	int32_t secs = (int32_t) (sum >> 32);

	if (secs == 0) {
		date->frac = (uint32_t) sum;
	} else {
		bfc_datetime_advance_secs(date, secs);
		date->frac = (uint32_t) sum;
	}

	return (BFC_SUCCESS);
}


#define POSIX_SECS_PER_DAY	86400L

static int
to_localtime(bfc_cdateptr_t date, struct tm *tm)
{
	int rc = BFC_SUCCESS;
	int64_t secs = (int64_t) POSIX_SECS_PER_DAY * date->day + date->secs;
#if defined(L4SC_USE_WINDOWS_LOCALTIME)
	__time64_t t = (__time64_t) secs;
#if defined(HAVE__LOCALTIME64_S)
	_localtime64_s(tm, &t);
#else
	const struct tm *tm2 = _localtime64(&t);
	if (tm2 != NULL) {
		*tm = *tm2;
	} else {
		memset(tm, 0, sizeof(*tm));
		rc = -EINVAL;
	}
#endif
#else /* not L4SC_USE_WINDOWS_LOCALTIME */
	time_t t = (time_t) secs;
#if defined(HAVE_LOCALTIME_R)
	localtime_r(&t, tm);
#else
	const struct tm *tm2 = localtime(&t);
	if (tm2 != NULL) {
		*tm = *tm2;
	} else {
		memset(tm, 0, sizeof(*tm));
		rc = -EINVAL;
	}
#endif
#endif
	return (rc);
}

static int
to_gmtime(bfc_cdateptr_t date, struct tm *tm)
{
	int rc = BFC_SUCCESS;
	int64_t secs = (int64_t) POSIX_SECS_PER_DAY * date->day + date->secs;
#if defined(L4SC_USE_WINDOWS_LOCALTIME)
	__time64_t t = (__time64_t) secs;
#if defined(HAVE__LOCALTIME64_S)
	_gmtime64_s(tm, &t);
#else
	const struct tm *tm2 = _gmtime64(&t);
	if (tm2 != NULL) {
		*tm = *tm2;
	} else {
		memset(tm, 0, sizeof(*tm));
		rc = -EINVAL;
	}
#endif
#else /* not L4SC_USE_WINDOWS_LOCALTIME */
	time_t t = (time_t) secs;
#if defined(HAVE_LOCALTIME_R)
	gmtime_r(&t, tm);
#else
	const struct tm *tm2 = gmtime(&t);
	if (tm2 != NULL) {
		*tm = *tm2;
	} else {
		memset(tm, 0, sizeof(*tm));
		rc = -EINVAL;
	}
#endif
#endif
	return (rc);
}

static int32_t
offset_to_seconds(int offs)
{
	if ((-12 <= offs) && (offs <= 12)) {
		return ((int32_t) 3600 * offs);
	}
	if ((-1200 <= offs) && (offs <= 1200)) {
		if (offs >= 0) {
			unsigned hrs = (unsigned) offs / 100;
			unsigned min = (unsigned) offs % 100;
			return ((int32_t) 3600 * hrs + 60 * min);
		} else {
			unsigned hrs = ((unsigned) -offs) / 100;
			unsigned min = ((unsigned) -offs) % 100;
			return -((int32_t) 3600 * hrs + 60 * min);
		}
	}
	return ((int32_t) offs);
}

static int
to_worldtime(bfc_cdateptr_t date, int offs, struct tm *tm)
{
	int rc = BFC_SUCCESS;
	int64_t secs = (int64_t) POSIX_SECS_PER_DAY * date->day
				+ date->secs + offset_to_seconds(offs);
#if defined(L4SC_USE_WINDOWS_LOCALTIME)
	__time64_t t = (__time64_t) secs;
#if defined(HAVE__LOCALTIME64_S)
	_gmtime64_s(tm, &t);
#else
	const struct tm *tm2 = _gmtime64(&t);
	if (tm2 != NULL) {
		*tm = *tm2;
	} else {
		memset(tm, 0, sizeof(*tm));
		rc = -EINVAL;
	}
#endif
#else /* not L4SC_USE_WINDOWS_LOCALTIME */
	time_t t = (time_t) secs;
#if defined(HAVE_LOCALTIME_R)
	gmtime_r(&t, tm);
#else
	const struct tm *tm2 = gmtime(&t);
	if (tm2 != NULL) {
		*tm = *tm2;
	} else {
		memset(tm, 0, sizeof(*tm));
		rc = -EINVAL;
	}
#endif
#endif
	return (rc);
}

static int
to_isodate(bfc_cdateptr_t date, char *buf, size_t bufsize)
{
	int rc, len;
	struct tm tm;

	if ((rc = to_gmtime(date, &tm)) < 0) {
		return(rc);
	}
	
	len = strftime(buf, bufsize, "%Y-%m-%dT%H:%M:%SZ", &tm);
	if (len < 16) {
		return (len);
	} else if (len+7 < bufsize) {
		long usecs = bfc_datetime_usecs(date);
		if ((unsigned long) usecs < 1000000uL) {
			len -= 1; /* 'Z' */
			len += snprintf(buf+len, bufsize-len, ".%06ldZ", usecs);
		}
	} else if (len+4 < bufsize) {
		int msecs = bfc_datetime_msecs(date);
		if ((unsigned) msecs < 1000) {
			len -= 1; /* 'Z' */
			len += snprintf(buf+len, bufsize-len, ".%03dZ", msecs);
		}
	}
	return (len);
}

static int
to_world_isodate(bfc_cdateptr_t date, int offs, char *buf, size_t bufsize)
{
	int rc, len;
	struct tm tm;

	if ((rc = to_worldtime(date, offs, &tm)) < 0) {
		return(rc);
	}

	len = strftime(buf, bufsize, "%Y-%m-%dT%H:%M:%S", &tm);
	if (len < 16) {
		return (len);
	} else if (len+12 < bufsize) {
		long usecs = bfc_datetime_usecs(date);
		if ((unsigned long) usecs < 1000000uL) {
			len += snprintf(buf+len, bufsize-len, ".%06ld", usecs);
		}
	} else if (len+9 < bufsize) {
		int msecs = bfc_datetime_msecs(date);
		if ((unsigned) msecs < 1000) {
			len += snprintf(buf+len, bufsize-len, ".%03d", msecs);
		}
	}

	if (len+5 < bufsize) {
		char sign = (offs >= 0)? '+': '-';
		unsigned o = (offs >= 0)? offs: -offs;
		static const char fmt[] = "%c%02u:%02u";
		if (o <= 12) {
			len += snprintf(buf+len, bufsize-len, fmt, sign, o, 0);
		} else if (o <= 1200) {
			len += snprintf(buf+len, bufsize-len,
					fmt, sign, o/100, o % 100);
		} else {
			len += snprintf(buf+len, bufsize-len,
					fmt, sign, o/3600, (o/60) % 60);
		}
	}

	return (len);
}

static int
to_local_isodate(bfc_cdateptr_t date, char *buf, size_t bufsize)
{
	int rc, offs, len;
	struct tm tm, utc;

	if ((rc = to_localtime(date, &tm)) < 0) {
		return(rc);
	}
	if ((rc = to_gmtime(date, &utc)) < 0) {
		return(rc);
	}
	if (tm.tm_yday == utc.tm_yday) {
		offs = 60 * ((int) tm.tm_hour - utc.tm_hour);
	} else if ((tm.tm_year > utc.tm_year)
		|| ((tm.tm_year==utc.tm_year) && (tm.tm_yday > utc.tm_yday))) {
		offs = 60 * ((int) tm.tm_hour + 24 - utc.tm_hour);
	} else {
		offs = 60 * ((int) tm.tm_hour - utc.tm_hour - 24);
	}
	offs += tm.tm_min - utc.tm_min;

	len = strftime(buf, bufsize, "%Y-%m-%dT%H:%M:%S", &tm);
	if (len < 16) {
		return (len);
	} else if (len+12 < bufsize) {
		long usecs = bfc_datetime_usecs(date);
		if ((unsigned long) usecs < 1000000uL) {
			len += snprintf(buf+len, bufsize-len, ".%06ld", usecs);
		}
	} else if (len+9 < bufsize) {
		int msecs = bfc_datetime_msecs(date);
		if ((unsigned) msecs < 1000) {
			len += snprintf(buf+len, bufsize-len, ".%03d", msecs);
		}
	}

	if (len+5 < bufsize) {
		char sign = (offs >= 0)? '+': '-';
		unsigned o = (offs >= 0)? offs: -offs;
		static const char fmt[] = "%c%02u:%02u";
		len += snprintf(buf+len, bufsize-len, fmt, sign, o/60, o % 60);
	}

	return (len);
}
