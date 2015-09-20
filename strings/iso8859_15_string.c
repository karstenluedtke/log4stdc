
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <inttypes.h>

#define NEED_BFC_STRING_CLASS 1

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"
#include "string_private.h"
#include "log4stdc.h"

extern const struct bfc_classhdr bfc_iso8859_15_traits_class;

static long get_codept(bfc_cobjptr_t s, size_t pos);
static int set_codept(bfc_objptr_t s, size_t pos, long c);
static int init_begin_iterator(bfc_cobjptr_t s, bfc_iterptr_t it, size_t bs);
static int init_limit_iterator(bfc_cobjptr_t s, bfc_iterptr_t it, size_t bs);
static int init_rbegin_iterator(bfc_cobjptr_t s, bfc_iterptr_t it, size_t bs);
static int init_rlimit_iterator(bfc_cobjptr_t s, bfc_iterptr_t it, size_t bs);

static int mutable_substr(bfc_cobjptr_t s, size_t pos, size_t n,
					void *buf, size_t bufsize);

const struct bfc_string_class bfc_shared_iso8859_15_string_class = {
	.super 		= &bfc_shared_string_class,
	.name 		= "shared_iso8859_15_string",
	.init 		= bfc_init_empty_iso8859_15_string,

	.getl		= get_codept,
	.setl		= set_codept,

	.ibegin		= init_begin_iterator,
	.ilimit		= init_limit_iterator,
	.rbegin		= init_rbegin_iterator,
	.rlimit		= init_rlimit_iterator,

	.traits		= (void *) &bfc_iso8859_15_traits_class,

	.substr		= bfc_shared_iso8859_15_substr,
	.mutable_substr	= mutable_substr,
	.buffered_substr= bfc_buffered_iso8859_15_substr,
};

const struct bfc_string_class bfc_buffered_iso8859_15_string_class = {
	.super 		= &bfc_string_class,
	.name 		= "buffered_iso8859_15_string",
	.init 		= bfc_init_empty_iso8859_15_string,

	.getl		= get_codept,
	.setl		= set_codept,

	.ibegin		= init_begin_iterator,
	.ilimit		= init_limit_iterator,
	.rbegin		= init_rbegin_iterator,
	.rlimit		= init_rlimit_iterator,

	.traits		= (void *) &bfc_iso8859_15_traits_class,

	.substr		= bfc_shared_iso8859_15_substr,
	.mutable_substr	= mutable_substr,
	.buffered_substr= bfc_buffered_iso8859_15_substr
};


int
bfc_init_empty_iso8859_15_string(void *buf, size_t bufsize,
				  bfc_mempool_t pool)
{
	static const long zbuf = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_objptr_t, s, buf, bufsize, pool,
			  &bfc_shared_iso8859_15_string_class);

	L4SC_WARN(logger, "%s(%p, %ld, pool %p): default constructor called!",
		__FUNCTION__, buf, (long) bufsize, pool);

	SET_STRBUF(s, &zbuf);
	return (BFC_SUCCESS);
}

int
bfc_init_shared_iso8859_15_string(void *buf, size_t bufsize,
				  const char *s, size_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_objptr_t, obj, buf, bufsize, NULL,
			  &bfc_shared_iso8859_15_string_class);

	L4SC_TRACE(logger, "%s(%p, %ld, s %p, n %ld)",
			__FUNCTION__, buf, (long) bufsize, s, (long) n);

	if (n >= bfc_basic_string_max_size(obj)) {
		L4SC_ERROR(logger, "%s: too large string: %lu >= %lu chars",
			__FUNCTION__, (unsigned long) n,
			(unsigned long) bfc_basic_string_max_size(obj));
		return (-EINVAL);
	}

	SET_STRBUF(obj, s);
	STRING_BUFSIZE(obj) = STRING_LEN(obj) = n;
	return (BFC_SUCCESS);
}

int
bfc_init_shared_iso8859_15_string_c_str(void *buf,size_t bufsize,const char *s)
{
	int n;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, s %p)",
		__FUNCTION__, buf, (long) bufsize, s);

	n = (*bfc_shared_iso8859_15_string_class.traits->szlen)(s);
	return bfc_init_shared_iso8859_15_string(buf, bufsize, s, n);
}

