
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

#define LOGGERNAME "barefootc.string", 16

extern struct bfc_classhdr bfc_wchar_traits_class;

static int clone_wstring(bfc_cwstrptr_t obj, void *buf, size_t bufsize);
static unsigned wstring_hashcode(bfc_cwstrptr_t s);
static int wstring_equals(bfc_cwstrptr_t s, bfc_cwstrptr_t other);
static int wstring_tostring(bfc_cwstrptr_t s, char *buf, size_t bufsize);
static void dump_wstring(bfc_cwstrptr_t s, int depth, struct l4sc_logger *log);
static void last_method(void) { }

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
				bfc_wstrptr_t, bfc_cwstrptr_t, wchar_t)
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
	/* .spare15 	*/ NULL,
	/* Char traits	*/
	/* .traits	*/ (void *) &bfc_wchar_traits_class,
	/* Allocators 	*/
	/* .init_bfstr	*/ bfc_init_wstring_bfstr,
	/* .init_move	*/ bfc_init_wstring_move,
	/* .init_substr	*/ bfc_init_wstring_substr,
	/* .init_buffer	*/ bfc_init_wstring_buffer,
	/* .init_c_str	*/ bfc_init_wstring_c_str,
	/* .init_fill	*/ bfc_init_wstring_fill,
	/* .init_range	*/ bfc_init_wstring_range,
	/* Capacity	*/
	/* .size 	*/ bfc_wstring_length,
	/* .max_size	*/ bfc_wstring_max_size,
	/* .resize	*/ bfc_wstring_resize,
	/* .capacity	*/ bfc_wstring_capacity,
	/* .reserve	*/ bfc_wstring_reserve,
	/* Modifiers	*/
	/* .assign_bfstr	*/ bfc_wstring_assign_bfstr,
	/* .assign_substr	*/ bfc_wstring_assign_substr,
	/* .assign_c_str	*/ bfc_wstring_assign_c_str,
	/* .assign_buffer	*/ bfc_wstring_assign_buffer,
	/* .assign_fill 	*/ bfc_wstring_assign_fill,
	/* .assign_range	*/ bfc_wstring_assign_range,
	/* .append_bfstr	*/ bfc_wstring_append_bfstr,
	/* .append_substr	*/ bfc_wstring_append_substr,
	/* .append_c_str	*/ bfc_wstring_append_c_str,
	/* .append_buffer	*/ bfc_wstring_append_buffer,
	/* .append_fill 	*/ bfc_wstring_append_fill,
	/* .append_range	*/ bfc_wstring_append_range,
	/* .push_back		*/ bfc_wstring_push_back,
	/* .insert_bfstr	*/ bfc_wstring_insert_bfstr,
	/* .insert_substr	*/ bfc_wstring_insert_substr,
	/* .insert_c_str	*/ bfc_wstring_insert_c_str,
	/* .insert_buffer	*/ bfc_wstring_insert_buffer,
	/* .insert_fill 	*/ bfc_wstring_insert_fill,
	/* .insert_fillit	*/ bfc_wstring_insert_fillit,
	/* .insert_char 	*/ bfc_wstring_insert_char,
	/* .insert_range	*/ bfc_wstring_insert_range,
	/* .erase_seq		*/ bfc_wstring_erase_seq,
	/* .erase_tail		*/ bfc_wstring_erase_tail,
	/* .erase_char		*/ bfc_wstring_erase_char,
	/* .erase_range		*/ bfc_wstring_erase_range,
	/* .pop_back		*/ bfc_wstring_pop_back,
	/* .replace_bfstr	*/ bfc_wstring_replace_bfstr,
	/* .replace_substr	*/ bfc_wstring_replace_substr,
	/* .replace_buffer	*/ bfc_wstring_replace_buffer,
	/* .replace_c_str	*/ bfc_wstring_replace_c_str,
	/* .replace_fill	*/ bfc_wstring_replace_fill,
	/* .replace_range_bfstr	*/ bfc_wstring_replace_range_bfstr,
	/* .replace_range_buffer*/ bfc_wstring_replace_range_buffer,
	/* .replace_range_c_str	*/ bfc_wstring_replace_range_c_str,
	/* .replace_range_fill	*/ bfc_wstring_replace_range_fill,
	/* .replace_ranges	*/ bfc_wstring_replace_ranges,
	/* .copy		*/ bfc_wstring_copy,
	/* .swap		*/ (void *) bfc_swap_objects,

	/* String operations */
	/* .find_bfstr		*/ bfc_wstring_find_bfstr,
	/* .find_buffer		*/ bfc_wstring_find_buffer,
	/* .find_c_str		*/ bfc_wstring_find_c_str,
	/* .find_char		*/ bfc_wstring_find_char,
	/* .rfind_bfstr		*/ bfc_wstring_rfind_bfstr,
	/* .rfind_buffer	*/ bfc_wstring_rfind_buffer,
	/* .rfind_c_str		*/ bfc_wstring_rfind_c_str,
	/* .rfind_char		*/ bfc_wstring_rfind_char,
	/* .find_first_of_bfstr	*/ bfc_wstring_find_first_of_bfstr,
	/* .find_first_of_buffer*/ bfc_wstring_find_first_of_buffer,
	/* .find_first_of_c_str	*/ bfc_wstring_find_first_of_c_str,
	/* .find_first_of_char	*/ bfc_wstring_find_first_of_char,
	/* .find_last_of_bfstr	*/ bfc_wstring_find_last_of_bfstr,
	/* .find_last_of_buffer	*/ bfc_wstring_find_last_of_buffer,
	/* .find_last_of_c_str	*/ bfc_wstring_find_last_of_c_str,
	/* .find_last_of_char	*/ bfc_wstring_find_last_of_char,
	/* .find_first_not_of_bfstr */ bfc_wstring_find_first_not_of_bfstr,
	/* .find_first_not_of_buffer*/ bfc_wstring_find_first_not_of_buffer,
	/* .find_first_not_of_c_str */ bfc_wstring_find_first_not_of_c_str,
	/* .find_first_not_of_char  */ bfc_wstring_find_first_not_of_char,
	/* .find_last_not_of_bfstr  */ bfc_wstring_find_last_not_of_bfstr,
	/* .find_last_not_of_buffer */ bfc_wstring_find_last_not_of_buffer,
	/* .find_last_not_of_c_str  */ bfc_wstring_find_last_not_of_c_str,
	/* .find_last_not_of_char   */ bfc_wstring_find_last_not_of_char,
	/* .substr		*/ bfc_wstring_substr,
	/* .compare_bfstr	*/ bfc_wstring_compare_bfstr,
	/* .compare_substr	*/ bfc_wstring_compare_substr,
	/* .compare_substrs	*/ bfc_wstring_compare_substrs,
	/* .compare_c_str	*/ bfc_wstring_compare_c_str,
	/* .compare_substr_c_str*/ bfc_wstring_compare_substr_c_str,
	/* .compare_buffer	*/ bfc_wstring_compare_buffer,
	/* Check nothing is missing */
	/* .last_method	*/ last_method
};

