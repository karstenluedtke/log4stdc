
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
#define STRING_CLASS_NAME "basic_wstring"
#endif


extern struct bfc_classhdr bfc_wchar_traits_class;

struct bfc_basic_wstring {
	BFC_STRINGHDR(bfc_string_classptr_t, wchar_t)
};

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			bfc_strptr_t, bfc_cstrptr_t, wchar_t)
};

extern struct bfc_string_class bfc_wstring_class;

struct bfc_string_class bfc_basic_wstring_class = {
	.super 		= &bfc_wstring_class,
	.name 		= STRING_CLASS_NAME,
	.init 		= bfc_init_basic_wstring,
	.destroy 	= bfc_destroy_basic_wstring,
	.clonesize 	= bfc_basic_wstring_objsize,

	.traits		= (void *) &bfc_wchar_traits_class,

	.size 		= bfc_basic_wstring_length,
	.max_size	= bfc_basic_wstring_max_size,
	.resize		= bfc_basic_wstring_resize,
	.capacity	= bfc_basic_wstring_capacity,
	.reserve	= bfc_basic_wstring_reserve,
};

int
bfc_init_basic_wstring(void *buf, size_t bufsize, struct mempool *pool)
{
	wchar_t *charbuf;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_strptr_t, s, buf, bufsize, pool,
			  &bfc_basic_wstring_class);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p)",
		__FUNCTION__, buf, (long) bufsize, pool);

	if (pool == NULL) {
		L4SC_ERROR(logger, "%s: no pool", __FUNCTION__);
		return (-EFAULT);
	}
	if ((charbuf = bfc_mempool_alloc(pool, 56 * sizeof(wchar_t))) == NULL) {
		L4SC_ERROR(logger, "%s: no memory", __FUNCTION__);
		return (-ENOMEM);
	}
	s->pool = pool;
	s->buf  = charbuf;
	s->bufsize = 56;
	charbuf[0] = 0;
	return (BFC_SUCCESS);
}

int
bfc_init_basic_wstring_buffer(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s, size_t n)
{
	wchar_t *charbuf;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_strptr_t, obj, buf, bufsize, pool,
			  &bfc_basic_wstring_class);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, s %p, n %ld)",
		__FUNCTION__, buf, (long) bufsize, pool, s, (long) n);

	if (n >= bfc_basic_wstring_max_size(obj)) {
		L4SC_ERROR(logger, "%s: too large string: %lu >= %lu chars",
			__FUNCTION__, (unsigned long) n,
			(unsigned long) bfc_basic_wstring_max_size(obj));
		return (-EINVAL);
	}
	if (pool == NULL) {
		L4SC_ERROR(logger, "%s: no pool", __FUNCTION__);
		return (-EFAULT);
	}
	if ((charbuf = bfc_mempool_alloc(pool,(n+1)*sizeof(wchar_t))) == NULL) {
		L4SC_ERROR(logger, "%s: no memory for %ld chars",
					__FUNCTION__, (long)(n+1));
		return (-ENOMEM);
	}
	obj->pool = pool;
	obj->buf  = charbuf;
	obj->bufsize = (n+1);
	if (n > 0) {
		(*obj->vptr->traits->copy)(charbuf, s, n);
	}
	charbuf[n] = 0;
	obj->len = n;
	return (BFC_SUCCESS);
}

int
bfc_init_basic_wstring_c_str(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s)
{
	int n;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, s %p)",
		__FUNCTION__, buf, (long) bufsize, pool, s);

	n = (*bfc_basic_wstring_class.traits->szlen)(s);
	return bfc_init_basic_wstring_buffer(buf, bufsize, pool, s, n);
}

