
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>

#include <inttypes.h>

#if defined(_MSC_VER)
#define inline
#endif

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "log4stdc.h"

static int init_iterator(void *buf,size_t bufsize,struct mempool *pool);
static int iterator_equals(bfc_citerptr_t it, bfc_citerptr_t other);
static void dump_iterator(bfc_citerptr_t it,int depth,struct l4sc_logger *log);

static size_t element_size(bfc_citerptr_t);
static const void *iterator_first(bfc_citerptr_t);
static void *forward_index(bfc_iterptr_t, size_t pos);
static long forward_getchar(bfc_citerptr_t, size_t pos);
static int advance_forward(bfc_iterptr_t it, ptrdiff_t n);
static ptrdiff_t forward_distance(bfc_citerptr_t first, bfc_citerptr_t limit);

static void *reverse_index(bfc_iterptr_t it, size_t pos);
static long reverse_getchar(bfc_citerptr_t it, size_t pos);
static int advance_reverse(bfc_iterptr_t it, ptrdiff_t n);
static ptrdiff_t reverse_distance(bfc_citerptr_t first, bfc_citerptr_t limit);

extern const struct bfc_iterator_class bfc_forward_iterator_class;
extern const struct bfc_iterator_class bfc_reverse_iterator_class;

#ifndef FORWARD_CLASS_NAME
#define FORWARD_CLASS_NAME			"wstring forward iterator"
#define REVERSE_CLASS_NAME			"wstring reverse iterator"
#define IMPLEMENT_DUMP 1
#endif

const struct bfc_iterator_class bfc_wstr_forward_iterator_class = {
	.super	= &bfc_forward_iterator_class,
	.name	= FORWARD_CLASS_NAME,
	.init 	= init_iterator,
	.equals = iterator_equals,
	.dump	= dump_iterator,
	/* Element access */
	.first	= iterator_first,
	.index	= forward_index,
	.getl	= forward_getchar,
	.element_size = element_size,
	/* Iterator functions */
	.advance    = advance_forward,
	.distance   = forward_distance,
};

const struct bfc_iterator_class bfc_wstr_reverse_iterator_class = {
	.super	= &bfc_reverse_iterator_class,
	.name	= REVERSE_CLASS_NAME,
	.init 	= init_iterator,
	.equals = iterator_equals,
	.dump	= dump_iterator,
	/* Element access */
	.first	= iterator_first,
	.index	= reverse_index,
	.getl	= reverse_getchar,
	.element_size = element_size,
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
	it->vptr = &bfc_wstr_forward_iterator_class;
	return (BFC_SUCCESS);
}

