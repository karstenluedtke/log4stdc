
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/atomic.h"
#include "log4stdc.h"

#if defined(_MSC_VER)
#define snprintf _snprintf
#endif

static void bfc_destroy_base_object(bfc_objptr_t obj);

const struct bfc_classhdr bfc_object_class = {
	/* intentionally not using selective initialization for base class: */
	/* I want the compiler to complain if something is missing.         */
	/* .super 	*/ NULL,
	/* .name 	*/ "object",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ bfc_default_init_object,
	/* .initrefc 	*/ bfc_default_init_refcount,
	/* .incrrefc 	*/ bfc_default_incr_refcount,
	/* .decrrefc 	*/ bfc_default_decr_refcount,
	/* .destroy 	*/ bfc_destroy_base_object,
	/* .clone 	*/ bfc_default_clone_object,
	/* .clonesize 	*/ bfc_get_base_object_size,
	/* .hashcode 	*/ bfc_default_get_object_hashcode,
	/* .equals 	*/ bfc_default_is_equal_object,
	/* .length 	*/ bfc_default_get_object_length,
	/* .tostring 	*/ bfc_default_object_tostring,
	/* .dump 	*/ bfc_default_dump_object,
	/* .spare13 	*/ NULL,
	/* .spare14 	*/ NULL,
	/* .spare15 	*/ NULL,
};

int
bfc_default_init_object(void *buf, size_t bufsize, struct mempool *pool)
{
	BFC_INIT_PROLOGUE(const struct bfc_object_class *,
			  bfc_objptr_t, object, buf, bufsize, pool,
			  &bfc_object_class);
	return (BFC_SUCCESS);
}

void
bfc_default_init_refcount(bfc_objptr_t obj, int n)
{
	bfc_init_atomic_counter(obj->refc, n);
}

void
bfc_default_incr_refcount(bfc_objptr_t obj)
{
	(void) bfc_incr_atomic_counter(obj->refc);
}

int
bfc_default_decr_refcount(bfc_objptr_t obj)
{
	int unrefd = bfc_decr_and_test_atomic_counter(obj->refc);
	if (unrefd) {
		bfc_destroy(obj);
	}
	return (!unrefd);
}

void
bfc_default_destroy_object(bfc_objptr_t obj)
{
	bfc_classptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

static void
bfc_destroy_base_object(bfc_objptr_t obj)
{
	obj->vptr = 0;
}

int
bfc_default_clone_object(bfc_cobjptr_t obj, void *buf, size_t bufsize)
{
	bfc_objptr_t object = (bfc_objptr_t) buf;
	size_t size = bfc_object_size(obj);
	if (bufsize < size) {
		return (-ENOSPC);
	}
	memcpy(object, obj, size);
	if (size >= sizeof(struct bfc_objhdr)) {
		object->lock = NULL;
		object->next = NULL;
		object->prev = NULL;
	}
	bfc_init_refcount(object, 0);
	return (BFC_SUCCESS);
}

size_t
bfc_get_base_object_size(bfc_cobjptr_t obj)
{
	return (sizeof(struct bfc_objhdr));
}

unsigned  
bfc_default_get_object_hashcode(bfc_cobjptr_t obj)
{
	size_t x = (size_t) obj;
	if (sizeof(x) > sizeof(unsigned)) {
		x ^= x >> (8*sizeof(unsigned));
	}
	return ((unsigned) x);
}

int
bfc_default_is_equal_object(bfc_cobjptr_t obj, bfc_cobjptr_t other)
{
	return (obj == other);
}

size_t
bfc_default_get_object_length(bfc_cobjptr_t obj)
{
	return (0);
}

int
bfc_default_object_tostring(bfc_cobjptr_t obj, char *buf, size_t bufsize)
{
	if (obj && obj->name && buf) {
		snprintf(buf, bufsize, "%s@%p", obj->name, obj);
	}
	return (0);
}

void
bfc_default_dump_object(bfc_cobjptr_t obj, int depth, struct l4sc_logger *log)
{
	size_t size;
	const unsigned char *p, *lim;

	if (obj && obj->name && BFC_CLASS(obj)) {
		L4SC_DEBUG(log, "object \"%s\" @%p", obj->name, obj);
		L4SC_DEBUG(log, " class \"%s\", pool %p, lock %p, refc %d",
			BFC_CLASS(obj)->name, obj->pool, obj->lock, obj->refc);
		size = bfc_object_size(obj);
		lim  = ((const unsigned char *) obj) + size;
		for (p = (const unsigned char *) &obj->next; p < lim; p += 8) {
			L4SC_DEBUG(log,
			  " %p: %02x %02x %02x %02x %02x %02x %02x %02x",
			  p, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
		}
	}
}

