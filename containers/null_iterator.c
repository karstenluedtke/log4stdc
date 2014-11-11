
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "log4stdc.h"

static int init_iterator(void *buf,size_t bufsize,struct mempool *pool);
static int iterator_equals(bfc_citerptr_t it, bfc_citerptr_t other);
static void dump_iterator(bfc_citerptr_t it,int depth,struct l4sc_logger *log);
static const void *iterator_first(bfc_citerptr_t);
static void *forward_index(bfc_iterptr_t, size_t pos);
static long forward_getchar(bfc_citerptr_t, size_t pos);
static int forward_setchar(bfc_iterptr_t, size_t pos, long val);
static int advance_forward(bfc_iterptr_t it, ptrdiff_t n);
static ptrdiff_t forward_distance(bfc_citerptr_t first, bfc_citerptr_t limit);

static int advance_reverse(bfc_iterptr_t it, ptrdiff_t n);
static ptrdiff_t reverse_distance(bfc_citerptr_t first, bfc_citerptr_t limit);

extern struct bfc_iterator_class bfc_forward_iterator_class;
extern struct bfc_iterator_class bfc_reverse_iterator_class;

#define FORWARD_CLASS_NAME			"null forward iterator"
#define REVERSE_CLASS_NAME			"null reverse iterator"

struct bfc_iterator_class bfc_null_forward_iterator_class = {
	.super	= &bfc_forward_iterator_class,
	.name	= FORWARD_CLASS_NAME,
	.init 	= init_iterator,
	.equals = iterator_equals,
	.dump	= dump_iterator,
	/* Element access */
	.first	= iterator_first,
	.index	= forward_index,
	.getl	= forward_getchar,
	.setl	= forward_setchar,
	/* Iterator functions */
	.advance    = advance_forward,
	.distance   = forward_distance,
};

struct bfc_iterator_class bfc_null_reverse_iterator_class = {
	.super	= &bfc_reverse_iterator_class,
	.name	= REVERSE_CLASS_NAME,
	.init 	= init_iterator,
	.equals = iterator_equals,
	.dump	= dump_iterator,
	/* Element access */
	.first	= iterator_first,
	.index	= forward_index,
	.getl	= forward_getchar,
	.setl	= forward_setchar,
	/* Iterator functions */
	.advance    = advance_reverse,
	.distance   = reverse_distance,
};

static int
init_iterator(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_iterptr_t it = (bfc_iterptr_t) buf;
	if (bufsize < sizeof(*it)) {
		return (-ENOSPC);
	}
	memset(it, 0, sizeof(*it));
	it->vptr = &bfc_null_forward_iterator_class;
	return (BFC_SUCCESS);
}

int
bfc_init_null_iterator(void *buf, size_t bufsize, size_t pos)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger("barefootc.string", 16);
	L4SC_TRACE(logger, "%s(%p, %ld, %ld)",
		__FUNCTION__, buf, (long) bufsize, (long) pos);

	if ((rc = init_iterator(buf, bufsize, NULL)) == BFC_SUCCESS) {
		bfc_iterptr_t it = (bfc_iterptr_t) buf;
		it->pos = pos;
	}
	bfc_object_dump(buf, 1, logger);
	return (rc);
}

int
bfc_init_null_reverse_iterator(void *buf, size_t bufsize, size_t pos)
{
	int rc;
	bfc_iterptr_t it = (bfc_iterptr_t) buf;
	l4sc_logger_ptr_t logger = l4sc_get_logger("barefootc.string", 16);
	L4SC_TRACE(logger, "%s(%p, %ld, %ld)",
		__FUNCTION__, buf, (long) bufsize, (long) pos);

	rc = bfc_init_null_iterator(buf, bufsize, pos);
	it->vptr = &bfc_null_reverse_iterator_class;
	if (pos == BFC_NPOS) {
		it->pos = pos; /* reverse end iterator */
	}
	bfc_object_dump(buf, 1, logger);
	return (rc);
}

static int
iterator_equals(bfc_citerptr_t it, bfc_citerptr_t other)
{
	if (it == other) {
		return (1);
	}
	if (it->pos == other->pos) {
		return (1);
	}
	return (0);
}

static void
dump_iterator(bfc_citerptr_t it, int depth, struct l4sc_logger *log)
{
	if (it && BFC_CLASS(it)) {
		L4SC_DEBUG(log, "%s @%p", BFC_CLASS(it)->name, it);
	}
}

static const void *
iterator_first(bfc_citerptr_t it)
{
	return (&it->obj);
}

static void *
forward_index(bfc_iterptr_t it, size_t pos)
{
	return (&it->obj);
}

static long
forward_getchar(bfc_citerptr_t it, size_t pos)
{
	return (0L);
}

static int
forward_setchar(bfc_iterptr_t it, size_t pos, long val)
{
	return (0);
}

static int
advance_forward(bfc_iterptr_t it, ptrdiff_t n)
{
	it->pos += n;
	return (0);
}

static ptrdiff_t
forward_distance(bfc_citerptr_t first, bfc_citerptr_t limit)
{
	return (limit->pos - first->pos);
}

static int
advance_reverse(bfc_iterptr_t it, ptrdiff_t n)
{
	it->pos -= n;
	return (0);
}

static ptrdiff_t
reverse_distance(bfc_citerptr_t first, bfc_citerptr_t limit)
{
	return (first->pos - limit->pos);
}

