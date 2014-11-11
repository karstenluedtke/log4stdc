
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"
#include "log4stdc.h"

#ifndef STRING_CLASS_NAME
#define STRING_CLASS_NAME "wstring"
#define IMPLEMENT_TOSTRING 1
#include "barefootc/utf8.h"
#endif


extern struct bfc_classhdr bfc_wchar_traits_class;

static int clone_wstring(bfc_cstrptr_t obj, void *buf, size_t bufsize);
static unsigned wstring_hashcode(bfc_cstrptr_t s);
static int wstring_equals(bfc_cstrptr_t s, bfc_cstrptr_t other);
static int wstring_tostring(bfc_cstrptr_t s, char *buf, size_t bufsize);
static void dump_wstring(bfc_cstrptr_t s, int depth, struct l4sc_logger *log);
static long bfc_wstring_getlong(bfc_cstrptr_t s, size_t pos);
static int bfc_wstring_setlong(bfc_strptr_t s, size_t pos, long c);
static int init_begin_iterator(bfc_cstrptr_t s, bfc_iterptr_t it, size_t bs);
static int init_limit_iterator(bfc_cstrptr_t s, bfc_iterptr_t it, size_t bs);
static int init_rbegin_iterator(bfc_cstrptr_t s, bfc_iterptr_t it, size_t bs);
static int init_rlimit_iterator(bfc_cstrptr_t s, bfc_iterptr_t it, size_t bs);
static void last_method(void) { }

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
				bfc_strptr_t, bfc_cstrptr_t, wchar_t)
};

struct bfc_string_class bfc_wstring_class = {
	/* intentionally not using selective initialization for base class: */
	/* I want the compiler to complain if something is missing.         */
	/* .super 	*/ NULL,
	/* .name 	*/ STRING_CLASS_NAME,
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ bfc_init_wstring,
	/* .destroy 	*/ bfc_destroy_wstring,
	/* .clone 	*/ clone_wstring,
	/* .clonesize 	*/ bfc_wstring_objsize,
	/* .hashcode 	*/ wstring_hashcode,
	/* .equals 	*/ wstring_equals,
	/* .length 	*/ bfc_wstring_length,
	/* .tostring 	*/ wstring_tostring,
	/* .dump 	*/ dump_wstring,

	/* Element access */
	/* .first	*/ bfc_wstring_data,
	/* .index	*/ bfc_wstring_index,
	/* .getl	*/ bfc_wstring_getlong,
	/* .setl	*/ bfc_wstring_setlong,
	/* .spare17 	*/ NULL,

	/* Iterators */
	/* .ibegin	*/ init_begin_iterator,
	/* .ilimit	*/ init_limit_iterator,
	/* .rbegin	*/ init_rbegin_iterator,
	/* .rlimit	*/ init_rlimit_iterator,
	/* .spare22 	*/ NULL,
	/* .spare23 	*/ NULL,

	/* Char traits	*/
	/* .traits	*/ (void *) &bfc_wchar_traits_class,

	/* Capacity	*/
	/* .size 	*/ bfc_wstring_length,
	/* .max_size	*/ bfc_wstring_max_size,
	/* .resize	*/ bfc_wstring_resize,
	/* .capacity	*/ bfc_wstring_capacity,
	/* .reserve	*/ bfc_wstring_reserve,

	/* Modifiers	*/
	/* .assign_buffer	*/ bfc_wstring_assign_buffer,
	/* .assign_fill 	*/ bfc_wstring_assign_fill,
	/* .append_buffer	*/ bfc_wstring_append_buffer,
	/* .append_fill 	*/ bfc_wstring_append_fill,
	/* .push_back		*/ bfc_wstring_push_back,
	/* .insert_buffer	*/ bfc_wstring_insert_buffer,
	/* .insert_fill 	*/ bfc_wstring_insert_fill,
	/* .insert_fillit	*/ bfc_wstring_insert_fillit,
	/* .insert_char 	*/ bfc_wstring_insert_char,
	/* .pop_back		*/ bfc_wstring_pop_back,
	/* .replace_buffer	*/ bfc_wstring_replace_buffer,
	/* .replace_fill	*/ bfc_wstring_replace_fill,
	/* .replace_range_buffer*/ bfc_wstring_replace_range_buffer,
	/* .replace_range_fill	*/ bfc_wstring_replace_range_fill,
	/* .replace_ranges	*/ bfc_wstring_replace_ranges,
	/* .copy		*/ bfc_wstring_copy,
	/* .swap		*/ (void *) bfc_swap_objects,

