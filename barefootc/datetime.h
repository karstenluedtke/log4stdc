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
#include <time.h>

#include "barefootc/object.h"

struct tm;

struct bfc_datetime_class;
typedef const struct bfc_datetime_class *bfc_datetime_classptr_t;

#define BFC_DATETIMEHDR(classptrT,objptrT) \
	classptrT	vptr;	/**< virtual methods */			\
	int32_t		day;	/**< day after Jan 1st, 1970; or 0   */	\
	uint32_t	secs;	/**< seconds of the day              */	\
	uint32_t	frac;	/**< fractional part of time, 1s/2**32 */

struct bfc_datetime {
	bfc_datetime_classptr_t vptr; /**< virtual methods */
	int32_t		day;	/**< day after Jan 1st, 1970; or 0     */
	uint32_t	secs;	/**< seconds of the day                */
	uint32_t	frac;	/**< fractional part of time, 1s/2**32 */
};

typedef struct bfc_datetime bfc_datetime_t;
typedef struct bfc_datetime *bfc_dateptr_t;
typedef const struct bfc_datetime *bfc_cdateptr_t;

typedef struct bfc_datetime *bfc_timeptr_t;
typedef const struct bfc_datetime *bfc_ctimeptr_t;

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
				bfc_dateptr_t, bfc_cdateptr_t)
};

struct timeval;
struct timespec;

int	bfc_init_datetime(void *buf, size_t bufsize);
int	bfc_init_current_datetime(void *buf, size_t bufsize);
int	bfc_init_datetime_from_time_t(void *buf, size_t bufsize, time_t secs);
int	bfc_init_datetime_from_timeval(void *buf, size_t bufsize,
					const struct timeval *tv);
int	bfc_init_datetime_from_timespec(void *buf, size_t bufsize,
					const struct timespec *spec);
int	bfc_init_datetime_from_isotime(void *buf, size_t bufsize,
					const char *s, size_t len);
int	bfc_init_precision_datetime(void *buf, size_t bufsize,
					time_t secs, unsigned long nsecs);
void	bfc_destroy_datetime(bfc_dateptr_t date);
int	bfc_clone_datetime(bfc_cdateptr_t obj, void *buf, size_t bufsize);
size_t	bfc_datetime_objsize(bfc_cdateptr_t date);
int	bfc_datetime_equals(bfc_cdateptr_t date, bfc_cdateptr_t other);
size_t	bfc_datetime_length(bfc_cdateptr_t date);
size_t	bfc_datetime_position(bfc_cdateptr_t date);

time_t	bfc_datetime_secs(bfc_cdateptr_t date);
int	bfc_datetime_msecs(bfc_cdateptr_t date);
long	bfc_datetime_usecs(bfc_cdateptr_t date);
long	bfc_datetime_nsecs(bfc_cdateptr_t date);

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_DATETIME_H_ */
