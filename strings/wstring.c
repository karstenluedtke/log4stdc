
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "barefootc/object.h"
#include "barefootc/string.h"

extern struct bfc_classhdr bfc_wchar_traits_class;

int	bfc_init_wstring(void *buf, size_t bufsize, struct mempool *pool);
void	bfc_destroy_wstring(bfc_wstrptr_t obj);
size_t	bfc_wstring_objsize(bfc_cwstrptr_t obj);
// capacity:
size_t	bfc_wstring_length(bfc_cwstrptr_t s);	
size_t	bfc_wstring_max_size(bfc_cwstrptr_t s);
int	bfc_wstring_resize(bfc_wstrptr_t s, size_t n, wchar_t c);
size_t	bfc_wstring_capacity(bfc_cwstrptr_t s);
int	bfc_wstring_reserve(bfc_wstrptr_t s, size_t n);
// element access:
wchar_t	bfc_wstring_at(bfc_cwstrptr_t s, size_t pos);
wchar_t *bfc_wstring_ref(bfc_wstrptr_t s, size_t pos);
const wchar_t *bfc_wstring_data(bfc_cwstrptr_t s);  /* not zero terminated */

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
				bfc_wstrptr_t, bfc_cwstrptr_t, wchar_t)
};

struct bfc_string_class bfc_wstring_class = {
	/* .super 	*/ NULL,
	/* .name 	*/ "wstring",
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
	/* .traits	*/ (void *) &bfc_wchar_traits_class,
	/* .size 	*/ bfc_wstring_length,
	/* .max_size	*/ bfc_wstring_max_size,
	/* .resize	*/ bfc_wstring_resize,
	/* .capacity	*/ bfc_wstring_capacity,
	/* .reserve	*/ bfc_wstring_reserve,
	/* .at		*/ bfc_wstring_at,
	/* .ref		*/ bfc_wstring_ref,
	/* .data	*/ bfc_wstring_data,
};

int
bfc_init_wstring(void *buf, size_t bufsize, struct mempool *pool)
{
	BFC_STRING_INIT_PROLOGUE(bfc_string_classptr_t,
			  bfc_wstrptr_t, s, buf, bufsize, pool,
			  &bfc_wstring_class);
	s->buf = L"";
	return (BFC_SUCCESS);
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

