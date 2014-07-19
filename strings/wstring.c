
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "log4stdc.h"

#define iterptrT void *

#ifndef STRING_CLASS_NAME
#define STRING_CLASS_NAME "wstring"
#endif

#define LOGGERNAME "barefootc.string", 16

extern struct bfc_classhdr bfc_wchar_traits_class;


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
	/* .clone 	*/ (void *) bfc_default_clone_object,
	/* .clonesize 	*/ bfc_wstring_objsize,
	/* .hashcode 	*/ (void *) bfc_default_get_object_hashcode,
	/* .equals 	*/ (void *) bfc_default_is_equal_object,
	/* .length 	*/ (void *) bfc_wstring_length,
	/* .tostring 	*/ (void *) bfc_default_object_tostring,
	/* .dump 	*/ (void *) bfc_default_dump_object,
	/* .spare13 	*/ NULL,
	/* .spare14 	*/ NULL,
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
	/* Element access */
	/* .at		*/ bfc_wstring_at,
	/* .ref		*/ bfc_wstring_ref,
	/* .data	*/ bfc_wstring_data,
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
	/* .swap		*/ bfc_wstring_swap,

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
	RETURN_METHCALL(bfc_string_classptr_t, s, size, (s), 0); 
}

wchar_t *
bfc_wstrbuf(bfc_cwstrptr_t s)
{
	return (wchar_t *)(uintptr_t) bfc_wstrdata(s);
}

const wchar_t *
bfc_wstrdata(bfc_cwstrptr_t s)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, data, (s), s->buf); 
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
	int rc;
	rc = bfc_init_wstring(buf, bufsize, pool);
	return (rc);
}

int
bfc_init_wstring_range(void *buf, size_t bufsize, struct mempool *pool,
				iterptrT begin, iterptrT end)
{
	int rc;
	rc = bfc_init_wstring(buf, bufsize, pool);
	return (rc);
}

void
bfc_destroy_wstring(bfc_wstrptr_t obj)
{
	bfc_string_classptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

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
	if (n <= s->len) {
		wchar_t *data = bfc_wstrbuf(s);
		s->len = n;
		data[n] = '\0';
		return ((int) n);
	} else if (bfc_str_reserve == BFC_SUCCESS) {
		wchar_t *data = bfc_wstrbuf(s);
		(*s->vptr->traits->assign)(data + s->len, n - s->len, c);
		data[n] = '\0';
		return ((int) n);
	}
	return (-ENOSPC);
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
wchar_t
bfc_wstring_at(bfc_cwstrptr_t s, size_t pos)
{
	return (s->buf[s->offs + pos]);
}

wchar_t *
bfc_wstring_ref(bfc_wstrptr_t s, size_t pos)
{
	return (s->buf + s->offs + pos);
}

const wchar_t*
bfc_wstring_data(bfc_cwstrptr_t s)  /* not zero terminated */
{
	return (s->buf + s->offs);
}

/* Modifiers */
bfc_wstrptr_t
bfc_wstring_assign_bfstr(bfc_wstrptr_t s, bfc_cwstrptr_t s2)
{
	const wchar_t *data = bfc_wstrdata(s2);
	size_t len = bfc_wstrlen(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, assign_buffer, (s,data,len),
			bfc_wstring_assign_buffer(s, data, len));
}

bfc_wstrptr_t
bfc_wstring_assign_substr(bfc_wstrptr_t s, bfc_cwstrptr_t s2,
			 		size_t subpos, size_t sublen)
{
	const wchar_t *data = bfc_wstrdata(s2) + subpos;
	size_t len = bfc_wstring_sublen(s2, subpos, sublen);

	RETURN_METHCALL(bfc_string_classptr_t, s, assign_buffer, (s,data,len),
			bfc_wstring_assign_buffer(s, data, len));
}

bfc_wstrptr_t
bfc_wstring_assign_c_str(bfc_wstrptr_t s, const wchar_t *s2)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, assign_buffer, (s, s2, n),
			bfc_wstring_assign_buffer(s, s2, n));
}

