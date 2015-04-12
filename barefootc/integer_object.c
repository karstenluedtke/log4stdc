
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
	struct bfc_integer *obj = (struct bfc_integer *) buf;
	if (bufsize < sizeof(*obj)) {
		return (-ENOSPC);
	} else {
		memset(obj, 0, sizeof(*obj));
		obj->vptr = (bfc_classptr_t) &bfc_integer_class;
	}
	return (BFC_SUCCESS);
}

void
bfc_number_init_refcount(bfc_objptr_t obj, int n)
{
}

void
bfc_number_incr_refcount(bfc_objptr_t obj)
{
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
	return (sizeof(struct bfc_integer));
}

unsigned  
bfc_integer_get_hashcode(bfc_cobjptr_t obj, int hashlen)
{
	const struct bfc_integer *p = (const struct bfc_integer *) obj;
	return (bfc_reduce_hashcode(p->n, 8*sizeof(p->n), hashlen));
}

int
bfc_integer_is_equal(bfc_cobjptr_t obj, bfc_cobjptr_t other)
{
	const struct bfc_integer *p = (const struct bfc_integer *) obj;
	const struct bfc_integer *q = (const struct bfc_integer *) other;
	return (p->n == q->n);
}

size_t
bfc_number_get_object_length(bfc_cobjptr_t obj)
{
	return (1);
}

int
bfc_integer_object_tostring(bfc_cobjptr_t obj, char *buf, size_t bufsize)
{
	const struct bfc_integer *p = (const struct bfc_integer *) obj;

	if (p && buf) {
		snprintf(buf, bufsize, "%ld", (long) p->n);
	}
	return (0);
}

void
bfc_integer_dump_object(bfc_cobjptr_t obj, int depth, struct l4sc_logger *log)
{
	const struct bfc_integer *p = (const struct bfc_integer *) obj;

	if (obj && BFC_CLASS(obj) && p) {
		L4SC_DEBUG(log, "%s object @%p: %ld",
			BFC_CLASS(obj)->name, obj, (long) p->n);
	}
}

