
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/string.h"

#define iterptrT void *

#ifndef STRING_CLASS_NAME
#define STRING_CLASS_NAME "wstring"
#endif

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

int
bfc_init_wstring(void *buf, size_t bufsize, struct mempool *pool)
{
	static const long zbuf = 0;

	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_wstrptr_t, s, buf, bufsize, pool,
			  &bfc_wstring_class);
	s->buf = (wchar_t *) &zbuf;
	return (BFC_SUCCESS);
}

int
bfc_init_wstring_bfstr(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cwstrptr_t str)
{
	int rc;
	rc = bfc_init_wstring(buf, bufsize, pool);
	return (rc);
}

int
bfc_init_wstring_move(void *buf, size_t bufsize, struct mempool *pool,
				bfc_wstrptr_t str)
{
	int rc;
	rc = bfc_init_wstring(buf, bufsize, pool);
	return (rc);
}

int
bfc_init_wstring_substr(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cwstrptr_t str, size_t pos, size_t n)
{
	int rc;
	rc = bfc_init_wstring(buf, bufsize, pool);
	return (rc);
}

int
bfc_init_wstring_buffer(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s, size_t n)
{
	bfc_wstrptr_t obj = (bfc_wstrptr_t) buf;
	int rc;

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
	return (bfc_wstring_capacity(s));
}

int
bfc_wstring_resize(bfc_wstrptr_t s, size_t n, wchar_t c)
{
	return (0);
}

size_t
bfc_wstring_capacity(bfc_cwstrptr_t s)
{
	return ((s->bufsize != 0)? s->bufsize: s->len);
}

