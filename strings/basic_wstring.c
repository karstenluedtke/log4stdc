
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
#include "log4stdc.h"

#ifndef STRING_CLASS_NAME
#define STRING_CLASS_NAME "basic_wstring"
#endif

void *bfc_alloc_stringbuf(struct mempool *pool, size_t nbytes);
void *bfc_realloc_stringbuf(void *charbuf, size_t nbytes);
void  bfc_free_stringbuf(void *charbuf);

static int clone_string(bfc_cstrptr_t obj, void *buf, size_t bufsize,
						struct mempool *pool);

extern const struct bfc_classhdr bfc_wchar_traits_class;

struct bfc_basic_wstring {
	BFC_STRINGHDR(bfc_string_classptr_t, wchar_t)
};

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			bfc_strptr_t, bfc_cstrptr_t, wchar_t)
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

	.replace_ranges	= bfc_basic_string_replace_ranges,
};

#define GET_STRBUF(s)		BFC_UNCONST(wchar_t*,(s)->name)
#define SET_STRBUF(s,buf)	(s)->name = (const char *)(buf)

int
bfc_init_basic_wstring(void *buf, size_t bufsize, struct mempool *pool)
{
	wchar_t *charbuf;
	const unsigned n = 60 - 3*sizeof(void *);
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
	if ((charbuf = bfc_alloc_stringbuf(pool, n*sizeof(wchar_t))) == NULL) {
		L4SC_ERROR(logger, "%s: no memory", __FUNCTION__);
		return (-ENOMEM);
	}
	SET_STRBUF(s, charbuf);
	s->bufsize = n;
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
	if ((charbuf = bfc_alloc_stringbuf(pool,(n+1)*sizeof(wchar_t)))==NULL) {
		L4SC_ERROR(logger, "%s: no memory for %ld chars",
					__FUNCTION__, (long)(n+1));
		return (-ENOMEM);
	}
	SET_STRBUF(obj, charbuf);
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
	if ((charbuf = bfc_alloc_stringbuf(pool,(n+1)*sizeof(wchar_t)))==NULL) {
		L4SC_ERROR(logger, "%s: no memory for %ld chars",
					__FUNCTION__, (long)(n+1));
		return (-ENOMEM);
	}
	SET_STRBUF(obj, charbuf);
	obj->bufsize = (n+1);
	if (n > 0) {
		(*obj->vptr->traits->assign)(charbuf, n, c);
	}
	charbuf[n] = 0;
	obj->len = n;
	return (BFC_SUCCESS);
}

int
bfc_init_basic_wstring_move(void *buf, size_t bufsize, bfc_strptr_t str)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_strptr_t, obj, buf, bufsize, NULL,
			  &bfc_basic_wstring_class);

	L4SC_TRACE(logger, "%s(%p, %ld, str %p)",
		__FUNCTION__, buf, (long) bufsize, str);

	SET_STRBUF(obj, GET_STRBUF(str));
	obj->len = str->len;
	obj->bufsize = str->bufsize;
	str->bufsize = 0;
	str->len = 0;
	SET_STRBUF(str, NULL);
	return (BFC_SUCCESS);
}

void
bfc_destroy_basic_wstring(bfc_strptr_t obj)
{
	bfc_string_classptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		wchar_t *charbuf = GET_STRBUF(obj);
		obj->len = obj->bufsize = 0;
		SET_STRBUF(obj, NULL);
		if (charbuf) {
			bfc_free_stringbuf(charbuf);
		}
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

static int
clone_string(bfc_cstrptr_t obj, void *buf, size_t bufsize, struct mempool *pool)
{
	size_t len = bfc_wstring_length(obj);
	const wchar_t *p = bfc_wstring_data(obj);
	struct mempool *newpool = pool? pool: bfc_basic_string_pool(obj);
	return (bfc_init_basic_wstring_buffer(buf, bufsize, newpool, p, len));
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
		wchar_t *data = GET_STRBUF(s) + s->offs;
		s->len = n;
		data[s->len] = '\0';
		return (BFC_SUCCESS);
	} else if ((rc = bfc_basic_wstring_reserve(s, n)) == BFC_SUCCESS) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_DEBUG(logger, "%s: got %ld chars", __FUNCTION__, (long)n);
		wchar_t *data = GET_STRBUF(s) + s->offs;
		(*s->vptr->traits->assign)(data + s->len, n - s->len, c);
		s->len = n;
		data[n] = '\0';
		L4SC_DEBUG(logger, "%s(%p): %ld chars @%p",
			__FUNCTION__, s, (long) s->len, GET_STRBUF(s));
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
	p = bfc_realloc_stringbuf(GET_STRBUF(s), need * sizeof(wchar_t));
	if (p == NULL) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s: cannot allocate %ld chars of size %d",
				__FUNCTION__,(long)need, (int)sizeof(wchar_t));
		return (-ENOMEM);
	}
	SET_STRBUF(s, p);
	s->bufsize = need;
	return (BFC_SUCCESS);
}