bfc_wstrptr_t
bfc_wstring_assign_buffer(bfc_wstrptr_t s, const wchar_t *s2, size_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %p, %ld)", __FUNCTION__, s, s2, (long) n);

	if (bfc_wstr_reserve(s, n) == BFC_SUCCESS) {
		wchar_t *data = bfc_wstrbuf(s);
		if (n > 0) {
			(*s->vptr->traits->copy)(data, s2, n);
		}
		data[n] = '\0';
		s->len = n;
		return (s);
	} else {
		L4SC_ERROR(logger, "%s: no space for %ld characters",
						__FUNCTION__, (long) n);
	}
	return (NULL);
}

bfc_wstrptr_t
bfc_wstring_assign_fill(bfc_wstrptr_t s, size_t n, wchar_t c)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, %02x)", __FUNCTION__, s, (long) n, c);

	if (bfc_wstr_reserve(s, n) == BFC_SUCCESS) {
		wchar_t *data = bfc_wstrbuf(s);
		if (n > 0) {
			(*s->vptr->traits->assign)(data, n, c);
		}
		data[n] = '\0';
		s->len = n;
		return (s);
	} else {
		L4SC_ERROR(logger, "%s: no space for %ld characters",
						__FUNCTION__, (long) n);
	}
	return (NULL);
}

bfc_wstrptr_t
bfc_wstring_assign_range(bfc_wstrptr_t s, iterptrT first, iterptrT last)
{
	return (NULL);
}

bfc_wstrptr_t
bfc_wstring_append_bfstr(bfc_wstrptr_t s, bfc_cwstrptr_t s2)
{
	const wchar_t *data = bfc_wstrdata(s2);
	size_t len = bfc_wstrlen(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, append_buffer, (s,data,len),
			bfc_wstring_append_buffer(s, data, len));
}

bfc_wstrptr_t
bfc_wstring_append_substr(bfc_wstrptr_t s,
			  bfc_cwstrptr_t s2, size_t subpos, size_t sublen)
{
	const wchar_t *data = bfc_wstrdata(s2) + subpos;
	size_t len = bfc_wstring_sublen(s2, subpos, sublen);

	RETURN_METHCALL(bfc_string_classptr_t, s, append_buffer, (s,data,len),
			bfc_wstring_append_buffer(s, data, len));
}

bfc_wstrptr_t
bfc_wstring_append_c_str(bfc_wstrptr_t s, const wchar_t *s2)
{
	size_t n = (*s->vptr->traits->szlen)(s2);
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);

	L4SC_TRACE(logger, "%s(%p, %p, %ld): %s, %s", __FUNCTION__,
		s, s2, (long) n, s->vptr->name, s->vptr->traits->name);

	RETURN_METHCALL(bfc_string_classptr_t, s, append_buffer, (s, s2, n),
			bfc_wstring_append_buffer(s, s2, n));
}

bfc_wstrptr_t
bfc_wstring_append_buffer(bfc_wstrptr_t s, const wchar_t *s2, size_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %p, %ld)", __FUNCTION__, s, s2, (long) n);

	if (bfc_wstr_reserve(s, s->len + n) == BFC_SUCCESS) {
		wchar_t *data = bfc_wstrbuf(s) + s->len;
		if (n > 0) {
			(*s->vptr->traits->copy)(data, s2, n);
		}
		data[n] = '\0';
		s->len += n;
		L4SC_DEBUG(logger, "%s: len %ld", __FUNCTION__, (long) s->len);
		return (s);
	} else {
		L4SC_ERROR(logger, "%s: no space for %ld+%ld characters",
				__FUNCTION__, (long) s->len, (long) n);
	}
	return (NULL);
}

bfc_wstrptr_t
bfc_wstring_append_fill(bfc_wstrptr_t s, size_t n, wchar_t c)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, %02x)", __FUNCTION__, s, (long) n, c);

	if (bfc_wstr_reserve(s, s->len + n) == BFC_SUCCESS) {
		wchar_t *data = bfc_wstrbuf(s) + s->len;
		if (n > 0) {
			(*s->vptr->traits->assign)(data, n, c);
		}
		data[n] = '\0';
		s->len += n;
		L4SC_DEBUG(logger, "%s: len %ld", __FUNCTION__, (long) s->len);
		return (s);
	} else {
		L4SC_ERROR(logger, "%s: no space for %ld+%ld characters",
				__FUNCTION__, (long) s->len, (long) n);
	}
	return (NULL);
}

bfc_wstrptr_t
bfc_wstring_append_range(bfc_wstrptr_t s, iterptrT first, iterptrT last)
{
	return (NULL);
}