size_t
bfc_wstrlen(bfc_cwstrptr_t s)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, length, (s), 0); 
}

wchar_t *
bfc_wstrbuf(bfc_cwstrptr_t s)
{
	return (wchar_t *)(uintptr_t) bfc_wstrdata(s);
}

const wchar_t *
bfc_wstrdata(bfc_cwstrptr_t s)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, first, (s), s->buf); 
}

size_t
bfc_wstring_sublen(bfc_cwstrptr_t s, size_t pos, size_t n)
{
	size_t remain, len = bfc_wstrlen(s);
	if ((pos == BFC_NPOS) || (pos >= len)) {
		return (0);
	} else if (n > (remain = len - pos)) {
		return (remain);
	}
	return (n);
}

int
bfc_wstr_reserve(bfc_wstrptr_t s, size_t n)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, reserve, (s, n),
			bfc_wstring_reserve(s, n)); 
}

int
bfc_init_wstring(void *buf, size_t bufsize, struct mempool *pool)
{
	static const long zbuf = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_wstrptr_t, s, buf, bufsize, pool,
			  &bfc_wstring_class);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p)",
		__FUNCTION__, buf, (long) bufsize, pool);

	s->buf = (wchar_t *) &zbuf;
	return (BFC_SUCCESS);
}

int
bfc_init_wstring_bfstr(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cwstrptr_t str)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, str %p)",
		__FUNCTION__, buf, (long) bufsize, pool, str);

	rc = bfc_init_wstring_buffer(buf, bufsize, pool,
				     bfc_wstrdata(str), bfc_wstrlen(str));
	return (rc);
}

int
bfc_init_wstring_move(void *buf, size_t bufsize, struct mempool *pool,
				bfc_wstrptr_t str)
{
	int rc;
	bfc_wstrptr_t obj = (bfc_wstrptr_t) buf;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, str %p)",
		__FUNCTION__, buf, (long) bufsize, pool, str);
	if ((rc = bfc_init_wstring(obj, bufsize, pool)) < 0) {
		return(rc);
	}
	obj->buf = str->buf;
	obj->len = str->len;
	obj->bufsize = str->bufsize;
	str->bufsize = 0;
	str->len = 0;
	str->buf = NULL;
	return (rc);
}

int
bfc_init_wstring_substr(void *buf, size_t bufsize,
			bfc_cwstrptr_t str, size_t pos, size_t n)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, str %p, pos %ld, n %ld)",
		__FUNCTION__, buf, (long) bufsize, str, (long) pos, (long) n);

	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(str))) {
		return (-ERANGE);
	}
	rc = bfc_init_wstring_buffer(buf, bufsize, NULL,
		bfc_wstrdata(str) + pos, bfc_wstring_sublen(str, pos, n));
	return (rc);
}

