
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/datetime.h"
#include "log4stdc.h"

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

static inline uint32_t
umul_hiword32(uint32_t a, uint32_t b)
{
	uint32_t hiword;

#if defined(__i386) || defined(__i386__) || \
    defined(__x86_64) || defined(__x86_64__) || defined(__amd64)
	__asm__ __volatile__(
	"mul %2" : "=d"(hiword) : "a"(a), "d"(b) : "cc");

#elif defined(__ARMEL__) || defined(__ARMEB__) || defined(__ARM_EABI__)
	uint32_t loword;
	__asm__ __volatile__(
	"umull %0,%1,%2,%3" : "=&r"(loword), "=&r"(hiword) : "r"(a), "r"(b));
	/* "=&r" means early clobbering, */
	/* these register must not be used as input registers */
#else
	hiword = (uint32_t) (((uint64_t) a * b) >> 32);
#endif
	return (hiword);
}

int
bfc_datetime_msecs(bfc_cdateptr_t date)
{
	return ((int) umul_hiword32(date->frac, 1000u));
}

long
bfc_datetime_usecs(bfc_cdateptr_t date)
{
	return ((long) umul_hiword32(date->frac, (uint32_t) 1000000uL));
}

long
bfc_datetime_nsecs(bfc_cdateptr_t date)
{
	return ((long) umul_hiword32(date->frac, (uint32_t) 1000000000uL));
}