int
bfc_init_buffered_iso8859_15_string(void *buf, size_t bufsize,
				    const char *s, size_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_objptr_t, obj, buf, bufsize, NULL,
			  &bfc_buffered_iso8859_15_string_class);

	L4SC_TRACE(logger, "%s(%p, %ld, s %p, n %ld)",
			__FUNCTION__, buf, (long) bufsize, s, (long) n);

	if (n >= bfc_basic_string_max_size(obj)) {
		L4SC_ERROR(logger, "%s: too large string: %lu >= %lu chars",
			__FUNCTION__, (unsigned long) n,
			(unsigned long) bfc_basic_string_max_size(obj));
		return (-EINVAL);
	}

	SET_STRBUF(obj, s);
	STRING_BUFSIZE(obj) = STRING_LEN(obj) = n;
	return (BFC_SUCCESS);
}

int
bfc_shared_iso8859_15_substr(bfc_cobjptr_t s, size_t pos, size_t n,
						void *buf, size_t bufsize)
{
	const char *data = bfc_string_subdata(s, pos);
	const size_t sublen = bfc_string_sublen(s, pos, n);

	if ((pos == BFC_NPOS) || (pos > bfc_strlen(s))) {
		return (-ERANGE);
	}
	return (bfc_init_shared_iso8859_15_string(buf, bufsize, data, sublen));
}

static int
mutable_substr(bfc_cobjptr_t s, size_t pos, size_t n, void *buf, size_t bufsize)
{
	const char *data = bfc_string_subdata(s, pos);
	const size_t sublen = bfc_string_sublen(s, pos, n);
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_ERROR(logger, "%s: not available in class %s",
				__FUNCTION__, BFC_CLASS(s)->name);

	bfc_init_shared_iso8859_15_string(buf, bufsize, data, sublen);
	return (-ENOSYS);
}

int
bfc_buffered_iso8859_15_substr(bfc_cobjptr_t s, size_t pos, size_t n,
				void *buf, size_t bufsize,
				void *databuf, size_t dbufsize)
{
	int rc;

	if ((pos == BFC_NPOS) || (pos > bfc_strlen(s))) {
		return (-ERANGE);
	}
	rc = bfc_init_buffered_iso8859_15_string(buf, bufsize,
						 databuf, dbufsize);
	if (rc >= 0) {
		bfc_objptr_t substr = (bfc_objptr_t) buf;
		const char *data = bfc_string_subdata(s, pos);
		const size_t sublen = bfc_string_sublen(s, pos, n);
		rc = bfc_string_assign_buffer(substr, data, sublen);
	}
	return (rc);
}

static long
get_codept(bfc_cobjptr_t s, size_t pos)
{
	const char *p;
	unsigned codept;

	p = (char*) bfc_string_index((bfc_objptr_t)(uintptr_t)s, pos);
	codept = (*STRING_TRAITS(s)->to_int)(*p);
	return (codept);
}

static int
set_codept(bfc_objptr_t s, size_t pos, long codept)
{
	char c = (*STRING_TRAITS(s)->to_char)((int)codept);

	if ((pos == BFC_NPOS) || (pos > bfc_strlen(s))) {
		return (-ERANGE);
	} else if (pos == bfc_strlen(s)) {
		bfc_string_push_back((bfc_objptr_t)s, c);
	} else {
		char *p = (char*)bfc_string_index((bfc_objptr_t)s, pos);
		*p = c;
	}
	return (BFC_SUCCESS);
}

#define INIT_FORWARD_ITERATOR	bfc_init_iterator
#define INIT_REVERSE_ITERATOR	bfc_init_reverse_iterator

static int
init_begin_iterator(bfc_cobjptr_t s, bfc_iterptr_t it, size_t bs)
{
	return (INIT_FORWARD_ITERATOR(it, bs, (bfc_cobjptr_t)s, 0));
}

static int
init_limit_iterator(bfc_cobjptr_t s, bfc_iterptr_t it, size_t bs)
{
	size_t pos = bfc_strlen(s);
	return (INIT_FORWARD_ITERATOR(it, bs, (bfc_cobjptr_t)s, pos));
}

static int
init_rbegin_iterator(bfc_cobjptr_t s, bfc_iterptr_t it, size_t bs)
{
	size_t n = bfc_strlen(s);
	size_t pos = (n > 0)? (n-1): BFC_NPOS;
	return (INIT_REVERSE_ITERATOR(it, bs, (bfc_cobjptr_t)s, pos));
}

static int
init_rlimit_iterator(bfc_cobjptr_t s, bfc_iterptr_t it, size_t bs)
{
	return (INIT_REVERSE_ITERATOR(it, bs, (bfc_cobjptr_t)s, BFC_NPOS));
}