int
bfc_init_wstring_buffer(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s, size_t n)
{
	bfc_wstrptr_t obj = (bfc_wstrptr_t) buf;
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, s %p, n %ld)",
		__FUNCTION__, buf, (long) bufsize, pool, s, (long) n);

	if ((rc = bfc_init_wstring(obj, bufsize, pool)) < 0) {
		return(rc);
	}
	obj->buf = (wchar_t *) (intptr_t) s;
	obj->bufsize = obj->len = n;
	return (rc);
}

int
bfc_init_wstring_c_str(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s)
{
	bfc_wstrptr_t obj = (bfc_wstrptr_t) buf;
	int rc;

	if ((rc = bfc_init_wstring(obj, bufsize, pool)) < 0) {
		return(rc);
	}
	obj->buf = (wchar_t *) (intptr_t) s;
	obj->bufsize = obj->len = (*obj->vptr->traits->szlen)(s);
	return (rc);
}

int
bfc_init_wstring_fill(void *buf, size_t bufsize, struct mempool *pool,
				size_t n, wchar_t c)
{
	bfc_wstrptr_t s = (bfc_wstrptr_t) buf;
	int rc;

	if ((rc = bfc_init_wstring(s, bufsize, pool)) < 0) {
		return(rc);
	}

	RETURN_METHCALL(bfc_string_classptr_t, s,
			assign_fill, (s, n, c),
			bfc_wstring_assign_fill(s, n, c));
}

int
bfc_init_wstring_range(void *buf, size_t bufsize, struct mempool *pool,
				bfc_iterptr_t begin, bfc_iterptr_t end)
{
	bfc_wstrptr_t s = (bfc_wstrptr_t) buf;
	int rc;
	bfc_iterator_t zero;

	if ((rc = bfc_init_wstring(s, bufsize, pool)) < 0) {
		return(rc);
	}

	bfc_init_iterator(&zero, sizeof(zero), (bfc_objptr_t) s, 0);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, &zero, &zero, begin, end),
			bfc_wstring_replace_ranges(s,&zero,&zero,begin,end));
}