	/* String operations */
	/* .find_buffer		*/ bfc_wstring_find_buffer,
	/* .find_char		*/ bfc_wstring_find_char,
	/* .rfind_buffer	*/ bfc_wstring_rfind_buffer,
	/* .rfind_char		*/ bfc_wstring_rfind_char,
	/* .find_first_of_buffer*/ bfc_wstring_find_first_of_buffer,
	/* .find_first_of_char	*/ bfc_wstring_find_first_of_char,
	/* .find_last_of_buffer	*/ bfc_wstring_find_last_of_buffer,
	/* .find_last_of_char	*/ bfc_wstring_find_last_of_char,
	/* .find_first_not_of_buffer*/ bfc_wstring_find_first_not_of_buffer,
	/* .find_first_not_of_char  */ bfc_wstring_find_first_not_of_char,
	/* .find_last_not_of_buffer */ bfc_wstring_find_last_not_of_buffer,
	/* .find_last_not_of_char   */ bfc_wstring_find_last_not_of_char,
	/* .substr		*/ bfc_wstring_substr,
	/* .compare_buffer	*/ bfc_wstring_compare_buffer,

	/* Check nothing is missing */
	/* .last_method	*/ last_method
};

wchar_t *
bfc_wstrbuf(bfc_cstrptr_t s)
{
	return (wchar_t *)(uintptr_t) bfc_wstrdata(s);
}

int
bfc_init_wstring(void *buf, size_t bufsize, struct mempool *pool)
{
	static const long zbuf = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_strptr_t, s, buf, bufsize, pool,
			  &bfc_wstring_class);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p)",
		__FUNCTION__, buf, (long) bufsize, pool);

	s->buf = (void *) &zbuf;
	return (BFC_SUCCESS);
}

int
bfc_init_wstring_buffer(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s, size_t n)
{
	bfc_strptr_t obj = (bfc_strptr_t) buf;
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, s %p, n %ld)",
		__FUNCTION__, buf, (long) bufsize, pool, s, (long) n);

	if ((rc = bfc_init_wstring(obj, bufsize, pool)) < 0) {
		return(rc);
	}
	obj->buf = (void *) (intptr_t) s;
	obj->bufsize = obj->len = n;
	return (rc);
}

int
bfc_init_wstring_c_str(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s)
{
	bfc_strptr_t obj = (bfc_strptr_t) buf;
	int rc;

	if ((rc = bfc_init_wstring(obj, bufsize, pool)) < 0) {
		return(rc);
	}
	obj->buf = (void *) (intptr_t) s;
	obj->bufsize = obj->len = (*obj->vptr->traits->szlen)(s);
	return (rc);
}