int
bfc_init_wstr_iterator(void *buf, size_t bufsize, const wchar_t *s, size_t pos)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger("barefootc.string", 16);
	L4SC_TRACE(logger, "%s(%p, %ld, %p, %ld)",
		__FUNCTION__, buf, (long) bufsize, s, (long) pos);

	if ((rc = init_iterator(buf, bufsize, NULL)) == BFC_SUCCESS) {
		bfc_iterptr_t it = (bfc_iterptr_t) buf;
		size_t len = wcslen(s);
		it->obj = (bfc_objptr_t) (uintptr_t) s;
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
bfc_init_wstr_reverse_iterator(void *buf, size_t bufsize,
				const wchar_t *s, size_t pos)
{
	int rc;
	bfc_iterptr_t it = (bfc_iterptr_t) buf;
	l4sc_logger_ptr_t logger = l4sc_get_logger("barefootc.string", 16);
	L4SC_TRACE(logger, "%s(%p, %ld, %p, %ld)",
		__FUNCTION__, buf, (long) bufsize, s, (long) pos);

	rc = bfc_init_wstr_iterator(buf, bufsize, s, pos);
	it->vptr = &bfc_wstr_reverse_iterator_class;
	if (pos == BFC_NPOS) {
		it->pos = pos; /* reverse end iterator */
	}
	bfc_object_dump(buf, 1, logger);
	return (rc);
}

static int
iterator_equals(bfc_citerptr_t it, bfc_citerptr_t other)
{
	wchar_t *s = (wchar_t *) it->obj;
	if (it == other) {
		return (1);
	}
	if (s + it->pos == ((wchar_t *)other->obj) + other->pos) {
		return (1);
	}
	if (it->obj == other->obj) {
		if (it->pos == other->pos) {
			return (1);
		}
		if ((other->pos >= wcslen((wchar_t *)other->obj)) /* NPOS */
		 && (it->pos >= wcslen((wchar_t *)it->obj))) {
			return (1);
		}
	}
	return (0);
}

#ifdef IMPLEMENT_DUMP
static void
dump_iterator(bfc_citerptr_t it, int depth, struct l4sc_logger *log)
{
	if (it && BFC_CLASS(it)) {
		L4SC_DEBUG(log, "%s @%p", BFC_CLASS(it)->name, it);
		L4SC_DEBUG(log, "pos %ld in wchar string @%p",
				(long) it->pos, it->obj);
	}
}
#endif

static size_t
element_size(bfc_citerptr_t it)
{
	return (sizeof(wchar_t));
}

static const void *
iterator_first(bfc_citerptr_t it)
{
	wchar_t *s = (wchar_t *) it->obj;
	return (s + it->pos);
}

static inline void *
iterator_index(bfc_citerptr_t it, ptrdiff_t n)
{
	wchar_t *s = (wchar_t *) it->obj;
	if (n == 0) {
		return (s + it->pos);
	} else if (n > 0) {
		size_t objlen = wcslen((wchar_t *)it->obj);
		if (it->pos + n < objlen) {
			return (s + it->pos + n);
		}
	} else if (it->pos >= (-n)) {
		return (s + it->pos + n);
	}
	return (NULL);
}

static void *
forward_index(bfc_iterptr_t it, size_t pos)
{
	return (iterator_index(it, (ptrdiff_t) pos));
}

static long
forward_getchar(bfc_citerptr_t it, size_t pos)
{
	wchar_t *cp;
	if ((cp = iterator_index(it, (ptrdiff_t) pos)) == NULL) {
		return (-ERANGE);
	}
	return (((long) *cp) & ~(~0uL << 8*sizeof(*cp)));
}

static int
advance_forward(bfc_iterptr_t it, ptrdiff_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger("barefootc.string", 16);
	L4SC_TRACE(logger, "%s(%p, %ld)", __FUNCTION__, it, (long) n);

	if (n > 0) {
		size_t objlen = wcslen((wchar_t *)it->obj);
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
	bfc_iterator_dump(it, 1, logger);
	return (BFC_SUCCESS);
}

static ptrdiff_t
forward_distance(bfc_citerptr_t first, bfc_citerptr_t limit)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger("barefootc.string", 16);
	if (bfc_iterator_equals(first, limit)) {
		L4SC_DEBUG(logger, "%s: equals: 0", __FUNCTION__);
		return (0);
	}
	if (limit->obj == first->obj) {
		if (limit->pos == BFC_NPOS) {
			return (wcslen((wchar_t *) limit->obj) - first->pos);
		}
		L4SC_DEBUG(logger, "%s: same string: %ld",
			__FUNCTION__, (long) (limit->pos - first->pos));
		return (limit->pos - first->pos);
	} else {
		wchar_t *s1 = (wchar_t *) first->obj;
		wchar_t *s2 = (wchar_t *) limit->obj;
		L4SC_DEBUG(logger, "%s: different string: %ld", __FUNCTION__,
			(long) ((s2 + limit->pos) - (s1 + first->pos)));
		return ((s2 + limit->pos) - (s1 + first->pos));
	}
	return (0);
}

static void *
reverse_index(bfc_iterptr_t it, size_t pos)
{
	return (iterator_index(it, 0 - (ptrdiff_t) pos));
}

static long
reverse_getchar(bfc_citerptr_t it, size_t pos)
{
	wchar_t *cp;
	if ((cp = iterator_index(it, 0 - (ptrdiff_t) pos)) == NULL) {
		return (-ERANGE);
	}
	return (((long) *cp) & ~(~0uL << 8*sizeof(*cp)));
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
			return (wcslen((wchar_t *) first->obj) - limit->pos);
		}
		if (limit->pos == BFC_NPOS) {
			return (first->pos + 1);
		}
		return (first->pos - limit->pos);
	}
	return (0);
}