void
bfc_destroy_wstring(bfc_wstrptr_t obj)
{
	bfc_string_classptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

static int
clone_wstring(bfc_cwstrptr_t obj, void *buf, size_t bufsize)
{
	bfc_wstrptr_t s = (bfc_wstrptr_t) buf;
	size_t size = bfc_object_size(obj);
	if (bufsize < size) {
		return (-ENOSPC);
	}
	memcpy(s, obj, size);
	return (BFC_SUCCESS);
}

static unsigned  
wstring_hashcode(bfc_cwstrptr_t s)
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
wstring_equals(bfc_cwstrptr_t s, bfc_cwstrptr_t other)
{
	if (s == other) {
		return (1);
	}
	return (bfc_wstring_compare_bfstr(s, other) == 0);
}

#ifdef IMPLEMENT_TOSTRING
static int
wstring_tostring(bfc_cwstrptr_t s, char *buf, size_t bufsize)
{
	size_t n;

	n = bfc_utf8_from_wchar(buf, bufsize, bfc_wstrdata(s), bfc_wstrlen(s));

	return ((int) n);
}

static void
dump_wstring(bfc_cwstrptr_t s, int depth, struct l4sc_logger *log)
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
bfc_wstring_objsize(bfc_cwstrptr_t obj)
{
	return (sizeof(struct bfc_wstring));
}

// capacity:
size_t
bfc_wstring_length(bfc_cwstrptr_t s)
{
	return (s->len);
}

size_t
bfc_wstring_max_size(bfc_cwstrptr_t s)
{
	return ((0x7FFFuL << 8*(sizeof(wchar_t*)-2)) / sizeof(wchar_t));
}

int
bfc_wstring_resize(bfc_wstrptr_t s, size_t n, wchar_t c)
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
bfc_wstring_capacity(bfc_cwstrptr_t s)
{
	return ((s->bufsize != 0)? s->bufsize: s->len);
}

int
bfc_wstring_reserve(bfc_wstrptr_t s, size_t n)
{
	if (n >= bfc_wstring_max_size(s)) {
		return (-EINVAL);
	}
	if (s->offs + n < s->bufsize) {
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

// element access:
wchar_t *
bfc_wstring_index(bfc_wstrptr_t s, size_t pos)
{
	return (s->buf + s->offs + pos);
}

const wchar_t*
bfc_wstring_data(bfc_cwstrptr_t s)  /* not zero terminated */
{
	return (s->buf + s->offs);
}

/* Modifiers */
int
bfc_wstring_assign_bfstr(bfc_wstrptr_t s, bfc_cwstrptr_t s2)
{
	const wchar_t *data = bfc_wstrdata(s2);
	size_t len = bfc_wstrlen(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, assign_buffer, (s,data,len),
			bfc_wstring_assign_buffer(s, data, len));
}

int
bfc_wstring_assign_substr(bfc_wstrptr_t s, bfc_cwstrptr_t s2,
			 		size_t subpos, size_t sublen)
{
	const wchar_t *data = bfc_wstrdata(s2) + subpos;
	size_t len = bfc_wstring_sublen(s2, subpos, sublen);

	if ((subpos == BFC_NPOS) || (subpos > bfc_wstrlen(s2))) {
		return (-ERANGE);
	}

	RETURN_METHCALL(bfc_string_classptr_t, s, assign_buffer, (s,data,len),
			bfc_wstring_assign_buffer(s, data, len));
}

int
bfc_wstring_assign_c_str(bfc_wstrptr_t s, const wchar_t *s2)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, assign_buffer, (s, s2, n),
			bfc_wstring_assign_buffer(s, s2, n));
}

int
bfc_wstring_assign_buffer(bfc_wstrptr_t s, const wchar_t *s2, size_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	int rc;
	
	L4SC_TRACE(logger, "%s(%p, %p, %ld)", __FUNCTION__, s, s2, (long) n);

	if ((rc = bfc_wstr_reserve(s, n)) == BFC_SUCCESS) {
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

int
bfc_wstring_assign_fill(bfc_wstrptr_t s, size_t n, wchar_t c)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	int rc;
	
	L4SC_TRACE(logger, "%s(%p, %ld, %02x)", __FUNCTION__, s, (long) n, c);

	if ((rc = bfc_wstr_reserve(s, n)) == BFC_SUCCESS) {
		wchar_t *data = bfc_wstrbuf(s);
		if (n > 0) {
			(*s->vptr->traits->assign)(data, n, c);
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

int
bfc_wstring_assign_range(bfc_wstrptr_t s,bfc_iterptr_t first,bfc_iterptr_t last)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	const size_t len = bfc_wstrlen(s);
	bfc_iterator_t begin, limit;

	L4SC_TRACE(logger, "%s(%p, %p, %p)", __FUNCTION__, s, first, last);

	bfc_init_iterator(&begin, sizeof(begin), (bfc_objptr_t) s, 0);
	bfc_init_iterator(&limit, sizeof(limit), (bfc_objptr_t) s, len);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, &begin, &limit, first, last),
			bfc_wstring_replace_ranges(s,&begin,&limit,first,last));
}

int
bfc_wstring_append_bfstr(bfc_wstrptr_t s, bfc_cwstrptr_t s2)
{
	const wchar_t *data = bfc_wstrdata(s2);
	size_t len = bfc_wstrlen(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, append_buffer, (s,data,len),
			bfc_wstring_append_buffer(s, data, len));
}

int
bfc_wstring_append_substr(bfc_wstrptr_t s,
			  bfc_cwstrptr_t s2, size_t subpos, size_t sublen)
{
	const wchar_t *data = bfc_wstrdata(s2) + subpos;
	size_t len = bfc_wstring_sublen(s2, subpos, sublen);

	if ((subpos == BFC_NPOS) || (subpos > bfc_wstrlen(s2))) {
		return (-ERANGE);
	}

	RETURN_METHCALL(bfc_string_classptr_t, s, append_buffer, (s,data,len),
			bfc_wstring_append_buffer(s, data, len));
}

int
bfc_wstring_append_c_str(bfc_wstrptr_t s, const wchar_t *s2)
{
	size_t n = (*s->vptr->traits->szlen)(s2);
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);

	L4SC_TRACE(logger, "%s(%p, %p, %ld): %s, %s", __FUNCTION__,
		s, s2, (long) n, s->vptr->name, s->vptr->traits->name);

	RETURN_METHCALL(bfc_string_classptr_t, s, append_buffer, (s, s2, n),
			bfc_wstring_append_buffer(s, s2, n));
}

int
bfc_wstring_append_buffer(bfc_wstrptr_t s, const wchar_t *s2, size_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	int rc;
	
	L4SC_TRACE(logger, "%s(%p, %p, %ld)", __FUNCTION__, s, s2, (long) n);

	if ((rc = bfc_wstr_reserve(s, s->len + n)) == BFC_SUCCESS) {
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
bfc_wstring_append_fill(bfc_wstrptr_t s, size_t n, wchar_t c)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	int rc;
	
	L4SC_TRACE(logger, "%s(%p, %ld, %02x)", __FUNCTION__, s, (long) n, c);

	if ((rc = bfc_wstr_reserve(s, s->len + n)) == BFC_SUCCESS) {
		wchar_t *data = bfc_wstrbuf(s) + s->len;
		if (n > 0) {
			(*s->vptr->traits->assign)(data, n, c);
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
bfc_wstring_append_range(bfc_wstrptr_t s, iterptrT first, iterptrT last)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	const size_t len = bfc_wstrlen(s);
	bfc_iterator_t end;

	L4SC_TRACE(logger, "%s(%p, %p, %p)", __FUNCTION__, s, first, last);

	bfc_init_iterator(&end, sizeof(end), (bfc_objptr_t) s, len);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, &end, &end, first, last),
			bfc_wstring_replace_ranges(s, &end, &end, first, last));
}

int
bfc_wstring_push_back(bfc_wstrptr_t s, wchar_t c)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	int rc;
	
	L4SC_TRACE(logger, "%s(%p, %02x)", __FUNCTION__, s, c);

	if ((rc = bfc_wstr_reserve(s, s->len + 1)) == BFC_SUCCESS) {
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
bfc_wstring_insert_bfstr(bfc_wstrptr_t s, size_t pos, bfc_cwstrptr_t s2)
{
	const wchar_t *data = bfc_wstrdata(s2);
	size_t len = bfc_wstrlen(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, data, len),
			bfc_wstring_replace_buffer(s, pos, 0, data, len));
}

int
bfc_wstring_insert_substr(bfc_wstrptr_t s, size_t pos,
			bfc_cwstrptr_t s2, size_t subpos, size_t sublen)
{
	const wchar_t *data = bfc_wstrdata(s2) + subpos;
	size_t len = bfc_wstring_sublen(s2, subpos, sublen);

	if ((subpos == BFC_NPOS) || (subpos > bfc_wstrlen(s2))) {
		return (-ERANGE);
	}

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, data, len),
			bfc_wstring_replace_buffer(s, pos, 0, data, len));
}