void
bfc_destroy_wstring(bfc_strptr_t obj)
{
	bfc_string_classptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

static int
clone_wstring(bfc_cstrptr_t obj, void *buf, size_t bufsize)
{
	bfc_strptr_t s = (bfc_strptr_t) buf;
	size_t size = bfc_object_size(obj);
	if (bufsize < size) {
		return (-ENOSPC);
	}
	memcpy(s, obj, size);
	return (BFC_SUCCESS);
}

static unsigned
wstring_hashcode(bfc_cstrptr_t s)
{
	const size_t n = bfc_wstrlen(s);
	const wchar_t *p = bfc_wstrdata(s);
	unsigned x = 0;
	size_t i;
	for (i=0; i < n; i++) {
		x = (x << 7) ^ (x >> (8*sizeof(x)-7)) ^ p[i];
	}
	return (x);
}

static int
wstring_equals(bfc_cstrptr_t s, bfc_cstrptr_t other)
{
	int rc;

	if (s == other) {
		return (1);
	}
	rc = bfc_string_compare_bfstr((bfc_cstrptr_t)s, (bfc_cstrptr_t)other);
	return (rc == 0);
}

#ifdef IMPLEMENT_TOSTRING
static int
wstring_tostring(bfc_cstrptr_t s, char *buf, size_t bufsize)
{
	size_t n;

	n = bfc_utf8_from_wchar(buf, bufsize, bfc_wstrdata(s), bfc_wstrlen(s));

	return ((int) n);
}

static void
dump_wstring(bfc_cstrptr_t s, int depth, struct l4sc_logger *log)
{
	if (s && BFC_CLASS(s)) {
		const size_t len = bfc_wstrlen(s);
		if (len < 200) {
			const size_t bufsize = 3*len + 20;
			char *buf = alloca(bufsize);
			wstring_tostring(s, buf, bufsize);
			L4SC_DEBUG(log, "%s @%p, len %ld: \"%s\"",
				BFC_CLASS(s)->name, s, (long) len, buf);
		} else {
			L4SC_DEBUG(log, "%s @%p, len %ld @%p",
				BFC_CLASS(s)->name, s, (long) len,
				bfc_wstrdata(s));
		}
	}
}
#endif

size_t
bfc_wstring_objsize(bfc_cstrptr_t obj)
{
	return (sizeof(struct bfc_string));
}

// capacity:
size_t
bfc_wstring_length(bfc_cstrptr_t s)
{
	return (s->len);
}

size_t
bfc_wstring_max_size(bfc_cstrptr_t s)
{
	return ((0x7FFFuL << 8*(sizeof(wchar_t*)-2)) / sizeof(wchar_t));
}

int
bfc_wstring_resize(bfc_strptr_t s, size_t n, int c)
{
	int rc;

	if (n <= s->len) {
		wchar_t *data = bfc_wstrbuf(s);
		s->len = n;
		data[n] = '\0';
		return (BFC_SUCCESS);
	}
	RETVAR_METHCALL(rc, bfc_string_classptr_t, s,
			reserve, (s, n),
			-ENOSYS);
	if (rc == BFC_SUCCESS) {
		wchar_t *data = bfc_wstrbuf(s);
		(*s->vptr->traits->assign)(data + s->len, n - s->len, c);
		s->len = n;
		data[n] = '\0';
		return (BFC_SUCCESS);
	}
	return (rc);
}

size_t
bfc_wstring_capacity(bfc_cstrptr_t s)
{
	return ((s->bufsize != 0)? s->bufsize: s->len);
}

int
bfc_wstring_reserve(bfc_strptr_t s, size_t n)
{
	if (n >= bfc_wstring_max_size(s)) {
		return (-EINVAL);
	}
	if (s->offs + n < s->bufsize) {
		return (BFC_SUCCESS);
	}
	if (n < s->bufsize) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		wchar_t *buf = (wchar_t *) s->buf;
		wchar_t *p = buf + s->bufsize - (n+1);
		L4SC_DEBUG(logger, "%s: buf %p + %ld < limit %p",
			__FUNCTION__, buf, (long) n, buf + s->bufsize);
		if (s->len > 0) {
			(*s->vptr->traits->move)(p, buf+s->offs, s->len);
		}
		p[s->len] = '\0';
		s->offs = p - buf;
		return (BFC_SUCCESS);
	}
	return (-ENOSYS);
}

// element access:
wchar_t *
bfc_wstring_index(bfc_strptr_t s, size_t pos)
{
	wchar_t *buf = (wchar_t *) s->buf;
	return (buf + s->offs + pos);
}

const wchar_t*
bfc_wstring_data(bfc_cstrptr_t s)  /* not zero terminated */
{
	wchar_t *buf = (wchar_t *) s->buf;
	return (buf + s->offs);
}

static long
bfc_wstring_getlong(bfc_cstrptr_t s, size_t pos)
{
	const wchar_t *p;
	p = (wchar_t*) bfc_string_index((bfc_strptr_t)(uintptr_t)s, pos);
	return (((long) *p) & ~(~0uL << 8*sizeof(*p)));
}

static int
bfc_wstring_setlong(bfc_strptr_t s, size_t pos, long c)
{
	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(s))) {
		return (-ERANGE);
	} else if (pos == bfc_wstrlen(s)) {
		bfc_string_push_back((bfc_strptr_t)s, c);
	} else {
		wchar_t *p = (wchar_t*)bfc_string_index((bfc_strptr_t)s, pos);
		*p = (wchar_t) c;
	}
	return (BFC_SUCCESS);
}

/* Iterators */

#ifndef INIT_FORWARD_ITERATOR
#define INIT_FORWARD_ITERATOR	bfc_init_iterator
#endif
#ifndef INIT_REVERSE_ITERATOR
#define INIT_REVERSE_ITERATOR	bfc_init_reverse_iterator
#endif

static int
init_begin_iterator(bfc_cstrptr_t s, bfc_iterptr_t it, size_t bs)
{
	return (INIT_FORWARD_ITERATOR(it, bs, (bfc_cobjptr_t)s, 0));
}

static int
init_limit_iterator(bfc_cstrptr_t s, bfc_iterptr_t it, size_t bs)
{
	size_t pos = bfc_wstrlen(s);
	return (INIT_FORWARD_ITERATOR(it, bs, (bfc_cobjptr_t)s, pos));
}

static int
init_rbegin_iterator(bfc_cstrptr_t s, bfc_iterptr_t it, size_t bs)
{
	size_t n = bfc_wstrlen(s);
	size_t pos = (n > 0)? (n-1): BFC_NPOS;
	return (INIT_REVERSE_ITERATOR(it, bs, (bfc_cobjptr_t)s, pos));
}

