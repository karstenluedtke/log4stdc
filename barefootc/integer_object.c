
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

const struct bfc_classhdr bfc_integer_class = {
	/* intentionally not using selective initialization for base class: */
	/* I want the compiler to complain if something is missing.         */
	/* .super 	*/ NULL,
	/* .name 	*/ "integer",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ bfc_init_integer_object,
	/* .initrefc 	*/ bfc_number_init_refcount,
	/* .incrrefc 	*/ bfc_number_incr_refcount,
	/* .decrrefc 	*/ bfc_number_decr_refcount,
	/* .destroy 	*/ bfc_number_destroy_object,
	/* .clone 	*/ bfc_number_clone_object,
	/* .clonesize 	*/ bfc_get_integer_object_size,
	/* .hashcode 	*/ bfc_integer_get_hashcode,
	/* .equals 	*/ bfc_integer_is_equal,
	/* .length 	*/ bfc_number_get_object_length,
	/* .tostring 	*/ bfc_integer_object_tostring,
	/* .dump 	*/ bfc_integer_dump_object,
	/* .spare13 	*/ NULL,
	/* .spare14 	*/ NULL,
	/* .spare15 	*/ NULL,
};

int
bfc_init_integer_object(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_numptr_t obj = (bfc_numptr_t) buf;
	if (bufsize < sizeof(*obj)) {
		return (-ENOSPC);
	} else {
		memset(obj, 0, sizeof(*obj));
		obj->vptr = (bfc_classptr_t) &bfc_integer_class;
	}
	return (BFC_SUCCESS);
}

int
bfc_number_init_refcount(bfc_objptr_t obj, int n)
{
	return (-ENOSYS);
}

int
bfc_number_incr_refcount(bfc_objptr_t obj)
{
	return (-ENOSYS);
}

int
bfc_number_decr_refcount(bfc_objptr_t obj)
{
	return (-ENOSYS);
}

void
bfc_number_destroy_object(bfc_objptr_t obj)
{
	obj->vptr = NULL;
}

int
bfc_number_clone_object(bfc_cobjptr_t obj,
			void *buf, size_t bufsize, struct mempool *pool)
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
	bfc_cnumptr_t p = (bfc_cnumptr_t) obj;
	return (bfc_reduce_hashcode(p->u.n, 8*sizeof(p->u.n), hashlen));
}

int
bfc_integer_is_equal(bfc_cobjptr_t obj, bfc_cobjptr_t other)
{
	bfc_cnumptr_t p = (bfc_cnumptr_t) obj;
	bfc_cnumptr_t q = (bfc_cnumptr_t) other;
	return (p->u.n == q->u.n);
}

size_t
bfc_number_get_object_length(bfc_cobjptr_t obj)
{
	return (1);
}

int
bfc_integer_object_tostring(bfc_cobjptr_t obj, char *buf, size_t bufsize,
			    const char *fmt)
{
	bfc_cnumptr_t p = (bfc_cnumptr_t) obj;

	if (p && buf) {
		if (fmt == NULL) {
			snprintf(buf, bufsize, "%ld", (long) p->u.n);
		} else if (strchr(fmt, 'l')) {
			snprintf(buf, bufsize, fmt, (long) p->u.n);
		} else if (strchr(fmt, 'u')) {
			snprintf(buf, bufsize, fmt, (unsigned) p->u.n);
		} else {
			snprintf(buf, bufsize, fmt, (int) p->u.n);
		}
	}
	return (0);
}

void
bfc_integer_dump_object(bfc_cobjptr_t obj, int depth, struct l4sc_logger *log)
{
	bfc_cnumptr_t p = (bfc_cnumptr_t) obj;

	if (obj && BFC_CLASS(obj) && p) {
		L4SC_DEBUG(log, "%s object @%p: %ld",
			BFC_CLASS(obj)->name, obj, (long) p->u.n);
	}
}