int
bfc_wstring_insert_c_str(bfc_wstrptr_t s, size_t pos, const wchar_t *s2)
{
	size_t n = (*s->vptr->traits->szlen)(s2);
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);

	L4SC_TRACE(logger, "%s(%p, %p, %ld): %s, %s", __FUNCTION__,
		s, s2, (long) n, s->vptr->name, s->vptr->traits->name);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, s2, n),
			bfc_wstring_replace_buffer(s, pos, 0, s2, n));
}

int
bfc_wstring_insert_buffer(bfc_wstrptr_t s, size_t pos,
			  const wchar_t *s2, size_t n)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, s2, n),
			bfc_wstring_replace_buffer(s, pos, 0, s2, n));
}

int
bfc_wstring_insert_fill(bfc_wstrptr_t s, size_t pos, size_t n, wchar_t c)
{
	wchar_t *data = alloca(4*n+1);
	if (n > 0) {
		(*s->vptr->traits->assign)(data, n, c);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, data, n),
			bfc_wstring_replace_buffer(s, pos, 0, data, n));
}

int
bfc_wstring_insert_fillit(bfc_wstrptr_t s, iterptrT p, size_t n, wchar_t c)
{
	size_t pos;

	if (p->obj != (bfc_objptr_t) s) {
		return (-EFAULT);
	}
	pos = bfc_iterator_position(p);
	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			insert_fill, (s, pos, n, c),
			bfc_wstring_insert_fill(s, pos, n, c));
}

int
bfc_wstring_insert_char(bfc_wstrptr_t s, iterptrT p, wchar_t c)
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

int
bfc_wstring_insert_range(bfc_wstrptr_t s, iterptrT p,
			 iterptrT first, iterptrT last)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, p, p, first, last),
			bfc_wstring_replace_ranges(s, p, p, first, last));
}

int
bfc_wstring_erase_seq(bfc_wstrptr_t s, size_t pos, size_t len)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, len, NULL, 0),
			bfc_wstring_replace_buffer(s, pos, len, NULL, 0));
}

int
bfc_wstring_erase_tail(bfc_wstrptr_t s, size_t pos)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, BFC_NPOS, NULL, 0),
			bfc_wstring_replace_buffer(s, pos, BFC_NPOS, NULL, 0));
}

int
bfc_wstring_erase_char(bfc_wstrptr_t s, iterptrT p)
{
	size_t pos;

	if (p->obj != (bfc_objptr_t) s) {
		return (-EFAULT);
	}
	pos = bfc_iterator_position(p);
	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 1, NULL, 0),
			bfc_wstring_replace_buffer(s, pos, 1, NULL, 0));
}

int
bfc_wstring_erase_range(bfc_wstrptr_t s, iterptrT first, iterptrT last)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_range_buffer, (s, first, last, NULL, 0),
			bfc_wstring_replace_range_buffer(s,first,last,NULL,0));
}

void
bfc_wstring_pop_back(bfc_wstrptr_t s)
{
	if (bfc_wstrlen(s) > 0) {
		wchar_t *data = bfc_wstrbuf(s);
		data[--(s->len)] = '\0';
	}
}