static int
init_rlimit_iterator(bfc_cstrptr_t s, bfc_iterptr_t it, size_t bs)
{
	return (INIT_REVERSE_ITERATOR(it, bs, (bfc_cobjptr_t)s, BFC_NPOS));
}


/* Modifiers */
int
bfc_wstring_assign_buffer(bfc_strptr_t s, const wchar_t *s2, size_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	int rc;

	L4SC_TRACE(logger, "%s(%p, %p, %ld)", __FUNCTION__, s, s2, (long) n);

	if ((rc = bfc_string_reserve((bfc_strptr_t)s, n)) == BFC_SUCCESS) {
		wchar_t *data = bfc_wstrbuf(s);
		if (n > 0) {
			(*s->vptr->traits->copy)(data, s2, n);
		}
		data[n] = '\0';
		s->len = n;
		return (BFC_SUCCESS);
	} else {
		L4SC_ERROR(logger, "%s: no space for %ld characters",
						__FUNCTION__, (long) n);
	}
	return (rc);
}

#define MAYBE_MULTICHAR(codept)	((codept) & (~0 << (8*sizeof(wchar_t)-1)))

int
bfc_wstring_assign_fill(bfc_strptr_t s, size_t n, int c)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	int rc;

	L4SC_TRACE(logger, "%s(%p, %ld, %02x)", __FUNCTION__, s, (long) n, c);

	if ((rc = bfc_string_reserve((bfc_strptr_t)s, n)) == BFC_SUCCESS) {
		if (!MAYBE_MULTICHAR(c)) {
			wchar_t *data = bfc_wstrbuf(s);
			if (n > 0) {
				(*s->vptr->traits->assign)(data, n, c);
			}
			data[n] = '\0';
			s->len = n;
			return (BFC_SUCCESS);
		} else {
			bfc_iterator_t i1, i2, j1, j2;
			bfc_string_begin_iterator(s, &i1, sizeof(i1));
			bfc_string_end_iterator  (s, &i2, sizeof(i2));
			bfc_init_source_iterator(&j1, sizeof(j1), c, 0);
			bfc_init_source_iterator(&j2, sizeof(j2), c, n);
			RETURN_METHCALL(bfc_string_classptr_t, s,
				replace_ranges, (s, &i1, &i2, &j1, &j2),
				bfc_wstring_replace_ranges(s,&i1,&i2,&j1,&j2));
		}
	} else {
		L4SC_ERROR(logger, "%s: no space for %ld characters",
						__FUNCTION__, (long) n);
	}
	return (rc);
}

int
bfc_wstring_append_buffer(bfc_strptr_t s, const wchar_t *s2, size_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	int rc;

	L4SC_TRACE(logger, "%s(%p, %p, %ld)", __FUNCTION__, s, s2, (long) n);

	if ((rc = bfc_string_reserve((bfc_strptr_t)s, s->len + n))
							== BFC_SUCCESS) {
		wchar_t *data = bfc_wstrbuf(s) + s->len;
		if (n > 0) {
			(*s->vptr->traits->copy)(data, s2, n);
		}
		data[n] = '\0';
		s->len += n;
		L4SC_DEBUG(logger, "%s: len %ld", __FUNCTION__, (long) s->len);
		return (BFC_SUCCESS);
	} else {
		L4SC_ERROR(logger, "%s: no space for %ld+%ld characters",
				__FUNCTION__, (long) s->len, (long) n);
	}
	return (rc);
}

int
bfc_wstring_append_fill(bfc_strptr_t s, size_t n, int c)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	int rc;

	L4SC_TRACE(logger, "%s(%p, %ld, %02x)", __FUNCTION__, s, (long) n, c);

	if ((rc = bfc_string_reserve((bfc_strptr_t)s, s->len + n))
							== BFC_SUCCESS) {
		if (!MAYBE_MULTICHAR(c)) {
			wchar_t *data = bfc_wstrbuf(s) + s->len;
			if (n > 0) {
				(*s->vptr->traits->assign)(data, n, c);
			}
			data[n] = '\0';
			s->len += n;
			return (BFC_SUCCESS);
		} else {
			bfc_iterator_t i1, j1, j2;
			bfc_string_end_iterator(s, &i1, sizeof(i1));
			bfc_init_source_iterator(&j1, sizeof(j1), c, 0);
			bfc_init_source_iterator(&j2, sizeof(j2), c, n);
			RETURN_METHCALL(bfc_string_classptr_t, s,
				replace_ranges, (s, &i1, &i1, &j1, &j2),
				bfc_wstring_replace_ranges(s,&i1,&i1,&j1,&j2));
		}
	} else {
		L4SC_ERROR(logger, "%s: no space for %ld+%ld characters",
				__FUNCTION__, (long) s->len, (long) n);
	}
	return (rc);
}

