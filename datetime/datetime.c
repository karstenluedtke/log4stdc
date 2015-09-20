
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

static int init_datetime(void *buf, size_t bufsize, bfc_mempool_t pool);
static int clone_datetime(bfc_cobjptr_t date,
		   void *buf, size_t bufsize, bfc_mempool_t pool);
static int datetime_equals(bfc_cobjptr_t date, bfc_cobjptr_t other);
static unsigned bfc_datetime_hashcode(bfc_cobjptr_t date, int hashlen);
static void dump_datetime(bfc_cobjptr_t date,int depth,struct l4sc_logger*log);
static int datetime_tostring(bfc_cobjptr_t date,
			     char *buf, size_t bufsize, const char *fmt);

static int to_gmtime(bfc_cobjptr_t date, struct tm *tm);
static int to_localtime(bfc_cobjptr_t date, struct tm *tm);
static int to_worldtime(bfc_cobjptr_t date, int offs, struct tm *tm);
static int to_isodate(bfc_cobjptr_t date, char *buf, size_t bufsize);
static int to_local_isodate(bfc_cobjptr_t date, char *buf, size_t bufsize);
static int to_world_isodate(bfc_cobjptr_t date, int offs,
					char *buf, size_t bufsize);

static void last_method(void) { }

const struct bfc_datetime_class bfc_datetime_class = {
	/* intentionally not using selective initialization for base class: */
	/* I want the compiler to complain if something is missing.         */
	/* .super 	*/ NULL,
	/* .name 	*/ "datetime",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ init_datetime,
	/* .initrefc 	*/ (void *) bfc_default_init_refcount,
	/* .incrrefc 	*/ (void *) bfc_default_incr_refcount,
	/* .decrrefc 	*/ (void *) bfc_default_decr_refcount,
	/* .destroy 	*/ bfc_destroy_datetime,
	/* .clone 	*/ clone_datetime,
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
	/* .spare29 	*/ NULL,
	/* .spare30 	*/ NULL,
	/* .spare31 	*/ NULL,
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
init_datetime(void *buf, size_t bufsize, bfc_mempool_t pool)
{
	bfc_objptr_t date = (bfc_objptr_t) buf;
	if (bufsize < sizeof(*date)) {
		return (-ENOSPC);
	}
	memset(date, 0, sizeof(*date));
	date->vptr = (bfc_classptr_t) &bfc_datetime_class;
	return (BFC_SUCCESS);
}

int
bfc_init_datetime(void *buf, size_t bufsize)
{
	bfc_objptr_t date = (bfc_objptr_t) buf;
	if (bufsize < sizeof(*date)) {
		return (-ENOSPC);
	}
	memset(date, 0, sizeof(*date));
	date->vptr = (bfc_classptr_t) &bfc_datetime_class;
	return (BFC_SUCCESS);
}

int
bfc_init_datetime_copy(void *buf, size_t bufsize, const void *src)
{
	bfc_objptr_t date = (bfc_objptr_t) buf;
	if (bufsize < sizeof(*date)) {
		return (-ENOSPC);
	}
	memcpy(date, src, sizeof(*date));
	date->vptr = (bfc_classptr_t) &bfc_datetime_class;
	return (BFC_SUCCESS);
}

#define SECONDS_PER_DAY		((uint32_t) 24 * 3600)

#define DAYS_SINCE_1970(obj)	(obj)->length
#define SECOND_OF_DAY(obj)	(obj)->private_6
#define FRACTION_OF_SEC(obj)	(obj)->private_7

#define GET_DAYS(obj)		((ptrdiff_t) (DAYS_SINCE_1970(obj)))
#define SET_DAYS(obj,val)	DAYS_SINCE_1970(obj) = (size_t)(ptrdiff_t)(val)
#define GET_SECS(obj)		((uint32_t) (SECOND_OF_DAY(obj)))
#define SET_SECS(obj,val)	SECOND_OF_DAY(obj) = (size_t)(uint32_t)(val)
#define GET_FRAC(obj)		((uint32_t) (FRACTION_OF_SEC(obj)))
#define SET_FRAC(obj,val)	FRACTION_OF_SEC(obj) = (size_t)(uint32_t)(val)

int
bfc_init_datetime_from_time_t(void *buf, size_t bufsize, time_t secs)
{
	bfc_objptr_t date = (bfc_objptr_t) buf;
	int rc;
	
	if ((rc = bfc_init_datetime(buf, bufsize)) < 0) {
		return (rc);
	}
	if (secs > 0) {
		size_t day = secs / SECONDS_PER_DAY;
		SET_DAYS(date, day);
		SET_SECS(date, secs - ((time_t) day * SECONDS_PER_DAY));
	} else if (secs < 0) {
		ptrdiff_t day = (secs - SECONDS_PER_DAY + 1) / SECONDS_PER_DAY;
		SET_DAYS(date, day);
		SET_SECS(date, secs - ((time_t) day * SECONDS_PER_DAY));
	}
	return (rc);
}

int
bfc_init_datetime_precise(void *buf, size_t bufsize,
			  time_t secs, unsigned long nsecs)
{
	bfc_objptr_t date = (bfc_objptr_t) buf;
	uint32_t fraction;
	int rc;

	if ((rc = bfc_init_datetime_from_time_t(buf, bufsize, secs)) >= 0) {
		/*
		 * frac =   2**32 * nsecs / 10**9
		 *      =   nsecs * (2**32 / 10**9) 
		 *      =  (nsecs * (2**61 / 10**9)) >> 29
		 *     ~= ((nsecs * 2305843010) >> 32) << 3
		 */
		fraction = (umul32_hiword(nsecs, 2305843010uL) << 3);
		SET_FRAC(date, fraction);
	}
	return (rc);
}

#if defined(HAVE_STRUCT_TIMESPEC)
int
bfc_init_datetime_from_timespec(void *buf, size_t bufsize,
				const struct timespec *ts)
{
	bfc_objptr_t date = (bfc_objptr_t) buf;
	uint32_t fraction;
	int rc;

	if ((rc = bfc_init_datetime_from_time_t(buf,bufsize,ts->tv_sec)) >= 0) {
		/*
		 * frac =   2**32 * nsecs / 10**9
		 *      =   nsecs * (2**32 / 10**9) 
		 *      =  (nsecs * (2**61 / 10**9)) >> 29
		 *     ~= ((nsecs * 2305843010) >> 32) << 3
		 */
		fraction = umul32_shr29(ts->tv_nsec, 2305843010uL);
		SET_FRAC(date, fraction);
	}
	return (rc);
}
#endif /* HAVE_STRUCT_TIMESPEC */

int
bfc_init_datetime_from_timeval(void *buf, size_t bufsize,
				const struct timeval *tv)
{
	bfc_objptr_t date = (bfc_objptr_t) buf;
	uint32_t fraction;
	int rc;

	if ((rc = bfc_init_datetime_from_time_t(buf,bufsize,tv->tv_sec)) >= 0) {
		/*
		 * frac =   2**32 * usecs / 10**6
		 *      =   usecs * (2**32 / 10**6) 
		 *      =  (usecs * (2**51 / 10**6)) >> 19
		 *     ~= ((usecs * 2251799814) >> 32) << 13
		 */
		fraction = umul32_shr19(tv->tv_usec, 2251799814uL);
		SET_FRAC(date, fraction);
	}
	return (rc);
}

void
bfc_destroy_datetime(bfc_objptr_t date)
{
	date->vptr = NULL;
}

int
bfc_clone_datetime(bfc_cobjptr_t date, void *buf, size_t bufsize)
{
	size_t size = bfc_object_size(date);
	if (bufsize < size) {
		return (-ENOSPC);
	}
	memcpy(buf, date, size);
	return (BFC_SUCCESS);
}

static int
clone_datetime(bfc_cobjptr_t date,
		void *buf, size_t bufsize, bfc_mempool_t pool)
{
	return (bfc_clone_datetime(date, buf, bufsize));
}

size_t
bfc_datetime_objsize(bfc_cobjptr_t date)
{
	return (sizeof(bfc_datetime_t));
}

static unsigned  
bfc_datetime_hashcode(bfc_cobjptr_t date, int hashlen)
{
	uint32_t x;
	x  = GET_DAYS(date);
	x ^= GET_SECS(date);
	x ^= (GET_FRAC(date) << 10) ^ (GET_FRAC(date) >> 22);
	return (bfc_reduce_hashcode(x, 8*sizeof(x), hashlen));
}

static int
datetime_equals(bfc_cobjptr_t date, bfc_cobjptr_t other)
{
	if (date == other) {
		return (1);
	}
	return ((GET_DAYS(date) == GET_DAYS(other))
	     && (GET_SECS(date) == GET_SECS(other))
	     && (GET_FRAC(date) == GET_FRAC(other)));
}

size_t
bfc_datetime_length(bfc_cobjptr_t date)
{
	return (1);
}

static int
datetime_tostring(bfc_cobjptr_t date,
		  char *buf, size_t bufsize, const char *fmt)
{
	int rc = 0;
	char tmp[32];

	if (date && BFC_CLASS(date)) {
		if (fmt && (fmt[0] == 'Z')) {
			if (buf && (bufsize > 0)
			   && ((rc = to_isodate(date, buf, bufsize)) > 0)) {
				/* ok, printed */
			} else {
				rc = to_isodate(date, tmp, sizeof(tmp));
			}
		} else {
			if (buf && (bufsize > 0)
			 && ((rc = to_local_isodate(date, buf, bufsize)) > 0)) {
				/* ok, printed */
			} else {
				rc = to_local_isodate(date, tmp, sizeof(tmp));
			}
		}
	}
	return (rc);
}

static void
dump_datetime(bfc_cobjptr_t date, int depth, struct l4sc_logger *log)
{
	if (date && BFC_CLASS(date)) {
		L4SC_DEBUG(log, "%s @%p: %ld %ld.%lx",
			 BFC_CLASS(date)->name, date, (long) GET_DAYS(date),
			 (long)GET_SECS(date), (unsigned long)GET_FRAC(date));
	}
}

int
bfc_datetime_set_long(bfc_objptr_t date, size_t pos, long val)
{
	if (pos != 0) {
		switch (pos) {
		case BFC_DATETIME_DAYS_SINCE_1970:
			SET_DAYS(date, val);
			return (BFC_SUCCESS);
		case BFC_DATETIME_SECOND_OF_DAY:
			SET_SECS(date, (uint32_t) val);
			return (BFC_SUCCESS);
		case BFC_DATETIME_FRACTION_OF_SEC:
			SET_FRAC(date, (uint32_t) val);
			return (BFC_SUCCESS);
		default: ;
		}
	}
	if (val > 0) {
		unsigned long day = val / SECONDS_PER_DAY;
		SET_DAYS(date, day);
		SET_SECS(date, val - ((time_t) day * SECONDS_PER_DAY));
		FRACTION_OF_SEC(date) = 0;
	} else if (val < 0) {
		signed long day = (val - SECONDS_PER_DAY + 1) / SECONDS_PER_DAY;
		SET_DAYS(date, day);
		SET_SECS(date, val - ((time_t) day * SECONDS_PER_DAY));
		FRACTION_OF_SEC(date) = 0;
	} else {
		DAYS_SINCE_1970(date) = 0;
		SECOND_OF_DAY(date)   = 0;
		FRACTION_OF_SEC(date) = 0;
	}
	return (BFC_SUCCESS);
}

long
bfc_datetime_get_long(bfc_cobjptr_t date, size_t pos)
{
	if (pos != 0) {
		switch (pos) {
		case BFC_DATETIME_DAYS_SINCE_1970:
			return ((long) GET_DAYS(date));
		case BFC_DATETIME_SECOND_OF_DAY:
			return ((long) GET_SECS(date));
		case BFC_DATETIME_FRACTION_OF_SEC:
			return ((long) GET_FRAC(date));
		default: ;
		}
	}
	return ((long) 24 * 3600 * GET_DAYS(date) + GET_SECS(date));
}

time_t
bfc_datetime_secs(bfc_cobjptr_t date)
{
	return ((time_t) 24 * 3600 * GET_DAYS(date) + GET_SECS(date));
}

int
bfc_datetime_msecs(bfc_cobjptr_t date)
{
	uint32_t fraction = GET_FRAC(date);
	if (fraction >= 4290000000uL) {
		return (999);
	}
	return ((int) umul32_hiword(fraction + (uint32_t) 2147483uL, 1000u));
}

long
bfc_datetime_usecs(bfc_cobjptr_t date)
{
	uint32_t fraction = GET_FRAC(date);
	if (fraction >= 4294963000uL) {
		return (999999L);
	}
	return ((long) umul32_hiword(fraction + 2147, (uint32_t) 1000000uL));
}

long
bfc_datetime_nsecs(bfc_cobjptr_t date)
{
	uint32_t fraction = GET_FRAC(date);
	if (fraction >= 4294967290uL) {
		return (999999999L);
	}
	return ((long) umul32_hiword(fraction + 2, (uint32_t) 1000000000uL));
}

long
bfc_datetime_secs_between(bfc_cobjptr_t first, bfc_cobjptr_t last)
{
	int32_t days = GET_DAYS(last) - GET_DAYS(first);
	int32_t secs = GET_SECS(last) - GET_SECS(first);

	return ((days == 0)? (long)secs: (long)days * SECONDS_PER_DAY + secs);
}

long
bfc_datetime_msecs_between(bfc_cobjptr_t first, bfc_cobjptr_t last)
{
	uint32_t diff;
	long msecs = bfc_datetime_secs_between(first, last) * 1000u;
	uint32_t first_frac= GET_FRAC(first);
	uint32_t last_frac = GET_FRAC(last);

	if (last_frac > first_frac) {
		diff = last_frac - first_frac;
		msecs += umul32_hiword(diff + (uint32_t) 2147483uL, 1000u);
	} else if (first_frac > last_frac) {
		diff = first_frac - last_frac;
		msecs -= umul32_hiword(diff + (uint32_t) 2147483uL, 1000u);
	}

	return (msecs);
}

long
bfc_datetime_usecs_between(bfc_cobjptr_t first, bfc_cobjptr_t last)
{
	uint32_t diff;
	long usecs = bfc_datetime_secs_between(first, last) * 1000000uL;
	uint32_t first_frac= GET_FRAC(first);
	uint32_t last_frac = GET_FRAC(last);

	if (last_frac > first_frac) {
		diff = last_frac - first_frac;
		usecs += umul32_hiword(diff + 2147 /*round*/, 1000000uL);
	} else if (first_frac > last_frac) {
		diff = first_frac - last_frac;
		usecs -= umul32_hiword(diff + 2147 /*round*/, 1000000uL);
	}

	return (usecs);
}

long
bfc_datetime_nsecs_between(bfc_cobjptr_t first, bfc_cobjptr_t last)
{
	uint32_t diff;
	long nsecs = bfc_datetime_secs_between(first, last) * 1000000000uL;
	uint32_t first_frac= GET_FRAC(first);
	uint32_t last_frac = GET_FRAC(last);

	if (last_frac > first_frac) {
		diff = last_frac - first_frac;
		nsecs += umul32_hiword(diff + 2 /*round*/, 1000000000uL);
	} else if (first_frac > last_frac) {
		diff = first_frac - last_frac;
		nsecs -= umul32_hiword(diff + 2 /*round*/, 1000000000uL);
	}

	return (nsecs);
}

int
bfc_datetime_advance_secs(bfc_objptr_t date, signed long secs)
{
	signed long s = GET_SECS(date) + secs;
	unsigned long days;

	if (s >= 86400L) {
		days = (s < 2*86400L)? 1uL: (unsigned long) s / 86400uL;
		SET_DAYS(date, GET_DAYS(date) + days);
		SET_SECS(date, s - (days * 86400uL));
	} else if (s < 0) {
		days = (s >= -86400L)? 1uL: (unsigned long)(86399L-s)/86400uL;
		SET_DAYS(date, GET_DAYS(date) - days);
		SET_SECS(date, s + (days * 86400uL));
	} else {
		SET_SECS(date, s);
	}

	return (BFC_SUCCESS);
}

int
bfc_datetime_advance_msecs(bfc_objptr_t date, signed long msecs)
{
	int64_t sum  = (int64_t) 4294967L * msecs + GET_FRAC(date);
	int32_t secs = (int32_t) (sum >> 32);

	if (secs == 0) {
		SET_FRAC(date, (uint32_t) sum);
	} else {
		bfc_datetime_advance_secs(date, secs);
		SET_FRAC(date, (uint32_t) sum);
	}

	return (BFC_SUCCESS);
}

int
bfc_datetime_advance_usecs(bfc_objptr_t date, signed long usecs)
{
	int64_t sum  = (int64_t) 4295 * usecs + GET_FRAC(date);
	int32_t secs = (int32_t) (sum >> 32);

	if (secs == 0) {
		SET_FRAC(date, (uint32_t) sum);
	} else {
		bfc_datetime_advance_secs(date, secs);
		SET_FRAC(date, (uint32_t) sum);
	}

	return (BFC_SUCCESS);
}

int
bfc_datetime_advance_nsecs(bfc_objptr_t date, signed long nsecs)
{
	int64_t sum  = (int64_t) 4 * nsecs + GET_FRAC(date);
	int32_t secs = (int32_t) (sum >> 32);

	if (secs == 0) {
		SET_FRAC(date, (uint32_t) sum);
	} else {
		bfc_datetime_advance_secs(date, secs);
		SET_FRAC(date, (uint32_t) sum);
	}

	return (BFC_SUCCESS);
}


#define POSIX_SECS_PER_DAY	86400L

static int
to_localtime(bfc_cobjptr_t date, struct tm *tm)
{
	int rc = BFC_SUCCESS;
	int64_t secs = (int64_t) POSIX_SECS_PER_DAY * GET_DAYS(date)
	             + GET_SECS(date);
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
to_gmtime(bfc_cobjptr_t date, struct tm *tm)
{
	int rc = BFC_SUCCESS;
	int64_t secs = (int64_t) POSIX_SECS_PER_DAY * GET_DAYS(date)
	             + GET_SECS(date);
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
to_worldtime(bfc_cobjptr_t date, int offs, struct tm *tm)
{
	int rc = BFC_SUCCESS;
	int64_t secs = (int64_t) POSIX_SECS_PER_DAY * GET_DAYS(date)
	             + GET_SECS(date) + offset_to_seconds(offs);
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
to_isodate(bfc_cobjptr_t date, char *buf, size_t bufsize)
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
to_world_isodate(bfc_cobjptr_t date, int offs, char *buf, size_t bufsize)
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
to_local_isodate(bfc_cobjptr_t date, char *buf, size_t bufsize)
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
