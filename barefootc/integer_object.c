
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/number.h"
#include "log4stdc.h"

#if defined(_MSC_VER)
#define snprintf _snprintf
#endif

static void *
place_integer(bfc_objptr_t obj, size_t pos, void *p, bfc_mempool_t pool);

const struct bfc_classhdr bfc_number_class = {
	/* intentionally not using selective initialization for base class: */
	/* I want the compiler to complain if something is missing.         */
	/* .super 	*/ NULL,
	/* .name 	*/ "number",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ bfc_init_integer_object,
	/* .initrefc 	*/ bfc_default_init_refcount,
	/* .incrrefc 	*/ bfc_default_incr_refcount,
	/* .decrrefc 	*/ bfc_default_decr_refcount,
	/* .destroy 	*/ bfc_number_destroy_object,
	/* .clone 	*/ bfc_number_clone_object,
	/* .clonesize 	*/ bfc_get_integer_object_size,
	/* .hashcode 	*/ bfc_integer_get_hashcode,
	/* .equals 	*/ bfc_integer_is_equal,
	/* .length 	*/ bfc_number_get_object_length,
	/* .tostring 	*/ bfc_integer_object_tostring,
	/* .dump 	*/ bfc_integer_dump_object,
	/* .first 	*/ bfc_number_first,
	/* .index 	*/ bfc_number_index,
	/* .getl 	*/ bfc_integer_getlong,
	/* .setl 	*/ bfc_integer_setlong,
	/* .place 	*/ place_integer,
	NULL,
};

#define UNSIGNED_INT(obj)	(obj)->private_6
#define GET_UNSIGNED(obj)	(UNSIGNED_INT(obj))
#define GET_SIGNED(obj)		((ptrdiff_t) (UNSIGNED_INT(obj)))
#define SET_NUMBER(obj,val)	UNSIGNED_INT(obj) = (size_t)(ptrdiff_t)(val)

int
bfc_init_integer_object(void *buf, size_t bufsize, bfc_mempool_t pool)
{
	bfc_objptr_t obj = (bfc_objptr_t) buf;
	if (bufsize < sizeof(*obj)) {
		return (-ENOSPC);
	} else {
		memset(obj, 0, sizeof(*obj));
		obj->vptr = (bfc_classptr_t) &bfc_number_class;
	}
	return (BFC_SUCCESS);
}

void
bfc_number_destroy_object(bfc_objptr_t obj)
{
	obj->vptr = NULL;
}

int
bfc_number_clone_object(bfc_cobjptr_t obj,
			void *buf, size_t bufsize, bfc_mempool_t pool)
{
	size_t size = bfc_object_size(obj);
	if (bufsize < size) {
		return (-ENOSPC);
	} else {
		memcpy(buf, obj, size);
	}
	return (BFC_SUCCESS);
}

size_t
bfc_get_integer_object_size(bfc_cobjptr_t obj)
{
	return (sizeof(bfc_number_t));
}

unsigned  
bfc_integer_get_hashcode(bfc_cobjptr_t obj, int hashlen)
{
	return (bfc_reduce_hashcode(UNSIGNED_INT(obj),
				    8*sizeof(UNSIGNED_INT(obj)), hashlen));
}

int
bfc_integer_is_equal(bfc_cobjptr_t obj, bfc_cobjptr_t other)
{
	return (UNSIGNED_INT(obj) == UNSIGNED_INT(other));
}

size_t
bfc_number_get_object_length(bfc_cobjptr_t obj)
{
	return (1);
}

const void *
bfc_number_first(bfc_cobjptr_t obj)
{
	return (&UNSIGNED_INT(obj));
}

void *
bfc_number_index(bfc_objptr_t obj, size_t pos)
{
	return (&UNSIGNED_INT(obj));
}

long
bfc_integer_getlong(bfc_cobjptr_t obj, size_t pos)
{
	return ((long) GET_SIGNED(obj));
}

int
bfc_integer_setlong(bfc_objptr_t obj, size_t pos, long val)
{
	SET_NUMBER(obj, val);
	return(BFC_SUCCESS);
}

static void *
place_integer(bfc_objptr_t obj, size_t pos, void *p, bfc_mempool_t pool)
{
	bfc_objptr_t other = (bfc_objptr_t) p;

	if (other && bfc_instance_of_class(other, BFC_CLASS(obj))) {
		UNSIGNED_INT(obj) = UNSIGNED_INT(other);
	} else if (other && BFC_CLASS(other)) {
		SET_NUMBER(obj, bfc_object_getlong(other));
	}
	return (obj);
}

int
bfc_integer_object_tostring(bfc_cobjptr_t obj, char *buf, size_t bufsize,
			    const char *fmt)
{
	int rc = 0;
	char *dp = buf;
	char tmp[40];
		
	if (obj == NULL) {
		return (-EFAULT);
	} else if ((buf == NULL) || (bufsize == 0)) {
		dp = tmp;
	}

	if (fmt == NULL) {
		if ((rc = snprintf(dp, bufsize, "%ld", (long) GET_SIGNED(obj))) < 0) {
			rc = snprintf(tmp, sizeof(tmp), "%ld", (long) GET_SIGNED(obj));
		}
	} else if (strchr(fmt, 'l')) {
		if ((rc = snprintf(dp, bufsize, fmt, (long) GET_SIGNED(obj))) < 0) {
			rc = snprintf(tmp, sizeof(tmp), fmt, (long) GET_SIGNED(obj));
		}
	} else if (strchr(fmt, 'u')) {
		if ((rc = snprintf(dp, bufsize, fmt, (unsigned) GET_UNSIGNED(obj))) < 0) {
			rc = snprintf(tmp, sizeof(tmp), fmt, (unsigned) GET_SIGNED(obj));
		}
	} else {
		if ((rc = snprintf(dp, bufsize, fmt, (int) GET_SIGNED(obj))) < 0) {
			rc = snprintf(tmp, sizeof(tmp), fmt, (int) GET_SIGNED(obj));
		}
	}
	return (rc);
}

void
bfc_integer_dump_object(bfc_cobjptr_t obj, int depth, struct l4sc_logger *log)
{
	if (obj && BFC_CLASS(obj)) {
		L4SC_DEBUG(log, "%s object @%p: %ld",
			BFC_CLASS(obj)->name, obj, (long) GET_SIGNED(obj));
	}
}

