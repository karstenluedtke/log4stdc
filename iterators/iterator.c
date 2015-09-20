
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <inttypes.h>

#if defined(_MSC_VER)
#define snprintf _snprintf
#define inline
#endif

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "log4stdc.h"

static int default_init_iterator(void *buf,size_t bufsize,struct mempool *pool);
static int init_refcount(bfc_iterptr_t it, int n);
static int incr_refcount(bfc_iterptr_t it);
static int decr_refcount(bfc_iterptr_t it);
static int clone_iterator(bfc_citerptr_t it,
		void *buf, size_t bufsize, struct mempool *pool);
static int iterator_equals(bfc_citerptr_t it, bfc_citerptr_t other);
static unsigned bfc_iterator_hashcode(bfc_citerptr_t it, int hashlen);
static void dump_iterator(bfc_citerptr_t it,int depth,struct l4sc_logger *log);
static int iterator_tostring(bfc_citerptr_t it,
			     char *buf, size_t bufsize, const char *fmt);

static size_t element_size(bfc_citerptr_t);
static const void *iterator_first(bfc_citerptr_t);
static void *forward_index(bfc_iterptr_t, size_t pos);
static long forward_getlong(bfc_citerptr_t, size_t pos);
static int forward_setlong(bfc_iterptr_t, size_t pos, long val);
static int advance_forward(bfc_iterptr_t it, ptrdiff_t n);
static ptrdiff_t forward_distance(bfc_citerptr_t first, bfc_citerptr_t limit);

static void *reverse_index(bfc_iterptr_t it, size_t pos);
static long reverse_getlong(bfc_citerptr_t it, size_t pos);
static int reverse_setlong(bfc_iterptr_t it, size_t pos, long val);
static int advance_reverse(bfc_iterptr_t it, ptrdiff_t n);
static ptrdiff_t reverse_distance(bfc_citerptr_t first, bfc_citerptr_t limit);
static void last_method(void) { }

const struct bfc_iterator_class bfc_forward_iterator_class = {
	/* intentionally not using selective initialization for base class: */
	/* I want the compiler to complain if something is missing.         */
	/* .super 	*/ NULL,
	/* .name 	*/ "forward iterator",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ default_init_iterator,
	/* .initrefc 	*/ init_refcount,
	/* .incrrefc 	*/ incr_refcount,
	/* .decrrefc 	*/ decr_refcount,
	/* .destroy 	*/ bfc_destroy_iterator,
	/* .clone 	*/ clone_iterator,
	/* .clonesize 	*/ bfc_iterator_objsize,
	/* .hashcode 	*/ bfc_iterator_hashcode,
	/* .equals 	*/ iterator_equals,
	/* .length 	*/ bfc_iterator_length,
	/* .tostring 	*/ iterator_tostring,
	/* .dump 	*/ dump_iterator,
	/* Element access */
	/* .first	*/ iterator_first,
	/* .index	*/ forward_index,
	/* .getl	*/ forward_getlong,
	/* .setl	*/ forward_setlong,
	/* .spare17 	*/ NULL,
	/* .ibegin	*/ NULL,
	/* .ilimit	*/ NULL,
	/* .rbegin	*/ NULL,
	/* .rlimit	*/ NULL,
	/* .max_size	*/ NULL,
	/* .element_size*/ element_size,
	/* .capacity	*/ NULL,
	/* .reserve	*/ NULL,
	/* .spare29 	*/ NULL,
	/* .spare30 	*/ NULL,
	/* .spare31 	*/ NULL,
	/* Additional constructors */
	/* .initialize	*/ bfc_init_iterator,
	/* Iterator functions */
	/* .advance	*/ advance_forward,
	/* .distance	*/ forward_distance,
	/* .last_method	*/ last_method
};

