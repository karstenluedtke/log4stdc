/**
 * @file      datetime.h
 *
 * @brief     A generic representation of date and time.
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-08-31
 *
 * Copyright (c)  2014  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_DATETIME_H_
#define _BFC_DATETIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include "barefootc/object.h"

#define BFC_DATETIME_LOGGER	"barefootc.datetime",18

struct tm;

struct bfc_datetime_class;
typedef const struct bfc_datetime_class *bfc_datetime_classptr_t;

/**
 * @brief   Representation of date and time.
 *
 * Actually the representation below hides the complexity resulting from
 * the insertion of leap seconds in UTC.
 *
 * A day sometimes has 86401 seconds, in contrast to the POSIX definition
 * that "each and every day shall be accounted for by exactly 86400 seconds"
 * [The Open Group Base Specifications Issue 7, IEEE Std 1003.1, 2013 Edition].
 *
 * On the other hand the system time is usually adjusted (via NTP or manually)
 * by repeating a second whenever a leap second occurs.
 *
 * So the current system time always represents UTC, and other points in time
 * can be calculated by multiplying the difference in days and 86400 according
 * to the POSIX definition.
 */

#if 0
struct bfc_datetime {
	BFC_OBJHDR(bfc_datetime_classptr_t,struct bfc_datetime *)
	int32_t		day;	/**< day after Jan 1st, 1970; or 0     */
	uint32_t	secs;	/**< seconds of the day (UTC)          */
	uint32_t	frac;	/**< fractional part of time, 1s/2**32 */
};
#endif

typedef struct bfc_basic_object bfc_datetime_t;

#define BFC_DATETIME_METHODS(dateptrT,cdateptrT)			\
	/* comparision */						\
	long	(*secs_between)(cdateptrT first, cdateptrT last);	\
	long	(*msecs_between)(cdateptrT first, cdateptrT last);	\
	long	(*usecs_between)(cdateptrT first, cdateptrT last);	\
	long	(*nsecs_between)(cdateptrT first, cdateptrT last);	\
	/* conversion and formatting */					\
	time_t	(*get_secs)(cdateptrT date);				\
	int	(*get_msecs)(cdateptrT date);	/* without secs */	\
	long	(*get_usecs)(cdateptrT date);	/* without secs */	\
	long	(*get_nsecs)(cdateptrT date);	/* without secs */	\
	int	(*to_gmtime)(cdateptrT date, struct tm *tm);		\
	int	(*to_localtime)(cdateptrT date, struct tm *tm);		\
	int	(*to_worldtime)(cdateptrT date,int offs,struct tm *tm);	\
	int	(*to_isodate)(cdateptrT date,char *buf,size_t bufsize);	\
	int	(*to_local_isodate)(cdateptrT date,			\
					char *buf, size_t bufsize);	\
	int	(*to_world_isodate)(cdateptrT date, int offs,		\
					char *buf, size_t bufsize);	\
	int	(*format)(cdateptrT date, const char *fmt,		\
					char *buf, size_t bufsize);	\
	/* modifiers */							\
	int	(*advance_secs)(dateptrT date, signed long secs);	\
	int	(*advance_msecs)(dateptrT date, signed long msecs);	\
	int	(*advance_usecs)(dateptrT date, signed long usecs);	\
	int	(*advance_nsecs)(dateptrT date, signed long nsecs);	\
	/* Check nothing is missing */					\
	void	(*last_method)(void);

#define BFC_DATETIME_CLASS_DEF(classptrT,dateptrT,cdateptrT) \
	BFC_CLASSHDR(classptrT, dateptrT, cdateptrT)			\
	BFC_DATETIME_METHODS(dateptrT, cdateptrT)

struct bfc_datetime_class {
	BFC_DATETIME_CLASS_DEF(bfc_datetime_classptr_t,
				bfc_objptr_t, bfc_cobjptr_t)
};

struct timeval;
struct timespec;

int	bfc_init_datetime(void *buf, size_t bufsize);
int	bfc_init_datetime_copy(void *buf, size_t bufsize, const void *src);
int	bfc_init_current_datetime(void *buf, size_t bufsize);
int	bfc_init_datetime_from_time_t(void *buf, size_t bufsize, time_t secs);
int	bfc_init_datetime_from_timeval(void *buf, size_t bufsize,
					const struct timeval *tv);
int	bfc_init_datetime_from_timespec(void *buf, size_t bufsize,
					const struct timespec *spec);
int	bfc_init_datetime_from_isotime(void *buf, size_t bufsize,
					const char *s, size_t len);
int	bfc_init_datetime_precise(void *buf, size_t bufsize,
					time_t secs, unsigned long nsecs);
void	bfc_destroy_datetime(bfc_objptr_t date);
int	bfc_clone_datetime(bfc_cobjptr_t obj, void *buf, size_t bufsize);
size_t	bfc_datetime_objsize(bfc_cobjptr_t date);
int	bfc_datetime_equals(bfc_cobjptr_t date, bfc_cobjptr_t other);
size_t	bfc_datetime_length(bfc_cobjptr_t date);
size_t	bfc_datetime_position(bfc_cobjptr_t date);

time_t	bfc_datetime_secs(bfc_cobjptr_t date);
int	bfc_datetime_msecs(bfc_cobjptr_t date);
long	bfc_datetime_usecs(bfc_cobjptr_t date);
long	bfc_datetime_nsecs(bfc_cobjptr_t date);

long	bfc_datetime_get_long(bfc_cobjptr_t date, size_t pos);
int	bfc_datetime_set_long(bfc_objptr_t date, size_t pos, long val);
#define BFC_DATETIME_DAYS_SINCE_1970	1
#define BFC_DATETIME_SECOND_OF_DAY	2
#define BFC_DATETIME_FRACTION_OF_SEC	3

long	bfc_datetime_secs_between(bfc_cobjptr_t first, bfc_cobjptr_t last);
long	bfc_datetime_msecs_between(bfc_cobjptr_t first, bfc_cobjptr_t last);
long	bfc_datetime_usecs_between(bfc_cobjptr_t first, bfc_cobjptr_t last);
long	bfc_datetime_nsecs_between(bfc_cobjptr_t first, bfc_cobjptr_t last);

int	bfc_datetime_advance_secs(bfc_objptr_t date, signed long secs);
int	bfc_datetime_advance_msecs(bfc_objptr_t date, signed long msecs);
int	bfc_datetime_advance_usecs(bfc_objptr_t date, signed long usecs);
int	bfc_datetime_advance_nsecs(bfc_objptr_t date, signed long nsecs);

long	bfc_datetime_days_to_date(unsigned year, unsigned month, unsigned day);
uint32_t bfc_datetime_frac_from_decimal(unsigned long sub, int sublen);

int	bfc_datetime_to_gmtime(bfc_cobjptr_t date, struct tm *tm);
int	bfc_datetime_to_localtime(bfc_cobjptr_t date, struct tm *tm);
int	bfc_datetime_to_worldtime(bfc_cobjptr_t date,int offs,struct tm *tm);
int	bfc_datetime_to_isodate(bfc_cobjptr_t date,char *buf,size_t bufsize);
int	bfc_datetime_to_local_isodate(bfc_cobjptr_t date,
					char *buf, size_t bufsize);
int	bfc_datetime_to_world_isodate(bfc_cobjptr_t date, int offs,
					char *buf, size_t bufsize);
int	bfc_datetime_format(bfc_cobjptr_t date, const char *fmt,
					char *buf, size_t bufsize);

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_DATETIME_H_ */
