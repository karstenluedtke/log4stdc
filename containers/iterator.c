
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "log4stdc.h"

static int default_init_iterator(void *buf,size_t bufsize,struct mempool *pool);
static unsigned bfc_iterator_hashcode(bfc_citerptr_t it);

static const void *iterator_first(bfc_citerptr_t);
static void *forward_data(bfc_iterptr_t, size_t pos);
static int advance_forward(bfc_iterptr_t it, ptrdiff_t n);
static ptrdiff_t forward_distance(bfc_citerptr_t first, bfc_citerptr_t last);

static void *reverse_data(bfc_iterptr_t it, size_t pos);
static int advance_reverse(bfc_iterptr_t it, ptrdiff_t n);
static ptrdiff_t reverse_distance(bfc_citerptr_t first, bfc_citerptr_t last);
static void last_method(void) { }

struct bfc_iterator_class bfc_forward_iterator_class = {
	/* intentionally not using selective initialization for base class: */
	/* I want the compiler to complain if something is missing.         */
	/* .super 	*/ NULL,
	/* .name 	*/ "forward iterator",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ default_init_iterator,
	/* .destroy 	*/ bfc_destroy_iterator,
	/* .clone 	*/ (void *) bfc_default_clone_object,
	/* .clonesize 	*/ bfc_iterator_objsize,
	/* .hashcode 	*/ bfc_iterator_hashcode,
	/* .equals 	*/ bfc_iterator_equals,
	/* .length 	*/ bfc_iterator_length,
	/* .tostring 	*/ (void *) bfc_default_object_tostring,
	/* .dump 	*/ (void *) bfc_default_dump_object,
	/* Element access */
	/* .first	*/ iterator_first,
	/* .data	*/ forward_data,
	/* .get		*/ NULL,
	/* Allocators 	*/
	/* .initialize	*/ bfc_init_iterator,
	/* Iterator functions */
	/* .advance	*/ advance_forward,
	/* .distance	*/ forward_distance,
	/* .last_method	*/ last_method
};

struct bfc_iterator_class bfc_reverse_iterator_class = {
	/* intentionally not using selective initialization for base class: */
	/* I want the compiler to complain if something is missing.         */
	/* .super 	*/ &bfc_forward_iterator_class,
	/* .name 	*/ "reverse iterator",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ default_init_iterator,
	/* .destroy 	*/ bfc_destroy_iterator,
	/* .clone 	*/ (void *) bfc_default_clone_object,
	/* .clonesize 	*/ bfc_iterator_objsize,
	/* .hashcode 	*/ bfc_iterator_hashcode,
	/* .equals 	*/ bfc_iterator_equals,
	/* .length 	*/ bfc_iterator_length,
	/* .tostring 	*/ (void *) bfc_default_object_tostring,
	/* .dump 	*/ (void *) bfc_default_dump_object,
	/* Element access */
	/* .first	*/ iterator_first,
	/* .data	*/ reverse_data,
	/* .get		*/ NULL,
	/* Allocators 	*/
	/* .initialize	*/ bfc_init_iterator,
	/* Iterator functions */
	/* .advance	*/ advance_reverse,
	/* .distance	*/ reverse_distance,
	/* .last_method	*/ last_method
};

static int
default_init_iterator(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_iterptr_t it = (bfc_iterptr_t) buf;
	if (bufsize < sizeof(*it)) {
		return (-ENOSPC);
	}
	memset(it, 0, sizeof(*it));
	it->vptr = &bfc_forward_iterator_class;
	return (BFC_SUCCESS);
}

int
bfc_init_iterator(void *buf, size_t bufsize, bfc_cobjptr_t obj, size_t pos)
{
	int rc;

	if ((rc = default_init_iterator(buf, bufsize, NULL)) == BFC_SUCCESS) {
		bfc_iterptr_t it = (bfc_iterptr_t) buf;
		size_t len = bfc_object_length(obj);
		it->obj = (bfc_objptr_t) (uintptr_t) obj;
		if (pos == BFC_NPOS) {
			it->pos = len;
		} else if (pos > len) {
			it->pos = len;
			rc = -ERANGE;
		} else {
			it->pos = pos;
		}
	}
	return (rc);
}

