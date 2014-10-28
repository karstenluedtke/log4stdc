
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"
#include "log4stdc.h"

#ifndef STRING_CLASS_NAME
#define STRING_CLASS_NAME "shared_wstring"
#endif


extern struct bfc_classhdr bfc_wchar_traits_class;

struct bfc_shared_wstring {
	BFC_STRINGHDR(bfc_string_classptr_t, wchar_t)
};

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			bfc_strptr_t, bfc_cstrptr_t, wchar_t)
};

extern struct bfc_string_class bfc_wstring_class;

static int bfc_shared_string_illegal_method(bfc_cstrptr_t s, const char *meth);

int bfc_shared_wstring_assign_buffer(bfc_strptr_t s,
				const wchar_t *s2, size_t n);

static int bfc_shared_wstring_assign_fill(bfc_strptr_t s, size_t n, int c);
static int bfc_shared_wstring_append_buffer(bfc_strptr_t s,
				const wchar_t *s2, size_t n);
static int bfc_shared_wstring_append_fill(bfc_strptr_t s, size_t n, int c);
static int bfc_shared_wstring_push_back(bfc_strptr_t s, int c);
static int bfc_shared_wstring_insert_buffer(bfc_strptr_t s, size_t pos,
				const wchar_t *s2, size_t n);
static int bfc_shared_wstring_insert_fill(bfc_strptr_t s,
				size_t pos, size_t n, int c);
static int bfc_shared_wstring_insert_fillit(bfc_strptr_t s,
				bfc_iterptr_t p, size_t n, int c);
static int bfc_shared_wstring_insert_char(bfc_strptr_t s,
				bfc_iterptr_t p, int c);

void bfc_shared_wstring_pop_back(bfc_strptr_t s);

static int bfc_shared_wstring_replace_buffer(bfc_strptr_t s, size_t pos,
				size_t n1, const wchar_t* s2, size_t n2);
static int bfc_shared_wstring_replace_fill(bfc_strptr_t s,
				size_t pos, size_t n1, size_t n2, int c);
static int bfc_shared_wstring_replace_range_buffer(bfc_strptr_t s,
				bfc_iterptr_t i1, bfc_iterptr_t i2,
				const wchar_t* s2, size_t n);
static int bfc_shared_wstring_replace_range_fill(bfc_strptr_t s,
				bfc_iterptr_t i1, bfc_iterptr_t i2,
				size_t n, int c);
static int bfc_shared_wstring_replace_ranges(bfc_strptr_t s,
				bfc_iterptr_t i1, bfc_iterptr_t i2,
				bfc_iterptr_t j1, bfc_iterptr_t j2);

struct bfc_string_class bfc_shared_wstring_class = {
	.super 		= &bfc_wstring_class,
	.name 		= STRING_CLASS_NAME,
	.init 		= bfc_init_shared_wstring,
	.destroy 	= bfc_destroy_shared_wstring,
	.clonesize 	= bfc_shared_wstring_objsize,

	.traits		= (void *) &bfc_wchar_traits_class,

	.size 		= bfc_wstring_length,
	.max_size	= bfc_basic_wstring_max_size,
	.resize		= bfc_shared_wstring_resize,
	.capacity	= bfc_shared_wstring_capacity,
	.reserve	= bfc_shared_wstring_reserve,

	.assign_buffer	= bfc_shared_wstring_assign_buffer,
	.assign_fill 	= bfc_shared_wstring_assign_fill,
	.append_buffer	= bfc_shared_wstring_append_buffer,
	.append_fill 	= bfc_shared_wstring_append_fill,
	.push_back	= bfc_shared_wstring_push_back,
	.insert_buffer	= bfc_shared_wstring_insert_buffer,
	.insert_fill 	= bfc_shared_wstring_insert_fill,
	.insert_fillit	= bfc_shared_wstring_insert_fillit,
	.insert_char 	= bfc_shared_wstring_insert_char,
	.pop_back	= bfc_shared_wstring_pop_back,
	.replace_buffer	= bfc_shared_wstring_replace_buffer,
	.replace_fill	= bfc_shared_wstring_replace_fill,
	.replace_range_buffer = bfc_shared_wstring_replace_range_buffer,
	.replace_range_fill   = bfc_shared_wstring_replace_range_fill,
	.replace_ranges	= bfc_shared_wstring_replace_ranges,
};

int
bfc_init_shared_wstring(void *buf, size_t bufsize, struct mempool *pool)
{
	static const long zbuf = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_strptr_t, s, buf, bufsize, pool,
			  &bfc_shared_wstring_class);

	L4SC_WARN(logger, "%s(%p, %ld, pool %p): default constructor called!",
		__FUNCTION__, buf, (long) bufsize, pool);

	s->buf = (void *) &zbuf;
	return (BFC_SUCCESS);
}