int
bfc_wstring_push_back(bfc_wstrptr_t s, wchar_t c)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %02x)", __FUNCTION__, s, c);

	if (bfc_wstr_reserve(s, s->len + 1) == BFC_SUCCESS) {
		wchar_t *data = bfc_wstrbuf(s) + s->len;
		data[0] = c;
		data[1] = '\0';
		s->len += 1;
		return (c);
	} else {
		L4SC_ERROR(logger, "%s: no space for %ld+1 characters",
						__FUNCTION__, (long) s->len);
	}
	return (-1);
}

bfc_wstrptr_t
bfc_wstring_insert_bfstr(bfc_wstrptr_t s, size_t pos, bfc_cwstrptr_t s2)
{
	const wchar_t *data = bfc_wstrdata(s2);
	size_t len = bfc_wstrlen(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, data, len),
			bfc_wstring_replace_buffer(s, pos, 0, data, len));
}

bfc_wstrptr_t
bfc_wstring_insert_substr(bfc_wstrptr_t s, size_t pos,
			bfc_cwstrptr_t s2, size_t subpos, size_t sublen)
{
	const wchar_t *data = bfc_wstrdata(s2) + subpos;
	size_t len = bfc_wstring_sublen(s2, subpos, sublen);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, data, len),
			bfc_wstring_replace_buffer(s, pos, 0, data, len));
}

bfc_wstrptr_t
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

bfc_wstrptr_t
bfc_wstring_insert_buffer(bfc_wstrptr_t s, size_t pos,
			  const wchar_t *s2, size_t n)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, s2, n),
			bfc_wstring_replace_buffer(s, pos, 0, s2, n));
}

bfc_wstrptr_t
bfc_wstring_insert_fill(bfc_wstrptr_t s, size_t pos, size_t n, wchar_t c)
{
	wchar_t *data = alloca(4*n+1);
	if (n > 0) {
		(*s->vptr->traits->assign)(data, n, c);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, data, n),
			bfc_wstring_replace_buffer(s, pos, 0, data, n));
	return (NULL);
}

bfc_wstrptr_t
bfc_wstring_insert_fillit(bfc_wstrptr_t s, iterptrT p, size_t n, wchar_t c)
{
	return (NULL);
}

iterptrT
bfc_wstring_insert_char(bfc_wstrptr_t s, iterptrT p, wchar_t c)
{
	return (NULL);
}

bfc_wstrptr_t
bfc_wstring_insert_range(bfc_wstrptr_t s, iterptrT p,
			 iterptrT first, iterptrT last)
{
	return (NULL);
}

bfc_wstrptr_t
bfc_wstring_erase_seq(bfc_wstrptr_t s, size_t pos, size_t len)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, len, NULL, 0),
			bfc_wstring_replace_buffer(s, pos, len, NULL, 0));
}

bfc_wstrptr_t
bfc_wstring_erase_tail(bfc_wstrptr_t s, size_t pos)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, BFC_NPOS, NULL, 0),
			bfc_wstring_replace_buffer(s, pos, BFC_NPOS, NULL, 0));
}

iterptrT
bfc_wstring_erase_char(bfc_wstrptr_t s, iterptrT p)
{
	return (NULL);
}

bfc_wstrptr_t
bfc_wstring_erase_range(bfc_wstrptr_t s, iterptrT first, iterptrT last)
{
	return (NULL);
}

void
bfc_wstring_pop_back(bfc_wstrptr_t s)
{
	if (bfc_wstrlen(s) > 0) {
		wchar_t *data = bfc_wstrbuf(s);
		data[--(s->len)] = '\0';
	}
}

bfc_wstrptr_t
bfc_wstring_replace_bfstr(bfc_wstrptr_t s, size_t pos1, size_t n1,
			  bfc_cwstrptr_t str)
{
	const wchar_t *data = bfc_wstrdata(str);
	size_t len = bfc_wstrlen(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos1, n1, data, len),
			bfc_wstring_replace_buffer(s, pos1, n1, data, len));
}

bfc_wstrptr_t bfc_wstring_replace_substr(bfc_wstrptr_t s,size_t pos1,size_t n1,
	 			bfc_cwstrptr_t str, size_t pos2, size_t n2)
{
	const wchar_t *data = bfc_wstrdata(str) + pos2;
	size_t len = bfc_wstring_sublen(str, pos2, n2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos1, n1, data, len),
			bfc_wstring_replace_buffer(s, pos1, n1, data, len));
}