int
bfc_init_basic_wstring_fill(void *buf, size_t bufsize, struct mempool *pool,
				size_t n, int c)
{
	wchar_t *charbuf;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_strptr_t, obj, buf, bufsize, pool,
			  &bfc_basic_wstring_class);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, n %ld, c %02x)",
		__FUNCTION__, buf, (long) bufsize, pool, (long) n, c);

	if (n >= bfc_basic_wstring_max_size(obj)) {
		L4SC_ERROR(logger, "%s: too large string: %lu >= %lu chars",
			__FUNCTION__, (unsigned long) n,
			(unsigned long) bfc_basic_wstring_max_size(obj));
		return (-EINVAL);
	}
	if (pool == NULL) {
		L4SC_ERROR(logger, "%s: no pool", __FUNCTION__);
		return (-EFAULT);
	}
	if ((charbuf = bfc_mempool_alloc(pool,(n+1)*sizeof(wchar_t))) == NULL) {
		L4SC_ERROR(logger, "%s: no memory for %ld chars",
					__FUNCTION__, (long)(n+1));
		return (-ENOMEM);
	}
	obj->pool = pool;
	obj->buf  = charbuf;
	obj->bufsize = (n+1);
	if (n > 0) {
		(*obj->vptr->traits->assign)(charbuf, n, c);
	}
	charbuf[n] = 0;
	obj->len = n;
	return (BFC_SUCCESS);
}

void
bfc_destroy_basic_wstring(bfc_strptr_t obj)
{
	bfc_string_classptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		wchar_t *charbuf = obj->buf;
		struct mempool *pool;
		obj->len = obj->bufsize = 0;
		obj->buf = NULL;
		if (charbuf && (pool = obj->pool)) {
			mempool_free(pool, charbuf);
		}
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

size_t
bfc_basic_wstring_objsize(bfc_cstrptr_t obj)
{
	return (sizeof(struct bfc_basic_wstring));
}

// capacity:
size_t
bfc_basic_wstring_length(bfc_cstrptr_t s)
{
	return (s->len);
}

size_t
bfc_basic_wstring_max_size(bfc_cstrptr_t s)
{
	return ((0xFFF0uL << 7*(sizeof(s->bufsize)-2)) / sizeof(wchar_t));
}

int
bfc_basic_wstring_resize(bfc_strptr_t s, size_t n, int c)
{
	int rc = -ENOSPC;

	if (n <= s->len) {
		wchar_t *data = (wchar_t *)s->buf + s->offs;
		s->len = n;
		data[s->len] = '\0';
		return (BFC_SUCCESS);
	} else if ((rc = bfc_basic_wstring_reserve(s, n)) == BFC_SUCCESS) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_DEBUG(logger, "%s: got %ld chars", __FUNCTION__, (long)n);
		wchar_t *data = (wchar_t *)s->buf + s->offs;
		(*s->vptr->traits->assign)(data + s->len, n - s->len, c);
		s->len = n;
		data[n] = '\0';
		L4SC_DEBUG(logger, "%s(%p): %ld chars @%p",
			__FUNCTION__, s, (long) s->len, s->buf);
		return (BFC_SUCCESS);
	} else {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s: failed allocating %ld chars, err %d",
			__FUNCTION__, (long)n, rc);
	}
	return (rc);
}

size_t
bfc_basic_wstring_capacity(bfc_cstrptr_t s)
{
	if (s->bufsize > 0) {
		return (s->bufsize - 1);
	}
	return (0);
}

int
bfc_basic_wstring_reserve(bfc_strptr_t s, size_t n)
{
	wchar_t *p;
	size_t need;

	if (n > bfc_basic_wstring_max_size(s)) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s: too large %ld", __FUNCTION__, (long)n);
		return (-EINVAL);
	}
	need = s->offs + n + 1;
	if (need <= s->bufsize) {
		return (BFC_SUCCESS);
	}
	if (s->pool == NULL) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s: no pool", __FUNCTION__);
		return (-EFAULT);
	}
	p = bfc_mempool_realloc(s->pool, s->buf, need * sizeof(wchar_t));
	if (p == NULL) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s: cannot allocate %ld chars of size %d",
				__FUNCTION__,(long)need, (int)sizeof(wchar_t));
		return (-ENOMEM);
	}
	s->buf = p;
	s->bufsize = need;
	return (BFC_SUCCESS);
}