int
bfc_wstring_replace_bfstr(bfc_wstrptr_t s, size_t pos1, size_t n1,
			  bfc_cwstrptr_t str)
{
	const wchar_t *data = bfc_wstrdata(str);
	size_t len = bfc_wstrlen(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos1, n1, data, len),
			bfc_wstring_replace_buffer(s, pos1, n1, data, len));
}

int
bfc_wstring_replace_substr(bfc_wstrptr_t s,size_t pos1,size_t n1,
	 			bfc_cwstrptr_t str, size_t pos2, size_t n2)
{
	const wchar_t *data = bfc_wstrdata(str) + pos2;
	size_t len = bfc_wstring_sublen(str, pos2, n2);

	if ((pos2 == BFC_NPOS) || (pos2 > bfc_wstrlen(str))) {
		return (-ERANGE);
	}

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos1, n1, data, len),
			bfc_wstring_replace_buffer(s, pos1, n1, data, len));
}

int
bfc_wstring_replace_buffer(bfc_wstrptr_t s, size_t pos, size_t n1,
			   const wchar_t* s2, size_t n2)
{
	const size_t len = bfc_wstrlen(s);
	size_t nkill = bfc_wstring_sublen(s, pos, n1);
	size_t ntail = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	int rc;
	
	L4SC_TRACE(logger, "%s(%p, %ld, %ld, %p, %ld)",
		__FUNCTION__, s, (long) pos, (long) n1, s2, (long) n2);

	if ((pos == BFC_NPOS) || (pos > len)) {
		L4SC_ERROR(logger, "%s: pos %ld behind len %ld",
			__FUNCTION__, (long) pos, (long) len);
		return (-ERANGE);
	}
	if ((n2 > nkill)
	 && ((rc = bfc_wstr_reserve(s, s->len + n2 - nkill)) != BFC_SUCCESS)) {
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
bfc_wstring_replace_c_str(bfc_wstrptr_t s, size_t pos, size_t n1,
					const wchar_t* s2)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, n1, s2, n),
			bfc_wstring_replace_buffer(s, pos, n1, s2, n));
}

int
bfc_wstring_replace_fill(bfc_wstrptr_t s, size_t pos, size_t n1,
					size_t n2, wchar_t c)
{
	wchar_t *data = alloca(4*n2+1);
	if (n2 > 0) {
		(*s->vptr->traits->assign)(data, n2, c);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, n1, data, n2),
			bfc_wstring_replace_buffer(s, pos, n1, data, n2));
}

int
bfc_wstring_replace_range_bfstr(bfc_wstrptr_t s, iterptrT i1,
					iterptrT i2, bfc_cwstrptr_t s2)
{
	const wchar_t *data = bfc_wstrdata(s2);
	size_t len = bfc_wstrlen(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_range_buffer, (s, i1, i2, data, len),
			bfc_wstring_replace_range_buffer(s, i1, i2, data, len));
}

int
bfc_wstring_replace_range_buffer(bfc_wstrptr_t s, iterptrT i1,
				iterptrT i2, const wchar_t* s2, size_t n)
{
	size_t pos;
	ptrdiff_t k;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
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
bfc_wstring_replace_range_c_str(bfc_wstrptr_t s, iterptrT i1,
					iterptrT i2, const wchar_t* s2)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_range_buffer, (s, i1, i2, s2, n),
			bfc_wstring_replace_range_buffer(s, i1, i2, s2, n));
}

int
bfc_wstring_replace_range_fill(bfc_wstrptr_t s, iterptrT i1,
					iterptrT i2, size_t n, wchar_t c)
{
	wchar_t *data = alloca(4*n+1);
	if (n > 0) {
		(*s->vptr->traits->assign)(data, n, c);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_range_buffer, (s, i1, i2, data, n),
			bfc_wstring_replace_range_buffer(s, i1, i2, data, n));
}

int
bfc_wstring_replace_ranges(bfc_wstrptr_t s, iterptrT i1, iterptrT i2,
					    iterptrT j1, iterptrT j2)
{
	wchar_t *data;
	const wchar_t *cp;
	size_t n = 0;
	ptrdiff_t d = bfc_iterator_distance(j1, j2);
	bfc_iterator_t it;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);

	L4SC_TRACE(logger, "%s(%p, %p, %p, %p, %p)",
			__FUNCTION__, s, i1, i2, j1, j2);

	if (d < 0) {
		L4SC_ERROR(logger, "%s: negative distance %ld",
						__FUNCTION__, (long)d);
		return (-EINVAL);
	}
	data = alloca(4*d+20);
	bfc_clone_object(j1, &it, sizeof(it)); 
	while ((n < (size_t) d) && !bfc_iterator_equals(&it, j2)) {
		RETVAR_METHCALL(cp, bfc_iterator_classptr_t, &it,
				first, (&it), NULL);
		if (cp) {
			data[n++] = *cp;
		} else {
			break;
		}
		VOID_METHCALL(bfc_iterator_classptr_t, &it, advance, (&it, 1));
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_range_buffer, (s, i1, i2, data, n),
			bfc_wstring_replace_range_buffer(s, i1, i2, data, n));
}