int
bfc_wstring_push_back(bfc_strptr_t s, int c)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	int rc;

	L4SC_TRACE(logger, "%s(%p, %02x)", __FUNCTION__, s, c);

	if ((rc = bfc_string_reserve((bfc_strptr_t)s, s->len + 1))
							== BFC_SUCCESS) {
		wchar_t *data = bfc_wstrbuf(s) + s->len;
		data[0] = c;
		data[1] = '\0';
		s->len += 1;
		return (c);
	} else {
		L4SC_ERROR(logger, "%s: no space for %ld+1 characters",
						__FUNCTION__, (long) s->len);
	}
	return (rc);
}

int
bfc_wstring_insert_buffer(bfc_strptr_t s, size_t pos,
			  const wchar_t *s2, size_t n)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, s2, n),
			bfc_wstring_replace_buffer(s, pos, 0, s2, n));
}

int
bfc_wstring_insert_fill(bfc_strptr_t s, size_t pos, size_t n, int c)
{
	bfc_iterator_t i1, j1, j2;

	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	bfc_string_begin_iterator(s, &i1, sizeof(i1));
	bfc_iterator_set_position(&i1, pos);
	bfc_init_source_iterator(&j1, sizeof(j1), c, 0);
	bfc_init_source_iterator(&j2, sizeof(j2), c, n);
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, &i1, &i1, &j1, &j2),
			bfc_wstring_replace_ranges(s, &i1, &i1, &j1, &j2));
}

int
bfc_wstring_insert_fillit(bfc_strptr_t s, bfc_iterptr_t p, size_t n, int c)
{
	size_t pos;
	bfc_iterator_t j1, j2;

	if (p->obj != (bfc_objptr_t) s) {
		return (-EFAULT);
	}
	pos = bfc_iterator_position(p);
	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	bfc_init_source_iterator(&j1, sizeof(j1), c, 0);
	bfc_init_source_iterator(&j2, sizeof(j2), c, n);
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, p, p, &j1, &j2),
			bfc_wstring_replace_ranges(s, p, p, &j1, &j2));
}

int
bfc_wstring_insert_char(bfc_strptr_t s, bfc_iterptr_t p, int c)
{
	size_t pos;
	wchar_t data[2];

	if (p->obj != (bfc_objptr_t) s) {
		return (-EFAULT);
	}
	pos = bfc_iterator_position(p);
	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	data[0] = c; data[1] = '\0';
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, data, 1),
			bfc_wstring_replace_buffer(s, pos, 0, data, 1));
}

void
bfc_wstring_pop_back(bfc_strptr_t s)
{
	if (bfc_wstrlen(s) > 0) {
		wchar_t *data = bfc_wstrbuf(s);
		data[--(s->len)] = '\0';
	}
}

int
bfc_wstring_replace_buffer(bfc_strptr_t s, size_t pos, size_t n1,
			   const wchar_t* s2, size_t n2)
{
	const size_t len = bfc_wstrlen(s);
	size_t nkill = bfc_wstring_sublen(s, pos, n1);
	size_t ntail = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	int rc;

	L4SC_TRACE(logger, "%s(%p, %ld, %ld, %p, %ld)",
		__FUNCTION__, s, (long) pos, (long) n1, s2, (long) n2);

	if ((pos == BFC_NPOS) || (pos > len)) {
		L4SC_ERROR(logger, "%s: pos %ld behind len %ld",
			__FUNCTION__, (long) pos, (long) len);
		return (-ERANGE);
	}
	if ((n2 > nkill)
	 && ((rc = bfc_string_reserve((bfc_strptr_t)s, s->len + n2-nkill))
							!= BFC_SUCCESS)) {
		L4SC_ERROR(logger, "%s: no space for %ld-%ld+%ld characters",
			__FUNCTION__, (long) s->len, (long) nkill, (long) n2);
		return (rc);
	} else {
		wchar_t *data = bfc_wstrbuf(s);
		wchar_t *dest = data + len;
		wchar_t *tail = data + len;
		if ((pos != BFC_NPOS) && (pos < len)) {
			dest = data + pos;
			tail = dest + nkill;
			if (pos + nkill < len) {
				ntail = len - (pos + nkill);
			}
		}
		if ((ntail > 0) && (n2 != nkill)) {
			wchar_t *taildest = tail + n2 - nkill;
			L4SC_DEBUG(logger, "%s: move tail: %ld @%p -> %p",
				__FUNCTION__, (long) ntail, tail, taildest);
			(*s->vptr->traits->move)(taildest, tail, ntail);
			taildest[ntail] = '\0';
		}
		if (n2 > 0) {
			L4SC_DEBUG(logger, "%s: copy str2: %ld @%p -> %p",
				__FUNCTION__, (long) n2, s2, dest);
			(*s->vptr->traits->copy)(dest, s2, n2);
		}
		dest[n2+ntail] = '\0';
		s->len = dest + n2 + ntail - data;
		L4SC_DEBUG(logger, "%s: len %ld", __FUNCTION__, (long) s->len);
	}
	return (BFC_SUCCESS);
}