int
bfc_init_reverse_iterator(void *buf,size_t bufsize,bfc_cobjptr_t obj,size_t pos)
{
	int rc;
	bfc_iterptr_t it = (bfc_iterptr_t) buf;

	rc = bfc_init_iterator(buf, bufsize, obj, pos);
	it->vptr = &bfc_reverse_iterator_class;
}

void
bfc_destroy_iterator(bfc_iterptr_t it)
{
	it->vptr = NULL;
}

size_t
bfc_iterator_objsize(bfc_citerptr_t it)
{
	return (sizeof(struct bfc_iterator));
}

static unsigned  
bfc_iterator_hashcode(bfc_citerptr_t it)
{
	const char *p = (const char *) it->obj;
	size_t x = (size_t) (p + it->pos);
	if (sizeof(x) > sizeof(unsigned)) {
		x ^= x >> (8*sizeof(unsigned));
	}
	return ((unsigned) x);
}

int
bfc_iterator_equals(bfc_citerptr_t it, bfc_citerptr_t other)
{
	if (it == other) {
		return (1);
	}
	if (it->obj == other->obj) {
		if (it->pos == other->pos) {
			return (1);
		}
		if ((other->pos >= bfc_object_length(other->obj)) /* NPOS */
		 && (it->pos >= bfc_object_length(it->obj))) {
			return (1);
		}
	}
	return (0);
}

size_t
bfc_iterator_length(bfc_citerptr_t it)
{
	return (1);
}

static const void *
iterator_first(bfc_citerptr_t it)
{
	RETURN_METHCALL(bfc_classptr_t, it->obj,
			data, (it->obj, it->pos), NULL);
}

static inline void *
iterator_data(bfc_iterptr_t it, ptrdiff_t n)
{
	if (n == 0) {
		RETURN_METHCALL(bfc_classptr_t, it->obj,
				data, (it->obj, it->pos), NULL);
	} else if (n > 0) {
		size_t objlen = bfc_object_length(it->obj);
		if (it->pos + n < objlen) {
			RETURN_METHCALL(bfc_classptr_t, it->obj,
					data, (it->obj, it->pos + n), NULL);
		}
	} else if (it->pos >= (-n)) {
		RETURN_METHCALL(bfc_classptr_t, it->obj,
				data, (it->obj, it->pos + n), NULL);
	}
	return (NULL);
}

static void *
forward_data(bfc_iterptr_t it, size_t pos)
{
	return (iterator_data(it, (ptrdiff_t) pos));
}

static int
advance_forward(bfc_iterptr_t it, ptrdiff_t n)
{
	if (n > 0) {
		size_t objlen = bfc_object_length(it->obj);
		if (it->pos + n <= objlen) {
			it->pos += n;
		}
	} else if (it->pos >= (-n)) {
		it->pos += n;
	}
}

static ptrdiff_t
forward_distance(bfc_citerptr_t first, bfc_citerptr_t last)
{
	if (bfc_iterator_equals(first, last)) {
		return (0);
	}
	if (last->obj == first->obj) {
		if (last->pos == BFC_NPOS) {
			return (bfc_object_length(last) - first->pos);
		}
		return (last->pos - first->pos);
	}
	return (0);
}

static void *
reverse_data(bfc_iterptr_t it, size_t pos)
{
	return (iterator_data(it, 0 - (ptrdiff_t) pos));
}

static int
advance_reverse(bfc_iterptr_t it, ptrdiff_t n)
{
	return (advance_forward(it, -n));
}

static ptrdiff_t
reverse_distance(bfc_citerptr_t first, bfc_citerptr_t last)
{
	if (bfc_iterator_equals(first, last)) {
		return (0);
	}
	if (last->obj == first->obj) {
		if (first->pos == BFC_NPOS) {
			return (last->pos - bfc_object_length(first));
		}
		return (last->pos - first->pos);
	}
	return (0);
}