int
bfc_wstring_reserve(bfc_wstrptr_t s, size_t n)
{
	return (0);
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

const wchar_t* bfc_wstring_data(bfc_cwstrptr_t s)  /* not zero terminated */
{
	return (s->buf + s->offs);
}

/* Modifiers */
bfc_wstrptr_t bfc_wstring_assign_bfstr(bfc_wstrptr_t s, bfc_cwstrptr_t s2)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_assign_substr(bfc_wstrptr_t s, bfc_cwstrptr_t s2,
			 		size_t subpos, size_t sublen)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_assign_c_str(bfc_wstrptr_t s, const wchar_t *s2)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_assign_buffer(bfc_wstrptr_t s,
					const wchar_t *s2, size_t n)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_assign_fill(bfc_wstrptr_t s, size_t n, wchar_t c)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_assign_range(bfc_wstrptr_t s,
					iterptrT first, iterptrT last)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_append_bfstr(bfc_wstrptr_t s, bfc_cwstrptr_t s2)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_append_substr(bfc_wstrptr_t s, bfc_cwstrptr_t s2,
			 		size_t subpos, size_t sublen)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_append_c_str(bfc_wstrptr_t s, const wchar_t *s2)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_append_buffer(bfc_wstrptr_t s,
					const wchar_t *s2, size_t n)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_append_fill(bfc_wstrptr_t s, size_t n, wchar_t c)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_append_range(bfc_wstrptr_t s,
					iterptrT first, iterptrT last)
{
	return (NULL);
}

int	bfc_wstring_push_back(bfc_wstrptr_t s, wchar_t c)
{
	return (0);
}

bfc_wstrptr_t bfc_wstring_insert_bfstr(bfc_wstrptr_t s, size_t pos,
					bfc_cwstrptr_t s2)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_insert_substr(bfc_wstrptr_t s, size_t pos,
			bfc_cwstrptr_t s2, size_t subpos, size_t sublen)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_insert_c_str(bfc_wstrptr_t s, size_t pos,
					const wchar_t *s2)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_insert_buffer(bfc_wstrptr_t s, size_t pos,
			 		const wchar_t *s2, size_t n)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_insert_fill(bfc_wstrptr_t s, size_t pos,
					size_t n, wchar_t c)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_insert_fillit(bfc_wstrptr_t s, iterptrT p,
					size_t n, wchar_t c)
{
	return (NULL);
}

iterptrT      bfc_wstring_insert_char(bfc_wstrptr_t s, iterptrT p, wchar_t c)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_insert_range(bfc_wstrptr_t s, iterptrT p,
					iterptrT first, iterptrT last)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_erase_seq(bfc_wstrptr_t s, size_t pos, size_t len)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_erase_tail(bfc_wstrptr_t s, size_t pos)
{
	return (NULL);
}

iterptrT      bfc_wstring_erase_char(bfc_wstrptr_t s, iterptrT p)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_erase_range(bfc_wstrptr_t s,
					iterptrT first, iterptrT last)
{
	return (NULL);
}

void          bfc_wstring_pop_back(bfc_wstrptr_t s)
{
}

bfc_wstrptr_t bfc_wstring_replace_bfstr(bfc_wstrptr_t s, size_t pos1,size_t n1,
					bfc_cwstrptr_t str)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_replace_substr(bfc_wstrptr_t s,size_t pos1,size_t n1,
	 			bfc_cwstrptr_t str, size_t pos2, size_t n2)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_replace_buffer(bfc_wstrptr_t s, size_t pos,size_t n1,
					const wchar_t* s2, size_t n2)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_replace_c_str(bfc_wstrptr_t s, size_t pos, size_t n1,
					const wchar_t* s2)
{
	return (NULL);
}

bfc_wstrptr_t bfc_wstring_replace_fill(bfc_wstrptr_t s, size_t pos, size_t n1,
					size_t n2, wchar_t c)
{
	return (NULL);
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
size_t	bfc_wstring_find_bfstr(bfc_wstrptr_t s,bfc_cwstrptr_t str, size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_buffer(bfc_wstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n)
{
	return (0);
}

size_t	bfc_wstring_find_c_str(bfc_wstrptr_t s, const wchar_t* s2, size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_char(bfc_wstrptr_t s, wchar_t c, size_t pos)
{
	return (0);
}

size_t	bfc_wstring_rfind_bfstr(bfc_wstrptr_t s,bfc_cwstrptr_t str,size_t pos)
{
	return (0);
}

size_t	bfc_wstring_rfind_buffer(bfc_wstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n)
{
	return (0);
}

size_t	bfc_wstring_rfind_c_str(bfc_wstrptr_t s, const wchar_t* s2,size_t pos)
{
	return (0);
}

size_t	bfc_wstring_rfind_char(bfc_wstrptr_t s, wchar_t c, size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_first_of_bfstr(bfc_wstrptr_t s, bfc_cwstrptr_t str,
					size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_first_of_buffer(bfc_wstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n)
{
	return (0);
}

size_t	bfc_wstring_find_first_of_c_str(bfc_wstrptr_t s, const wchar_t* s2,
					size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_first_of_char(bfc_wstrptr_t s, wchar_t c, size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_last_of_bfstr(bfc_wstrptr_t s, bfc_cwstrptr_t str,
					size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_last_of_buffer(bfc_wstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n)
{
	return (0);
}

size_t	bfc_wstring_find_last_of_c_str(bfc_wstrptr_t s, const wchar_t* s2,
					size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_last_of_char(bfc_wstrptr_t s, wchar_t c, size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_first_not_of_bfstr(bfc_wstrptr_t s,bfc_cwstrptr_t str,
					size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_first_not_of_buffer(bfc_wstrptr_t s,const wchar_t* s2,
					size_t pos, size_t n)
{
	return (0);
}

size_t	bfc_wstring_find_first_not_of_c_str(bfc_wstrptr_t s, const wchar_t* s2,
					size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_first_not_of_char(bfc_wstrptr_t s, wchar_t c,
					size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_last_not_of_bfstr(bfc_wstrptr_t s, bfc_cwstrptr_t str,
					size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_last_not_of_buffer(bfc_wstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n)
{
	return (0);
}

size_t	bfc_wstring_find_last_not_of_c_str(bfc_wstrptr_t s, const wchar_t* s2,
					size_t pos)
{
	return (0);
}

size_t	bfc_wstring_find_last_not_of_char(bfc_wstrptr_t s, wchar_t c,
					size_t pos)
{
	return (0);
}

bfc_wstrptr_t bfc_wstring_substr(bfc_wstrptr_t s, size_t pos, size_t n,
					bfc_wstrptr_t buf)
{
	return (0);
}

int	bfc_wstring_compare_bfstr(bfc_wstrptr_t s, bfc_cwstrptr_t str)
{
	return (0);
}

int	bfc_wstring_compare_substr(bfc_wstrptr_t s, size_t pos1, size_t n1,
					bfc_cwstrptr_t str)
{
	return (0);
}

int	bfc_wstring_compare_substrs(bfc_wstrptr_t s, size_t pos1, size_t n1,
				 bfc_cwstrptr_t str, size_t pos2, size_t n2)
{
	return (0);
}

int	bfc_wstring_compare_c_str(bfc_wstrptr_t s, const wchar_t* s2)
{
	return (0);
}

int	bfc_wstring_compare_substr_c_str(bfc_wstrptr_t s, size_t pos1,
					size_t n1, const wchar_t* s2)
{
	return (0);
}

int	bfc_wstring_compare_buffer(bfc_wstrptr_t s, size_t pos1, size_t n1,
					const wchar_t* s2, size_t n2)
{
	return (0);
}