int
bfc_wstring_replace_fill(bfc_strptr_t s, size_t pos, size_t n1,
					size_t n2, int c)
{
	bfc_iterator_t i1, i2, j1, j2;

	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	bfc_string_begin_iterator(s, &i1, sizeof(i1));
	bfc_string_begin_iterator(s, &i2, sizeof(i2));
	bfc_iterator_set_position(&i1, pos);
	bfc_iterator_set_position(&i2, pos + bfc_wstring_sublen(s, pos, n1));
	bfc_init_source_iterator(&j1, sizeof(j1), c, 0);
	bfc_init_source_iterator(&j2, sizeof(j2), c, n2);
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, &i1, &i2, &j1, &j2),
			bfc_wstring_replace_ranges(s, &i1, &i2, &j1, &j2));
}

int
bfc_wstring_replace_range_buffer(bfc_strptr_t s, bfc_iterptr_t i1,
				bfc_iterptr_t i2, const wchar_t* s2, size_t n)
{
	size_t pos;
	ptrdiff_t k;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	if ((i1->obj != (bfc_objptr_t) s) || (i2->obj != (bfc_objptr_t) s)) {
		return (-EFAULT);
	}
	pos = bfc_iterator_position(i1);
	if ((k = bfc_iterator_distance(i1, i2)) < 0) {
		return (-EINVAL);
	}
	L4SC_DEBUG(logger,"%s: pos %ld, k %ld",__FUNCTION__,(long)pos,(long)k);
	bfc_object_dump(i1, 1, logger);
	bfc_object_dump(i2, 1, logger);
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, k, s2, n),
			bfc_wstring_replace_buffer(s, pos, k, s2, n));
}

int
bfc_wstring_replace_range_fill(bfc_strptr_t s, bfc_iterptr_t i1,
					bfc_iterptr_t i2, size_t n, int c)
{
	bfc_iterator_t j1, j2;

	bfc_init_source_iterator(&j1, sizeof(j1), c, 0);
	bfc_init_source_iterator(&j2, sizeof(j2), c, n);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, i1, i2, &j1, &j2),
			bfc_wstring_replace_ranges(s, i1, i2, &j1, &j2));
}

int
bfc_wstring_replace_ranges(bfc_strptr_t s, bfc_iterptr_t i1, bfc_iterptr_t i2,
					   bfc_iterptr_t j1, bfc_iterptr_t j2)
{
	int rc=0, rc2;
	ptrdiff_t d, e;
	size_t bufsize;
	wchar_t *tailbuf;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	bfc_iterator_t it;

	L4SC_TRACE(logger, "%s(%p, %p, %p, %p, %p)",
			__FUNCTION__, s, i1, i2, j1, j2);

	if ((d = bfc_iterator_distance(j1, j2)) < 0) {
		L4SC_ERROR(logger, "%s: negative distance %ld",
						__FUNCTION__, (long)d);
		return (-EINVAL);
	}

	if (bfc_string_end_iterator(s, &it, sizeof(it)) >= 0) {
		e = bfc_iterator_distance(i2, &it);
	} else {
		L4SC_ERROR(logger, "%s: init end iterator failed",__FUNCTION__);
		e = bfc_strlen(s);
	}
	bufsize = (e > 0)? e+1: 2;
	tailbuf = alloca(bufsize * sizeof(wchar_t));
	if (e > 0) {
		bfc_wstring_copy(s, tailbuf, e, bfc_iterator_position(i2)); 
	} else if (e < 0) {
		L4SC_ERROR(logger, "%s: negative tail distance %ld",
						__FUNCTION__, (long)e);
		return (-EINVAL);
	}

	bfc_string_resize(s, bfc_iterator_position(i1), 0);

	if (d > 0) {
		it = *i1;
		if ((rc = bfc_string_append_iter_range(s, &it, j1, j2)) < 0) {
			L4SC_ERROR(logger, "%s: appending range error %d",
							__FUNCTION__, rc);
		}
	}

	if ((e > 0) && tailbuf) {
		if ((rc2 = bfc_wstring_append_buffer(s, tailbuf, e)) < 0) {
			L4SC_ERROR(logger, "%s: appending tail error %d",
							__FUNCTION__, rc);
			rc = (rc < 0)? rc: rc2;
		}
	}

	bfc_object_dump(s, 1, logger);
	return (rc);
}