const struct bfc_iterator_class bfc_reverse_iterator_class = {
	/* intentionally not using selective initialization for base class: */
	/* I want the compiler to complain if something is missing.         */
	/* .super 	*/ &bfc_forward_iterator_class,
	/* .name 	*/ "reverse iterator",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ default_init_iterator,
	/* .initrefc 	*/ init_refcount,
	/* .incrrefc 	*/ incr_refcount,
	/* .decrrefc 	*/ decr_refcount,
	/* .destroy 	*/ bfc_destroy_iterator,
	/* .clone 	*/ clone_iterator,
	/* .clonesize 	*/ bfc_iterator_objsize,
	/* .hashcode 	*/ bfc_iterator_hashcode,
	/* .equals 	*/ iterator_equals,
	/* .length 	*/ bfc_iterator_length,
	/* .tostring 	*/ iterator_tostring,
	/* .dump 	*/ dump_iterator,
	/* Element access */
	/* .first	*/ iterator_first,
	/* .index	*/ reverse_index,
	/* .getl	*/ reverse_getlong,
	/* .setl	*/ reverse_setlong,
	/* .spare17 	*/ NULL,
	/* .ibegin	*/ NULL,
	/* .ilimit	*/ NULL,
	/* .rbegin	*/ NULL,
	/* .rlimit	*/ NULL,
	/* .max_size	*/ NULL,
	/* .element_size*/ element_size,
	/* .capacity	*/ NULL,
	/* .reserve	*/ NULL,
	/* .spare29 	*/ NULL,
	/* .spare30 	*/ NULL,
	/* .spare31 	*/ NULL,
	/* Additional constructors */
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
	l4sc_logger_ptr_t logger = l4sc_get_logger("barefootc.string", 16);
	L4SC_TRACE(logger, "%s(%p, %ld, %p, %ld)",
		__FUNCTION__, buf, (long) bufsize, obj, (long) pos);

	if ((rc = default_init_iterator(buf, bufsize, NULL)) == BFC_SUCCESS) {
		bfc_iterptr_t it = (bfc_iterptr_t) buf;
		size_t len = (obj == NULL)? 0: bfc_object_length(obj);
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
	bfc_object_dump(buf, 1, logger);
	return (rc);
}

int
bfc_init_reverse_iterator(void *buf,size_t bufsize,bfc_cobjptr_t obj,size_t pos)
{
	int rc;
	bfc_iterptr_t it = (bfc_iterptr_t) buf;

	rc = bfc_init_iterator(buf, bufsize, obj, pos);
	it->vptr = &bfc_reverse_iterator_class;
	if (pos == BFC_NPOS) {
		it->pos = pos; /* reverse end iterator */
	}
	return (rc);
}

static int init_refcount(bfc_iterptr_t it, int n)
{
	/* no refcount */
	return (-ENOSYS);
}

static int incr_refcount(bfc_iterptr_t it)
{
	/* no refcount */
	return (-ENOSYS);
}

static int decr_refcount(bfc_iterptr_t it)
{
	/* no refcount */
	return (-ENOSYS);
}

void
bfc_destroy_iterator(bfc_iterptr_t it)
{
	it->vptr = NULL;
}

int
bfc_clone_iterator(bfc_citerptr_t obj, void *buf, size_t bufsize)
{
	bfc_iterptr_t iter = (bfc_iterptr_t) buf;
	size_t size = bfc_object_size((bfc_cobjptr_t) obj);
	if (bufsize < size) {
		return (-ENOSPC);
	}
	memcpy(iter, obj, size);
	return (BFC_SUCCESS);
}

static int
clone_iterator(bfc_citerptr_t it,
		void *buf, size_t bufsize, struct mempool *pool)
{
	return (bfc_clone_iterator(it, buf, bufsize));
}

size_t
bfc_iterator_objsize(bfc_citerptr_t it)
{
	return (sizeof(struct bfc_iterator));
}

static size_t
element_size(bfc_citerptr_t it)
{
	if (it->obj) {
		RETURN_METHCALL(bfc_classptr_t, it->obj,
				element_size, (it->obj), 0);
	}
	return (0);
}

static unsigned  
bfc_iterator_hashcode(bfc_citerptr_t it, int hashlen)
{
	const char *p = (const char *) it->obj;
	size_t x = (size_t) (p + it->pos);
	return (bfc_reduce_hashcode(x, 8*sizeof(x), hashlen));
}

static int
iterator_equals(bfc_citerptr_t it, bfc_citerptr_t other)
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

static int
iterator_tostring(bfc_citerptr_t it, char *buf, size_t bufsize, const char *fmt)
{
	if (it && BFC_CLASS(it)) {
		snprintf(buf, bufsize, "%s @%p: %p[%ld]",
			 BFC_CLASS(it)->name, it, it->obj, (long) it->pos);
	}
	return (0);
}

static void
dump_iterator(bfc_citerptr_t it, int depth, struct l4sc_logger *log)
{
	if (it && BFC_CLASS(it)) {
		L4SC_DEBUG(log, "%s @%p", BFC_CLASS(it)->name, it);
		L4SC_DEBUG(log, "pos %ld in %p: %s", (long) it->pos, it->obj,
				(it->obj && it->obj->name)? it->obj->name: "");
	}
}

static const void *
iterator_first(bfc_citerptr_t it)
{
	RETURN_METHCALL(bfc_classptr_t, it->obj,
			index, (it->obj, it->pos), NULL);
}

static inline void *
iterator_index(bfc_iterptr_t it, ptrdiff_t n)
{
	if (n == 0) {
		RETURN_METHCALL(bfc_classptr_t, it->obj,
				index, (it->obj, it->pos), NULL);
	} else if (n > 0) {
		size_t objlen = bfc_object_length(it->obj);
		if (it->pos + n < objlen) {
			RETURN_METHCALL(bfc_classptr_t, it->obj,
					index, (it->obj, it->pos + n), NULL);
		}
	} else if (it->pos >= (-n)) {
		RETURN_METHCALL(bfc_classptr_t, it->obj,
				index, (it->obj, it->pos + n), NULL);
	}
	return (NULL);
}

static void *
forward_index(bfc_iterptr_t it, size_t pos)
{
	return (iterator_index(it, (ptrdiff_t) pos));
}

static long
forward_getlong(bfc_citerptr_t it, size_t pos)
{
	RETURN_METHCALL(bfc_classptr_t, it->obj,
			getl, (it->obj, it->pos + pos), -ENOSYS);
}

static int
forward_setlong(bfc_iterptr_t it, size_t pos, long val)
{
	RETURN_METHCALL(bfc_classptr_t, it->obj,
			setl, (it->obj, it->pos + pos, val), -ENOSYS);
}

static int
advance_forward(bfc_iterptr_t it, ptrdiff_t n)
{
	//l4sc_logger_ptr_t logger = l4sc_get_logger("barefootc.string", 16);
	//L4SC_TRACE(logger, "%s(%p, %ld)", __FUNCTION__, it, (long) n);

	if (n > 0) {
		size_t objlen = bfc_object_length(it->obj);
		if (it->pos == BFC_NPOS) {
			it->pos = (n <= objlen)? (n-1): objlen;
		} else if (it->pos + n <= objlen) {
			it->pos += n;
		} else {
			it->pos = objlen;
		}
	} else if (n < 0) {
		if ((it->pos != BFC_NPOS) && (it->pos >= (-n))) {
			it->pos += n;
		} else {
			it->pos = BFC_NPOS;
		}
	}
	//bfc_object_dump(it, 1, logger);
	return (BFC_SUCCESS);
}

static ptrdiff_t
forward_distance(bfc_citerptr_t first, bfc_citerptr_t limit)
{
	if (bfc_iterator_equals(first, limit)) {
		return (0);
	}
	if (limit->obj == first->obj) {
		if (limit->pos == BFC_NPOS) {
			return (bfc_object_length(limit->obj) - first->pos);
		}
		return (limit->pos - first->pos);
	}
	return (0);
}

static void *
reverse_index(bfc_iterptr_t it, size_t pos)
{
	return (iterator_index(it, 0 - (ptrdiff_t) pos));
}

static long
reverse_getlong(bfc_citerptr_t it, size_t pos)
{
	RETURN_METHCALL(bfc_classptr_t, it->obj,
			getl, (it->obj, it->pos - pos), -ENOSYS);
}

static int
reverse_setlong(bfc_iterptr_t it, size_t pos, long val)
{
	RETURN_METHCALL(bfc_classptr_t, it->obj,
			setl, (it->obj, it->pos - pos, val), -ENOSYS);
}

static int
advance_reverse(bfc_iterptr_t it, ptrdiff_t n)
{
	return (advance_forward(it, -n));
}

static ptrdiff_t
reverse_distance(bfc_citerptr_t first, bfc_citerptr_t limit)
{
	if (bfc_iterator_equals(first, limit)) {
		return (0);
	}
	if (limit->obj == first->obj) {
		if (first->pos == BFC_NPOS) {
			return (bfc_object_length(first->obj) - limit->pos);
		}
		if (limit->pos == BFC_NPOS) {
			return (first->pos + 1);
		}
		return (first->pos - limit->pos);
	}
	return (0);
}

size_t
bfc_iterator_position(bfc_citerptr_t it)
{
	return (it->pos);
}

size_t
bfc_iterator_set_position(bfc_iterptr_t it, size_t newpos)
{
	size_t oldpos = it->pos;

	it->pos = newpos;
	return (oldpos);
}

ptrdiff_t
bfc_iterator_distance(bfc_citerptr_t first, bfc_citerptr_t limit)
{
	RETURN_METHCALL(bfc_iterator_classptr_t, first,
			distance, (first, limit), 0);
}

int
bfc_iterator_equals(bfc_citerptr_t it, bfc_citerptr_t other)
{
	RETURN_METHCALL(bfc_iterator_classptr_t, it,
			equals, (it, other), 0);
}

int
bfc_iterator_advance(bfc_iterptr_t it, ptrdiff_t n)
{
	RETURN_METHCALL(bfc_iterator_classptr_t, it,
			advance, (it, n), -ENOSYS);
}

void
bfc_iterator_dump(bfc_citerptr_t it, int depth, struct l4sc_logger *log)
{
	VOID_METHCALL(bfc_iterator_classptr_t, it, dump, (it, depth, log));
}