size_t
bfc_wstring_copy(bfc_cwstrptr_t s, wchar_t* s2, size_t n, size_t pos)
{
	const size_t len = bfc_wstring_sublen(s, pos, n);
	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(s))) {
		return ((size_t) -ERANGE);
	}
	if (len > 0) {
		const wchar_t *data = bfc_wstrdata(s);
		(*s->vptr->traits->copy)(s2, data+pos, len);
		return (len);
	}
	return (0);
}


/* String operations */
size_t
bfc_wstring_find_bfstr(bfc_cwstrptr_t s, bfc_cwstrptr_t str, size_t pos)
{
	const wchar_t *pattern = bfc_wstrdata(str);
	size_t len = bfc_wstrlen(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_buffer, (s, pattern, pos, len),
			bfc_wstring_find_buffer(s, pattern, pos, len));
}

size_t
bfc_wstring_find_buffer(bfc_cwstrptr_t s, const wchar_t* s2,
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
bfc_wstring_find_c_str(bfc_cwstrptr_t s, const wchar_t* s2, size_t pos)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, find_buffer, (s, s2, pos, n),
			bfc_wstring_find_buffer(s, s2, pos, n));
}

size_t
bfc_wstring_find_char(bfc_cwstrptr_t s, wchar_t c, size_t pos)
{
	const int len = bfc_wstrlen(s);
	const wchar_t *cp = NULL, *data = bfc_wstrdata(s);
	if (pos < len) {
		cp = (*s->vptr->traits->find)(data+pos, len-pos, c);
	}
	return (cp? cp - data: BFC_NPOS);
}

size_t
bfc_wstring_rfind_bfstr(bfc_cwstrptr_t s,bfc_cwstrptr_t str,size_t pos)
{
	const wchar_t *pattern = bfc_wstrdata(str);
	size_t patlen = bfc_wstrlen(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			rfind_buffer, (s, pattern, pos, patlen),
			bfc_wstring_rfind_buffer(s, pattern, pos, patlen));
}

size_t
bfc_wstring_rfind_buffer(bfc_cwstrptr_t s, const wchar_t* s2,
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
bfc_wstring_rfind_c_str(bfc_cwstrptr_t s, const wchar_t* s2, size_t pos)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, rfind_buffer, (s, s2, pos, n),
			bfc_wstring_rfind_buffer(s, s2, pos, n));
}

size_t
bfc_wstring_rfind_char(bfc_cwstrptr_t s, wchar_t c, size_t pos)
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
bfc_wstring_find_first_of_bfstr(bfc_cwstrptr_t s,bfc_cwstrptr_t str,size_t pos)
{
	const wchar_t *pattern = bfc_wstrdata(str);
	size_t patlen = bfc_wstrlen(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_first_of_buffer, (s, pattern, pos, patlen),
			bfc_wstring_find_first_of_buffer(s,pattern,pos,patlen));
}

size_t
bfc_wstring_find_first_of_buffer(bfc_cwstrptr_t s, const wchar_t* s2,
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
bfc_wstring_find_first_of_c_str(bfc_cwstrptr_t s, const wchar_t* s2, size_t pos)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_first_of_buffer, (s, s2, pos, n),
			bfc_wstring_find_first_of_buffer(s, s2, pos, n));
}

size_t
bfc_wstring_find_first_of_char(bfc_cwstrptr_t s, wchar_t c, size_t pos)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_char, (s, c, pos),
			bfc_wstring_find_char(s, c, pos));
}

size_t
bfc_wstring_find_last_of_bfstr(bfc_cwstrptr_t s, bfc_cwstrptr_t str, size_t pos)
{
	const wchar_t *pattern = bfc_wstrdata(str);
	size_t patlen = bfc_wstrlen(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_last_of_buffer, (s, pattern, pos, patlen),
			bfc_wstring_find_last_of_buffer(s,pattern,pos,patlen));
}

size_t
bfc_wstring_find_last_of_buffer(bfc_cwstrptr_t s, const wchar_t* s2,
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
bfc_wstring_find_last_of_c_str(bfc_cwstrptr_t s, const wchar_t* s2, size_t pos)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_last_of_buffer, (s, s2, pos, n),
			bfc_wstring_find_last_of_buffer(s, s2, pos, n));
}

size_t
bfc_wstring_find_last_of_char(bfc_cwstrptr_t s, wchar_t c, size_t pos)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			rfind_char, (s, c, pos),
			bfc_wstring_rfind_char(s, c, pos));
}