size_t
bfc_wstring_copy(bfc_cstrptr_t s, wchar_t* s2, size_t n, size_t pos)
{
	const size_t len = bfc_wstring_sublen(s, pos, n);
	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(s))) {
		return ((size_t) -ERANGE);
	}
	if (len > 0) {
		const wchar_t *data = bfc_wstring_subdata(s, pos);
		(*s->vptr->traits->copy)(s2, data, len);
		return (len);
	}
	return (0);
}


/* String operations */
size_t
bfc_wstring_find_buffer(bfc_cstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n)
{
	if (n == 0) {
		return ((pos <= bfc_wstrlen(s))? pos: BFC_NPOS);
	} else if (n > bfc_wstrlen(s)) {
		return (BFC_NPOS);
	} else if (pos < bfc_wstrlen(s)) {
		const wchar_t *cp, *data = bfc_wstrdata(s);
		const wchar_t *limit = data + bfc_wstrlen(s);
		const wchar_t c = s2[0];
		for (cp = data + pos; cp < limit; cp++) {
			cp = (*s->vptr->traits->find)(cp, limit-cp, c);
			if ((cp == NULL) || (cp+n > limit)) {
				break;
			}
			if ((*s->vptr->traits->compare)(cp, s2, n) == 0) {
				return (cp - data);
			}
		}
	}
	return (BFC_NPOS);
}

size_t
bfc_wstring_find_char(bfc_cstrptr_t s, int c, size_t pos)
{
	const int len = bfc_wstrlen(s);
	const wchar_t *cp = NULL, *data = bfc_wstrdata(s);
	if (pos < len) {
		cp = (*s->vptr->traits->find)(data+pos, len-pos, c);
	}
	return (cp? cp - data: BFC_NPOS);
}

size_t
bfc_wstring_rfind_buffer(bfc_cstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n)
{
	const size_t len = bfc_wstrlen(s);
	const wchar_t *cp, *data = bfc_wstrdata(s);

	if (n == 0) {
		return ((pos < len)? pos: len);
	} else if (len >= n) {
		cp = data + len - n;
		if ((pos != BFC_NPOS) && (pos < len - n)) {
			cp = data + pos;
		}
		for (; cp >= data; cp--) {
			if ((*s->vptr->traits->compare)(cp, s2, n) == 0) {
				return (cp - data);
			}
		}
	}
	return (BFC_NPOS);
}

size_t
bfc_wstring_rfind_char(bfc_cstrptr_t s, int c, size_t pos)
{
	const size_t len = bfc_wstrlen(s);
	const wchar_t *cp, *data = bfc_wstrdata(s);

	if (len > 0) {
		cp = data + len - 1;
		if ((pos != BFC_NPOS) && (pos < len - 1)) {
			cp = data + pos;
		}
		for (; cp >= data; cp--) {
			if ((*s->vptr->traits->eq)(*cp, c)) {
				return (cp - data);
			}
		}
	}
	return (BFC_NPOS);
}

size_t
bfc_wstring_find_first_of_buffer(bfc_cstrptr_t s, const wchar_t* s2,
						size_t pos, size_t n)
{
	const wchar_t *cp, *data = bfc_wstrdata(s);
	const wchar_t *limit = data + bfc_wstrlen(s);

	if ((pos < bfc_wstrlen(s)) && (n > 0)) {
		for (cp = data + pos; cp < limit; cp++) {
			if ((*s->vptr->traits->find)(s2, n, *cp) != NULL) {
				return (cp - data);
			}
		}
	}
	return (BFC_NPOS);
}

size_t
bfc_wstring_find_first_of_char(bfc_cstrptr_t s, int c, size_t pos)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_char, (s, c, pos),
			bfc_wstring_find_char(s, c, pos));
}

size_t
bfc_wstring_find_last_of_buffer(bfc_cstrptr_t s, const wchar_t* s2,
						size_t pos, size_t n)
{
	const size_t len = bfc_wstrlen(s);
	const wchar_t *cp, *data = bfc_wstrdata(s);

	if ((len > 0) && (n > 0)) {
		cp = data + len - 1;
		if ((pos != BFC_NPOS) && (pos < len - 1)) {
			cp = data + pos;
		}
		for (; cp >= data; cp--) {
			if ((*s->vptr->traits->find)(s2, n, *cp) != NULL) {
				return (cp - data);
			}
		}
	}
	return (BFC_NPOS);
}

