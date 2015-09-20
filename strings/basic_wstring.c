
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"
#include "barefootc/unconst.h"
#include "string_private.h"
#include "log4stdc.h"

#ifndef STRING_CLASS_NAME
#define STRING_CLASS_NAME "basic_wstring"
#endif

void *bfc_alloc_stringbuf(bfc_mempool_t pool, size_t nbytes);
void *bfc_realloc_stringbuf(void *charbuf, size_t nbytes);
void  bfc_free_stringbuf(void *charbuf);

static int clone_string(bfc_cobjptr_t obj, void *buf, size_t bufsize,
						bfc_mempool_t pool);

extern const struct bfc_classhdr bfc_wchar_traits_class;

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			bfc_objptr_t, bfc_cobjptr_t, wchar_t)
};

extern const struct bfc_string_class bfc_wstring_class;

const struct bfc_string_class bfc_basic_wstring_class = {
	.super 		= &bfc_wstring_class,
	.name 		= STRING_CLASS_NAME,
	.init 		= bfc_init_basic_wstring,
	.destroy 	= bfc_destroy_basic_wstring,
	.clone	 	= clone_string,
	.clonesize 	= bfc_basic_wstring_objsize,
	.length 	= bfc_basic_wstring_length,

	.traits		= (void *) &bfc_wchar_traits_class,

	.max_size	= bfc_basic_wstring_max_size,
	.resize		= bfc_basic_wstring_resize,
	.capacity	= bfc_basic_wstring_capacity,
	.reserve	= bfc_basic_wstring_reserve,
	.getpool	= bfc_basic_string_pool,

	.replace_ranges	= bfc_basic_string_replace_ranges,
};

int
bfc_init_basic_wstring(void *buf, size_t bufsize, bfc_mempool_t pool)
{
	wchar_t *charbuf;
	const unsigned n = 60 - 3*sizeof(void *);
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_objptr_t, s, buf, bufsize, pool,
			  &bfc_basic_wstring_class);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p)",
		__FUNCTION__, buf, (long) bufsize, pool);

	if (pool == NULL) {
		L4SC_ERROR(logger, "%s: no pool", __FUNCTION__);
		return (-EFAULT);
	}
	if ((charbuf = bfc_alloc_stringbuf(pool, n*sizeof(wchar_t))) == NULL) {
		L4SC_ERROR(logger, "%s: no memory", __FUNCTION__);
		return (-ENOMEM);
	}
	SET_STRBUF(s, charbuf);
	STRING_BUFSIZE(s) = n;
	charbuf[0] = 0;
	return (BFC_SUCCESS);
}

int
bfc_init_basic_wstring_buffer(void *buf, size_t bufsize, bfc_mempool_t pool,
				const wchar_t* s, size_t n)
{
	wchar_t *charbuf;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_objptr_t, obj, buf, bufsize, pool,
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
	if ((charbuf = bfc_alloc_stringbuf(pool,(n+1)*sizeof(wchar_t)))==NULL) {
		L4SC_ERROR(logger, "%s: no memory for %ld chars",
					__FUNCTION__, (long)(n+1));
		return (-ENOMEM);
	}
	SET_STRBUF(obj, charbuf);
	STRING_BUFSIZE(obj) = (n+1);
	if (n > 0) {
		(*STRING_TRAITS(obj)->copy)(charbuf, s, n);
	}
	charbuf[n] = 0;
	STRING_LEN(obj) = n;
	return (BFC_SUCCESS);
}

int
bfc_init_basic_wstring_c_str(void *buf, size_t bufsize, bfc_mempool_t pool,
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
bfc_init_basic_wstring_fill(void *buf, size_t bufsize, bfc_mempool_t pool,
				size_t n, int c)
{
	wchar_t *charbuf;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_objptr_t, obj, buf, bufsize, pool,
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
	if ((charbuf = bfc_alloc_stringbuf(pool,(n+1)*sizeof(wchar_t)))==NULL) {
		L4SC_ERROR(logger, "%s: no memory for %ld chars",
					__FUNCTION__, (long)(n+1));
		return (-ENOMEM);
	}
	SET_STRBUF(obj, charbuf);
	STRING_BUFSIZE(obj) = (n+1);
	if (n > 0) {
		(*STRING_TRAITS(obj)->assign)(charbuf, n, c);
	}
	charbuf[n] = 0;
	STRING_LEN(obj) = n;
	return (BFC_SUCCESS);
}

int
bfc_init_basic_wstring_move(void *buf, size_t bufsize, bfc_objptr_t str)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_objptr_t, obj, buf, bufsize, NULL,
			  &bfc_basic_wstring_class);

	L4SC_TRACE(logger, "%s(%p, %ld, str %p)",
		__FUNCTION__, buf, (long) bufsize, str);

	SET_STRBUF(obj, GET_STRBUF(str));
	STRING_LEN(obj) = STRING_LEN(str);
	STRING_BUFSIZE(obj) = STRING_BUFSIZE(str);
	STRING_BUFSIZE(str) = 0;
	STRING_LEN(str) = 0;
	SET_STRBUF(str, NULL);
	return (BFC_SUCCESS);
}

