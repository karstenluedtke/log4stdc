
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
	static const long zbuf = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_basic_wstrptr_t, s, buf, bufsize, pool,
			  &bfc_basic_wstring_class);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p)",
		__FUNCTION__, buf, (long) bufsize, pool);

	s->buf = (wchar_t *) &zbuf;
	return (BFC_SUCCESS);
}

int
bfc_init_basic_wstring_bfstr(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cwstrptr_t str)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, str %p)",
		__FUNCTION__, buf, (long) bufsize, pool, str);
	rc = bfc_init_basic_wstring(buf, bufsize, pool);
	return (rc);
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
	
	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, str %p)",
		__FUNCTION__, buf, (long) bufsize, pool, str);
	rc = bfc_init_basic_wstring_buffer(buf, bufsize, pool,
		bfc_wstrdata(str) + pos, bfc_wstring_sublen(str, pos, n));
	return (rc);
}

int
bfc_init_basic_wstring_buffer(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s, size_t n)
{
	bfc_basic_wstrptr_t obj = (bfc_basic_wstrptr_t) buf;
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, s %p, n %ld)",
		__FUNCTION__, buf, (long) bufsize, pool, s, (long) n);

	if ((rc = bfc_init_basic_wstring(obj, bufsize, pool)) < 0) {
		return(rc);
	}
	obj->buf = (wchar_t *) (intptr_t) s;
	obj->bufsize = obj->len = n;
	return (rc);
}

int
bfc_init_basic_wstring_c_str(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s)
{
	bfc_basic_wstrptr_t obj = (bfc_basic_wstrptr_t) buf;
	int rc;

	if ((rc = bfc_init_basic_wstring(obj, bufsize, pool)) < 0) {
		return(rc);
	}
	obj->buf = (wchar_t *) (intptr_t) s;
	obj->bufsize = obj->len = (*obj->vptr->traits->szlen)(s);
	return (rc);
}

int
bfc_init_basic_wstring_fill(void *buf, size_t bufsize, struct mempool *pool,
				size_t n, wchar_t c)
{
	int rc;
	rc = bfc_init_basic_wstring(buf, bufsize, pool);
	return (rc);
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
	wchar_t *data = s->buf + s->offs;
	wchar_t *limit = s->buf + s->bufsize;
	if (n <= s->len) {
		s->len = n;
		data[n] = '\0';
		return ((int) n);
	} else if (data + n < limit) {
		(*s->vptr->traits->assign)(data + s->len, n - s->len, c);
		data[n] = '\0';
		return ((int) n);
	} else if (s->buf + n < limit) {
		data = limit - (n+1);
		s->offs = data - s->buf;
		(*s->vptr->traits->move)(data, s->buf + s->offs, s->len);
		(*s->vptr->traits->assign)(data + s->len, n - s->len, c);
		data[n] = '\0';
		return ((int) n);
	}
	return (-ENOSPC);
}

size_t
bfc_basic_wstring_capacity(bfc_basic_cwstrptr_t s)
{
	return ((s->bufsize != 0)? s->bufsize: s->len);
}

int
bfc_basic_wstring_reserve(bfc_basic_wstrptr_t s, size_t n)
{
	if (n >= bfc_basic_wstring_max_size(s)) {
		return (-EINVAL);
	}
	if (s->offs + n < s->bufsize) {
		return (BFC_SUCCESS);
	}
	if (s->pool) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
		size_t need = s->offs + n + 1;
		wchar_t *p = bfc_mempool_realloc(s->pool, s->buf,
						 need * sizeof(wchar_t));
		if (p == NULL) {
			L4SC_ERROR(logger,
				"%s: cannot allocated %ld chars of size %d",
				__FUNCTION__,(long)need, (int)sizeof(wchar_t));
			return (-ENOMEM);
		}
		L4SC_DEBUG(logger, "%s: allocated @%p: %ld chars of size %d",
			__FUNCTION__, p, (long) need, (int) sizeof(wchar_t));
		s->buf = p;
		s->bufsize = need;
		return (BFC_SUCCESS);
	}
	if (n < s->bufsize) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
		L4SC_DEBUG(logger, "%s: buf %p + %ld < limit %p",
			__FUNCTION__, s->buf, (long) n, s->buf + s->bufsize);
		wchar_t *p = s->buf + s->bufsize - (n+1);
		if (s->len > 0) {
			(*s->vptr->traits->move)(p, s->buf+s->offs, s->len);
		}
		p[s->len] = '\0';
		s->offs = p - s->buf;
		return (BFC_SUCCESS);
	}
	return (-ENOSYS);
}