size_t
bfc_wstring_find_last_of_char(bfc_cstrptr_t s, int c, size_t pos)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			rfind_char, (s, c, pos),
			bfc_wstring_rfind_char(s, c, pos));
}

size_t
bfc_wstring_find_first_not_of_buffer(bfc_cstrptr_t s,const wchar_t* s2,
					size_t pos, size_t n)
{
	const wchar_t *cp, *data = bfc_wstrdata(s);
	const wchar_t *limit = data + bfc_wstrlen(s);

	if (pos < bfc_wstrlen(s)) {
		if (n < 1) {
			return (pos);
		}
		for (cp = data + pos; cp < limit; cp++) {
			if ((*s->vptr->traits->find)(s2, n, *cp) == NULL) {
				return (cp - data);
			}
		}
	}
	return (BFC_NPOS);
}

size_t
bfc_wstring_find_first_not_of_char(bfc_cstrptr_t s, int c, size_t pos)
{
	const wchar_t *cp, *data = bfc_wstrdata(s);
	const wchar_t *limit = data + bfc_wstrlen(s);

	if (pos < bfc_wstrlen(s)) {
		for (cp = data + pos; cp < limit; cp++) {
			if (!(*s->vptr->traits->eq)(c, *cp)) {
				return (cp - data);
			}
		}
	}
	return (BFC_NPOS);
}

size_t
bfc_wstring_find_last_not_of_buffer(bfc_cstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n)
{
	const size_t len = bfc_wstrlen(s);
	const wchar_t *cp, *data = bfc_wstrdata(s);

	if (len > 0) {
		cp = data + len - 1;
		if ((pos != BFC_NPOS) && (pos < len - 1)) {
			cp = data + pos;
		}
		if (n < 1) {
			return (cp - data);
		}
		for (; cp >= data; cp--) {
			if ((*s->vptr->traits->find)(s2, n, *cp) == NULL) {
				return (cp - data);
			}
		}
	}
	return (BFC_NPOS);
}

size_t
bfc_wstring_find_last_not_of_char(bfc_cstrptr_t s, int c, size_t pos)
{
	const size_t len = bfc_wstrlen(s);
	const wchar_t *cp, *data = bfc_wstrdata(s);

	if (len > 0) {
		cp = data + len - 1;
		if ((pos != BFC_NPOS) && (pos < len - 1)) {
			cp = data + pos;
		}
		for (; cp >= data; cp--) {
			if (!(*s->vptr->traits->eq)(c, *cp)) {
				return (cp - data);
			}
		}
	}
	return (BFC_NPOS);
}

int
bfc_wstring_substr(bfc_cstrptr_t s, size_t pos, size_t n,
		   void *buf, size_t bufsize)
{
	bfc_strptr_t s2 = (bfc_strptr_t) buf;
	const wchar_t *data = bfc_wstring_subdata(s, pos);
	const size_t sublen = bfc_wstring_sublen(s, pos, n);
	int rc;

	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	/*
	 * If the buffer supplied is large enough, we make a writeable copy
	 * of the substring in the remaining buffer space behind s2.
	 */
	if (bufsize > sizeof(*s2) + ((sublen+1) * sizeof(wchar_t))) {
		const size_t l1 = sizeof(s2[0]);
		const size_t l2 = (bufsize - l1) / sizeof(wchar_t);
		wchar_t *b2 = (wchar_t *) &s2[1];
		if ((bfc_init_wstring_buffer(s2, l1, NULL, b2, l2) >= 0)
		 && (bfc_wstring_assign_buffer(s2, data, sublen)   >= 0)) {
			return ((int) sublen);
		}
	}
	return (bfc_init_shared_wstring_buffer(buf, bufsize, data, sublen));
}

int
bfc_wstring_compare_buffer(bfc_cstrptr_t s, size_t pos1, size_t n1,
			   const wchar_t* s2, size_t n2)
{
	const size_t l1 = bfc_wstring_sublen(s, pos1, n1);

	if ((pos1 == BFC_NPOS) || (pos1 > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	if ((l1 > 0) && (n2 > 0)) {
		const wchar_t *s1 = bfc_wstring_subdata(s, pos1);
		if (l1 == n2) {
			int rc = (*s->vptr->traits->compare)(s1, s2, l1);
			return ((rc > 0)? 1: (rc < 0)? -1: 0);
		} else if (l1 < n2) {
			int rc = (*s->vptr->traits->compare)(s1, s2, l1);
			return ((rc > 0)? 1: -1);
		} else {
			int rc = (*s->vptr->traits->compare)(s1, s2, n2);
			return ((rc < 0)? -1: 1);
		}
	} else if (l1 > 0) {
		return (1);
	} else if (n2 > 0) {
		return (-1);
	}
	return (0);
}