void
bfc_destroy_basic_wstring(bfc_objptr_t obj)
{
	bfc_classptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		wchar_t *charbuf = GET_STRBUF(obj);
		STRING_LEN(obj) = STRING_BUFSIZE(obj) = 0;
		SET_STRBUF(obj, NULL);
		if (charbuf) {
			bfc_free_stringbuf(charbuf);
		}
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

static int
clone_string(bfc_cobjptr_t obj, void *buf, size_t bufsize, bfc_mempool_t pool)
{
	size_t len = bfc_wstring_length(obj);
	const wchar_t *p = bfc_wstring_data(obj);
	bfc_mempool_t newpool = pool? pool: bfc_basic_string_pool(obj);
	return (bfc_init_basic_wstring_buffer(buf, bufsize, newpool, p, len));
}

size_t
bfc_basic_wstring_objsize(bfc_cobjptr_t obj)
{
	return (sizeof(bfc_string_t));
}

// capacity:
size_t
bfc_basic_wstring_length(bfc_cobjptr_t s)
{
	return (STRING_LEN(s));
}

size_t
bfc_basic_wstring_max_size(bfc_cobjptr_t s)
{
	return ((0xFFF0uL << 7*(sizeof(STRING_BUFSIZE(s))-2)) / sizeof(wchar_t));
}

int
bfc_basic_wstring_resize(bfc_objptr_t s, size_t n, int c)
{
	int rc = -ENOSPC;

	if (n <= STRING_LEN(s)) {
		wchar_t *data = GET_STRBUF(s) + STRING_OFFSET(s);
		STRING_LEN(s) = n;
		data[STRING_LEN(s)] = '\0';
		return (BFC_SUCCESS);
	} else if ((rc = bfc_basic_wstring_reserve(s, n)) == BFC_SUCCESS) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_DEBUG(logger, "%s: got %ld chars", __FUNCTION__, (long)n);
		size_t oldlen = STRING_LEN(s);
		wchar_t *data = GET_STRBUF(s) + STRING_OFFSET(s);
		(*STRING_TRAITS(s)->assign)(data + oldlen, n - oldlen, c);
		STRING_LEN(s) = n;
		data[n] = '\0';
		L4SC_DEBUG(logger, "%s(%p): %ld chars @%p",
			__FUNCTION__, s, (long) STRING_LEN(s), GET_STRBUF(s));
		return (BFC_SUCCESS);
	} else {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s: failed allocating %ld chars, err %d",
			__FUNCTION__, (long)n, rc);
	}
	return (rc);
}

size_t
bfc_basic_wstring_capacity(bfc_cobjptr_t s)
{
	if (STRING_BUFSIZE(s) > 0) {
		return (STRING_BUFSIZE(s) - 1);
	}
	return (0);
}

int
bfc_basic_wstring_reserve(bfc_objptr_t s, size_t n)
{
	wchar_t *p;
	size_t need;

	if (n > bfc_basic_wstring_max_size(s)) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s: too large %ld", __FUNCTION__, (long)n);
		return (-EINVAL);
	}
	need = STRING_OFFSET(s) + n + 1;
	if (need <= STRING_BUFSIZE(s)) {
		return (BFC_SUCCESS);
	}
	p = bfc_realloc_stringbuf(GET_STRBUF(s), need * sizeof(wchar_t));
	if (p == NULL) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s: cannot allocate %ld chars of size %d",
				__FUNCTION__,(long)need, (int)sizeof(wchar_t));
		return (-ENOMEM);
	}
	SET_STRBUF(s, p);
	STRING_BUFSIZE(s) = need;
	return (BFC_SUCCESS);
}