bfc_wstrptr_t
bfc_wstring_replace_buffer(bfc_wstrptr_t s, size_t pos, size_t n1,
			   const wchar_t* s2, size_t n2)
{
	size_t nkill = bfc_wstring_sublen(s, pos, n1);
	size_t ntail = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	
	L4SC_TRACE(logger, "%s(%p, %ld, %ld, %p, %ld)",
		__FUNCTION__, s, (long) pos, (long) n1, s2, (long) n2);

	if ((n2 > nkill)
	 && (bfc_wstr_reserve(s, s->len + n2 - nkill) != BFC_SUCCESS)) {
		L4SC_ERROR(logger, "%s: no space for %ld-%ld+%ld characters",
			__FUNCTION__, (long) s->len, (long) nkill, (long) n2);
		return (NULL);
	} else {
		const size_t len = bfc_wstrlen(s);
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
		return (s);
	}
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_replace_c_str(bfc_wstrptr_t s, size_t pos, size_t n1,
					const wchar_t* s2)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, n1, s2, n),
			bfc_wstring_replace_buffer(s, pos, n1, s2, n));
}

bfc_wstrptr_t bfc_wstring_replace_fill(bfc_wstrptr_t s, size_t pos, size_t n1,
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

bfc_wstrptr_t bfc_wstring_replace_range_bfstr(bfc_wstrptr_t s, iterptrT i1,
					iterptrT i2, bfc_cwstrptr_t s2)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_replace_range_buffer(bfc_wstrptr_t s, iterptrT i1,
				iterptrT i2, const wchar_t* s2, size_t n)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_replace_range_c_str(bfc_wstrptr_t s, iterptrT i1,
					iterptrT i2, const wchar_t* s2)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_replace_range_fill(bfc_wstrptr_t s, iterptrT i1,
					iterptrT i2, size_t n, wchar_t c)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_replace_ranges(bfc_wstrptr_t s, iterptrT i1,
					iterptrT i2, iterptrT j1, iterptrT j2)
{
	return (NULL);
}

size_t	bfc_wstring_copy(bfc_wstrptr_t s, wchar_t* s2, size_t n, size_t pos)
{
	return (0);
}

void	bfc_wstring_swap(bfc_wstrptr_t s, bfc_wstrptr_t str)
{
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
		return (0);
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
		return (len);
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
			if ((*s->vptr->traits->eq)(*cp, c) == 0) {
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
	return (0);
}

bfc_wstrptr_t
bfc_wstring_substr(bfc_cwstrptr_t s, size_t pos, size_t n,
		   bfc_wstrptr_t buf, size_t bufsize)
{
	int rc;
	RETVAR_METHCALL(rc, bfc_string_classptr_t, s,
			init_substr, (buf, bufsize, s, pos, n),
			bfc_init_wstring_substr(buf, bufsize, s, pos, n));
	if (rc < 0) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
		L4SC_ERROR(logger, "%s(%p, %ld, %ld): init error %d",
				__FUNCTION__, s, (long) pos, (long) n, rc);
		return (NULL);
	}
	return (buf);
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

	RETURN_METHCALL(bfc_string_classptr_t, s,
			compare_buffer, (s, pos1, n1, s2, n2),
			bfc_wstring_compare_buffer(s, pos1, n1, s2, n2));
}

int
bfc_wstring_compare_buffer(bfc_cwstrptr_t s, size_t pos1, size_t n1,
			   const wchar_t* s2, size_t n2)
{
	const size_t l1 = bfc_wstring_sublen(s, pos1, n1);

	if ((l1 > 0) && (n2 > 0)) {
		const wchar_t *s1 = bfc_wstrdata(s) + pos1;
		if (l1 == n2) {
			return ((*s->vptr->traits->compare)(s1, s2, l1));
		} else if (l1 < n2) {
			int rc = (*s->vptr->traits->compare)(s1, s2, l1);
			return ((rc != 0)? rc: -1);
		} else {
			int rc = (*s->vptr->traits->compare)(s1, s2, n2);
			return ((rc != 0)? rc: 1);
		}
	} else if (l1 > 0) {
		return (1);
	} else if (n2 > 0) {
		return (-1);
	}
	return (0);
}