int
bfc_init_shared_wstring_buffer(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s, size_t n)
{
	wchar_t *charbuf;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_strptr_t, obj, buf, bufsize, pool,
			  &bfc_shared_wstring_class);

	if (pool) {
		L4SC_WARN(logger, "%s(%p, %ld, pool %p, s %p, n %ld): pool!",
			__FUNCTION__, buf, (long) bufsize, pool, s, (long) n);
	} else {
		L4SC_TRACE(logger, "%s(%p, %ld, pool %p, s %p, n %ld)",
			__FUNCTION__, buf, (long) bufsize, pool, s, (long) n);
	}

	if (n >= bfc_basic_wstring_max_size(obj)) {
		L4SC_ERROR(logger, "%s: too large string: %lu >= %lu chars",
			__FUNCTION__, (unsigned long) n,
			(unsigned long) bfc_basic_wstring_max_size(obj));
		return (-EINVAL);
	}

	obj->buf = (void *) (intptr_t) s;
	obj->bufsize = obj->len = n;
	return (BFC_SUCCESS);
}

int
bfc_init_shared_wstring_c_str(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s)
{
	int n;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, s %p)",
		__FUNCTION__, buf, (long) bufsize, pool, s);

	n = (*bfc_shared_wstring_class.traits->szlen)(s);
	return bfc_init_shared_wstring_buffer(buf, bufsize, pool, s, n);
}

void
bfc_destroy_shared_wstring(bfc_strptr_t obj)
{
	bfc_string_classptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

size_t
bfc_shared_wstring_objsize(bfc_cstrptr_t obj)
{
	return (sizeof(struct bfc_shared_wstring));
}

// capacity:
int
bfc_shared_wstring_resize(bfc_strptr_t s, size_t n, int c)
{
	if (n <= s->len) {
		s->len = n;
		return (BFC_SUCCESS);
	} else {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s: cannot allocate %ld chars",
			__FUNCTION__, (long)n);
	}
	return (-ENOSPC);
}

size_t
bfc_shared_wstring_capacity(bfc_cstrptr_t s)
{
	return (s->len);
}

int
bfc_shared_wstring_reserve(bfc_strptr_t s, size_t n)
{
	wchar_t *p;

	if (n > bfc_basic_wstring_max_size(s)) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s: too large %ld", __FUNCTION__, (long)n);
		return (-EINVAL);
	} else if (n <= s->len) {
		return (BFC_SUCCESS);
	}
	return (-ENOMEM);
}

static int
bfc_shared_string_illegal_method(bfc_cstrptr_t s, const char *meth)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	L4SC_ERROR(logger, "illegal method %s called for string @%p", meth, s);
	return (-ENOSYS);
}

int
bfc_shared_wstring_assign_buffer(bfc_strptr_t s, const wchar_t *s2, size_t n)
{
	s->buf = (void *) (intptr_t) s;
	s->bufsize = s->len = n;
	return (BFC_SUCCESS);
}

static int
bfc_shared_wstring_assign_fill(bfc_strptr_t s, size_t n, int c)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}

static int
bfc_shared_wstring_append_buffer(bfc_strptr_t s, const wchar_t *s2, size_t n)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}

static int
bfc_shared_wstring_append_fill(bfc_strptr_t s, size_t n, int c)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}

static int
bfc_shared_wstring_push_back(bfc_strptr_t s, int c)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}

static int
bfc_shared_wstring_insert_buffer(bfc_strptr_t s, size_t pos,
			  const wchar_t *s2, size_t n)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}

static int
bfc_shared_wstring_insert_fill(bfc_strptr_t s, size_t pos, size_t n, int c)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}

static int
bfc_shared_wstring_insert_fillit(bfc_strptr_t s, bfc_iterptr_t p, size_t n, int c)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}

static int
bfc_shared_wstring_insert_char(bfc_strptr_t s, bfc_iterptr_t p, int c)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}

void
bfc_shared_wstring_pop_back(bfc_strptr_t s)
{
	if (bfc_wstrlen(s) > 0) {
		s->len--;
	}
}

static int
bfc_shared_wstring_replace_buffer(bfc_strptr_t s, size_t pos, size_t n1,
			   const wchar_t* s2, size_t n2)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}

static int
bfc_shared_wstring_replace_fill(bfc_strptr_t s, size_t pos, size_t n1,
					size_t n2, int c)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}

static int
bfc_shared_wstring_replace_range_buffer(bfc_strptr_t s, bfc_iterptr_t i1,
				bfc_iterptr_t i2, const wchar_t* s2, size_t n)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}

static int
bfc_shared_wstring_replace_range_fill(bfc_strptr_t s, bfc_iterptr_t i1,
					bfc_iterptr_t i2, size_t n, int c)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}

static int
bfc_shared_wstring_replace_ranges(bfc_strptr_t s,
				  bfc_iterptr_t i1, bfc_iterptr_t i2,
				  bfc_iterptr_t j1, bfc_iterptr_t j2)
{
	return (bfc_shared_string_illegal_method(s, __FUNCTION__));
}
