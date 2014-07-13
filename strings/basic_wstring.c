
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

#define iterptrT void *

#ifndef STRING_CLASS_NAME
#define STRING_CLASS_NAME "basic_wstring"
#endif

#define LOGGERNAME "barefootc.string", 16

extern struct bfc_classhdr bfc_wchar_traits_class;

struct bfc_basic_wstring {
	BFC_STRINGHDR(bfc_string_classptr_t, wchar_t)
	struct mempool *pool;
};

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			bfc_basic_wstrptr_t, bfc_basic_cwstrptr_t, wchar_t)
};

extern struct bfc_string_class bfc_wstring_class;

struct bfc_string_class bfc_basic_wstring_class = {
	.super 		= &bfc_wstring_class,
	.name 		= STRING_CLASS_NAME,
	.init 		= bfc_init_basic_wstring,
	.destroy 	= bfc_destroy_basic_wstring,
	.clonesize 	= bfc_basic_wstring_objsize,

	.traits		= (void *) &bfc_wchar_traits_class,

	.init_bfstr	= (void *) bfc_init_basic_wstring_bfstr,
	.init_move	= bfc_init_basic_wstring_move,
	.init_substr	= (void *) bfc_init_basic_wstring_substr,
	.init_buffer	= bfc_init_basic_wstring_buffer,
	.init_c_str	= bfc_init_basic_wstring_c_str,
	.init_fill	= bfc_init_basic_wstring_fill,
	.init_range	= bfc_init_basic_wstring_range,

	.size 		= bfc_basic_wstring_length,
	.max_size	= bfc_basic_wstring_max_size,
	.resize		= bfc_basic_wstring_resize,
	.capacity	= bfc_basic_wstring_capacity,
	.reserve	= bfc_basic_wstring_reserve,
};

int
bfc_init_basic_wstring(void *buf, size_t bufsize, struct mempool *pool)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_basic_wstrptr_t, s, buf, bufsize, pool,
			  &bfc_basic_wstring_class);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p)",
		__FUNCTION__, buf, (long) bufsize, pool);

	if (pool == NULL) {
		L4SC_ERROR(logger, "%s: no pool", __FUNCTION__);
		return (-EFAULT);
	}
	if ((s->buf = bfc_mempool_alloc(pool, 56 * sizeof(wchar_t))) == NULL) {
		L4SC_ERROR(logger, "%s: no memory", __FUNCTION__);
		return (-ENOMEM);
	}
	s->pool = pool;
	s->bufsize = 56;
	s->buf[0] = 0;
	return (BFC_SUCCESS);
}

int
bfc_init_basic_wstring_bfstr(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cwstrptr_t str)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, str %p)",
		__FUNCTION__, buf, (long) bufsize, pool, str);

	return bfc_init_basic_wstring_buffer(buf, bufsize, pool,
				bfc_wstrdata(str), bfc_wstrlen(str));
}

int
bfc_init_basic_wstring_move(void *buf, size_t bufsize, struct mempool *pool,
				bfc_basic_wstrptr_t str)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, str %p)",
		__FUNCTION__, buf, (long) bufsize, pool, str);
	rc = bfc_init_basic_wstring(buf, bufsize, pool);
	return (rc);
}

int
bfc_init_basic_wstring_substr(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cwstrptr_t str, size_t pos, size_t n)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, str %p, pos %ld, n %ld)",
		__FUNCTION__, buf, (long) bufsize, pool,
		str, (long) pos, (long) n);

	return bfc_init_basic_wstring_buffer(buf, bufsize, pool,
		bfc_wstrdata(str) + pos, bfc_wstring_sublen(str, pos, n));
}

int
bfc_init_basic_wstring_buffer(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s, size_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_basic_wstrptr_t, obj, buf, bufsize, pool,
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
	if ((obj->buf = bfc_mempool_alloc(pool,(n+1)*sizeof(wchar_t)))==NULL) {
		L4SC_ERROR(logger, "%s: no memory for %ld chars",
					__FUNCTION__, (long)(n+1));
		return (-ENOMEM);
	}
	obj->pool = pool;
	obj->bufsize = (n+1);
	if (n > 0) {
		(*obj->vptr->traits->copy)(obj->buf, s, n);
	}
	obj->buf[n] = 0;
	obj->len = n;
	return (BFC_SUCCESS);
}