size_t
bfc_wstring_find_first_not_of_bfstr(bfc_cwstrptr_t s,bfc_cwstrptr_t str,
					size_t pos)
{
	const wchar_t *pattern = bfc_wstrdata(str);
	size_t patlen = bfc_wstrlen(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_first_not_of_buffer, (s, pattern, pos, patlen),
			bfc_wstring_find_first_not_of_buffer(s, pattern,
								pos, patlen));
}

size_t
bfc_wstring_find_first_not_of_buffer(bfc_cwstrptr_t s,const wchar_t* s2,
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
bfc_wstring_find_first_not_of_c_str(bfc_cwstrptr_t s, const wchar_t* s2,
					size_t pos)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_first_not_of_buffer, (s, s2, pos, n),
			bfc_wstring_find_first_not_of_buffer(s, s2, pos, n));
}

size_t
bfc_wstring_find_first_not_of_char(bfc_cwstrptr_t s, wchar_t c, size_t pos)
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
bfc_wstring_find_last_not_of_bfstr(bfc_cwstrptr_t s, bfc_cwstrptr_t str,
					size_t pos)
{
	const wchar_t *pattern = bfc_wstrdata(str);
	size_t patlen = bfc_wstrlen(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_last_not_of_buffer, (s, pattern, pos, patlen),
			bfc_wstring_find_last_not_of_buffer(s, pattern,
								pos, patlen));
}

size_t
bfc_wstring_find_last_not_of_buffer(bfc_cwstrptr_t s, const wchar_t* s2,
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
bfc_wstring_find_last_not_of_c_str(bfc_cwstrptr_t s, const wchar_t* s2,
					size_t pos)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_last_not_of_buffer, (s, s2, pos, n),
			bfc_wstring_find_last_not_of_buffer(s, s2, pos, n));
}

size_t
bfc_wstring_find_last_not_of_char(bfc_cwstrptr_t s, wchar_t c, size_t pos)
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
bfc_wstring_substr(bfc_cwstrptr_t s, size_t pos, size_t n,
		   void *buf, size_t bufsize)
{
	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			init_substr, (buf, bufsize, s, pos, n),
			bfc_init_wstring_substr(buf, bufsize, s, pos, n));
}

int
bfc_wstring_compare_bfstr(bfc_cwstrptr_t s, bfc_cwstrptr_t str)
{
	size_t n1 = bfc_wstrlen(s);
	size_t n2 = bfc_wstrlen(str);
	const wchar_t *s2 = bfc_wstrdata(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			compare_buffer, (s, 0, n1, s2, n2),
			bfc_wstring_compare_buffer(s, 0, n1, s2, n2));
}

int
bfc_wstring_compare_substr(bfc_cwstrptr_t s, size_t pos1, size_t n1,
					bfc_cwstrptr_t str)
{
	size_t n2 = bfc_wstrlen(str);
	const wchar_t *s2 = bfc_wstrdata(str);

	if ((pos1 == BFC_NPOS) || (pos1 > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			compare_buffer, (s, pos1, n1, s2, n2),
			bfc_wstring_compare_buffer(s, pos1, n1, s2, n2));
}

int
bfc_wstring_compare_substrs(bfc_cwstrptr_t s, size_t pos1, size_t n1,
				 bfc_cwstrptr_t str, size_t pos2, size_t n2)
{
	const size_t l2 = bfc_wstring_sublen(str, pos2, n2);
	const wchar_t *s2 = bfc_wstrdata(str) + ((l2 > 0)? pos2: 0);

	if ((pos1 == BFC_NPOS) || (pos1 > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	if ((pos2 == BFC_NPOS) || (pos2 > bfc_wstrlen(str))) {
		return (-ERANGE);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			compare_buffer, (s, pos1, n1, s2, l2),
			bfc_wstring_compare_buffer(s, pos1, n1, s2, l2));
}

int
bfc_wstring_compare_c_str(bfc_cwstrptr_t s, const wchar_t* s2)
{
	size_t n1 = bfc_wstrlen(s);
	size_t n2 = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			compare_buffer, (s, 0, n1, s2, n2),
			bfc_wstring_compare_buffer(s, 0, n1, s2, n2));
}

int
bfc_wstring_compare_substr_c_str(bfc_cwstrptr_t s, size_t pos1, size_t n1,
				 const wchar_t* s2)
{
	size_t n2 = (*s->vptr->traits->szlen)(s2);

	if ((pos1 == BFC_NPOS) || (pos1 > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			compare_buffer, (s, pos1, n1, s2, n2),
			bfc_wstring_compare_buffer(s, pos1, n1, s2, n2));
}

int
bfc_wstring_compare_buffer(bfc_cwstrptr_t s, size_t pos1, size_t n1,
			   const wchar_t* s2, size_t n2)
{
	const size_t l1 = bfc_wstring_sublen(s, pos1, n1);

	if ((pos1 == BFC_NPOS) || (pos1 > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	if ((l1 > 0) && (n2 > 0)) {
		const wchar_t *s1 = bfc_wstrdata(s) + pos1;
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

