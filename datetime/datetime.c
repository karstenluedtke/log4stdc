
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/datetime.h"
#include "log4stdc.h"
#include "umul32_hiword.h"

static int init_datetime(void *buf, size_t bufsize, struct mempool *pool);
static int datetime_equals(bfc_cdateptr_t date, bfc_cdateptr_t other);
static unsigned bfc_datetime_hashcode(bfc_cdateptr_t date);
static void dump_datetime(bfc_cdateptr_t date,int depth,struct l4sc_logger*log);
static int datetime_tostring(bfc_cdateptr_t date, char *buf, size_t bufsize);

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
	/* .getl	*/ NULL,
	/* .setl	*/ NULL,
	/* .spare17 	*/ NULL,
	/* .ibegin	*/ NULL,
	/* .ilimit	*/ NULL,
	/* .rbegin	*/ NULL,
	/* .rlimit	*/ NULL,
	/* .spare22 	*/ NULL,
	/* .spare23 	*/ NULL,
	/* comparision */
	/* .secs_between	*/ NULL,
	/* .msecs_between	*/ NULL,
	/* .usecs_between	*/ NULL,
	/* .nsecs_between	*/ NULL,
	/* conversion and formatting */
	/* .get_secs		*/ bfc_datetime_secs,
	/* .get_msecs		*/ bfc_datetime_msecs,
	/* .get_usecs		*/ bfc_datetime_usecs,
	/* .get_nsecs		*/ bfc_datetime_nsecs,
	/* .to_gmtime		*/ NULL,
	/* .to_localtime	*/ NULL,
	/* .to_worldtime	*/ NULL,
	/* .to_isodate		*/ NULL,
	/* .to_local_isodate	*/ NULL,
	/* .to_world_isodate	*/ NULL,
	/* .format		*/ NULL,
	/* modifiers */	
	/* .advance_secs	*/ NULL,
	/* .advance_msecs	*/ NULL,
	/* .advance_usecs	*/ NULL,
	/* .advance_nsecs	*/ NULL,
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

time_t
bfc_datetime_secs(bfc_cdateptr_t date)
{
	return ((time_t) 24 * 3600 * date->day + date->secs);
}

int
bfc_datetime_msecs(bfc_cdateptr_t date)
{
	return ((int) umul32_hiword(date->frac, 1000u));
}

long
bfc_datetime_usecs(bfc_cdateptr_t date)
{
	return ((long) umul32_hiword(date->frac, (uint32_t) 1000000uL));
}

long
bfc_datetime_nsecs(bfc_cdateptr_t date)
{
	return ((long) umul32_hiword(date->frac, (uint32_t) 1000000000uL));
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
	long msecs = bfc_datetime_secs_between(first, last) * 1000u;

	if (last->frac > first->frac) {
		msecs += umul32_hiword(last->frac - first->frac, 1000u);
	} else if (first->frac > last->frac) {
		msecs -= umul32_hiword(first->frac - last->frac, 1000u);
	}

	return (msecs);
}

long
bfc_datetime_usecs_between(bfc_cdateptr_t first, bfc_cdateptr_t last)
{
	long usecs = bfc_datetime_secs_between(first, last) * 1000000uL;

	if (last->frac > first->frac) {
		usecs += umul32_hiword(last->frac - first->frac, 1000000uL);
	} else if (first->frac > last->frac) {
		usecs -= umul32_hiword(first->frac - last->frac, 1000000uL);
	}

	return (usecs);
}

long
bfc_datetime_nsecs_between(bfc_cdateptr_t first, bfc_cdateptr_t last)
{
	long nsecs = bfc_datetime_secs_between(first, last) * 1000000000uL;

	if (last->frac > first->frac) {
		nsecs += umul32_hiword(last->frac - first->frac, 1000000000uL);
	} else if (first->frac > last->frac) {
		nsecs -= umul32_hiword(first->frac - last->frac, 1000000000uL);
	}

	return (nsecs);
}