int
bfc_init_basic_wstring_c_str(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s)
{
	int n;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, s %p)",
		__FUNCTION__, buf, (long) bufsize, pool, s);

	n = (*bfc_basic_wstring_class.traits->szlen)(s);
	return bfc_init_basic_wstring_buffer(buf, bufsize, pool, s, n);
}

int
bfc_init_basic_wstring_fill(void *buf, size_t bufsize, struct mempool *pool,
				size_t n, wchar_t c)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_basic_wstrptr_t, obj, buf, bufsize, pool,
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
	if ((obj->buf = bfc_mempool_alloc(pool,(n+1)*sizeof(wchar_t)))==NULL) {
		L4SC_ERROR(logger, "%s: no memory for %ld chars",
					__FUNCTION__, (long)(n+1));
		return (-ENOMEM);
	}
	obj->pool = pool;
	obj->bufsize = (n+1);
	if (n > 0) {
		(*obj->vptr->traits->assign)(obj->buf, n, c);
	}
	obj->buf[n] = 0;
	obj->len = n;
	return (BFC_SUCCESS);
}

int
bfc_init_basic_wstring_range(void *buf, size_t bufsize, struct mempool *pool,
				iterptrT begin, iterptrT end)
{
	int rc;
	rc = bfc_init_basic_wstring(buf, bufsize, pool);
	return (rc);
}

void
bfc_destroy_basic_wstring(bfc_basic_wstrptr_t obj)
{
	bfc_string_classptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

size_t
bfc_basic_wstring_objsize(bfc_basic_cwstrptr_t obj)
{
	return (sizeof(struct bfc_basic_wstring));
}

// capacity:
size_t
bfc_basic_wstring_length(bfc_basic_cwstrptr_t s)
{
	return (s->len);
}

size_t
bfc_basic_wstring_max_size(bfc_basic_cwstrptr_t s)
{
	return ((0x7FFFuL << 8*(sizeof(s->bufsize)-2)) / sizeof(wchar_t));
}

int
bfc_basic_wstring_resize(bfc_basic_wstrptr_t s, size_t n, wchar_t c)
{
	if (n <= s->len) {
		wchar_t *data = s->buf + s->offs;
		s->len = n;
		data[s->len] = '\0';
		return ((int) n);
	} else if (bfc_basic_wstring_reserve(s, n) == BFC_SUCCESS) {
		wchar_t *data = s->buf + s->offs;
		(*s->vptr->traits->assign)(data + s->len, n - s->len, c);
		data[n] = '\0';
		return ((int) n);
	}
	return (-ENOSPC);
}

size_t
bfc_basic_wstring_capacity(bfc_basic_cwstrptr_t s)
{
	return (s->bufsize);
}

int
bfc_basic_wstring_reserve(bfc_basic_wstrptr_t s, size_t n)
{
	wchar_t *p;
	size_t need;

	if (s->offs + n < s->bufsize) {
		return (BFC_SUCCESS);
	}

	if (n >= bfc_basic_wstring_max_size(s)) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
		L4SC_ERROR(logger, "%s: too large %ld", __FUNCTION__, (long)n);
		return (-EINVAL);
	}
	if (s->pool == NULL) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
		L4SC_ERROR(logger, "%s: no pool", __FUNCTION__);
		return (-EFAULT);
	}
	need = s->offs + n + 1;
	p = bfc_mempool_realloc(s->pool, s->buf, need * sizeof(wchar_t));
	if (p == NULL) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
		L4SC_ERROR(logger, "%s: cannot allocated %ld chars of size %d",
				__FUNCTION__,(long)need, (int)sizeof(wchar_t));
		return (-ENOMEM);
	}
	s->buf = p;
	s->bufsize = need;
	return (BFC_SUCCESS);
}
