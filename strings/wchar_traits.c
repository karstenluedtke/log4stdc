
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>

#include "barefootc/object.h"
#include "barefootc/string.h"

int	bfc_wchar_eq(wchar_t c, wchar_t d);		/* c == d */
int	bfc_wchar_lt(wchar_t c, wchar_t d);		/* c <  d */
size_t	bfc_wchar_length(const wchar_t *s);	/* strlen, wcslen */
wchar_t	bfc_wchar_assign(wchar_t *p, size_t n, wchar_t c); /* memset */
int	bfc_wchar_compare(const wchar_t *p, const wchar_t *q, size_t n);
const wchar_t* bfc_wchar_find(const wchar_t *p, size_t n, wchar_t c);
wchar_t *bfc_wchar_move(wchar_t *dest, const wchar_t *src, size_t n);
wchar_t *bfc_wchar_copy(wchar_t *dest, const wchar_t *src, size_t n);
int	bfc_wchar_eof(void);			/* EOF, WEOF */
int	bfc_wchar_not_eof(int c);
wchar_t	bfc_int_to_wchar(int c);
int	bfc_wchar_to_int(wchar_t c);
int	bfc_wchar_eq_int(int x, int y);

struct wchar_traits_class {
	BFC_CLASSHDR(const struct wchar_traits_class *, void *, const void *)
	BFC_CHAR_TRAITS_METHODS(wchar_t, int)
};

struct wchar_traits_class bfc_wchar_traits_class = {
	.super	= NULL,
	.name	= "wchar_traits",
	.eq	= bfc_wchar_eq,
	.lt	= bfc_wchar_lt,
	.szlen	= bfc_wchar_length,
	.assign	= bfc_wchar_assign,
	.compare= bfc_wchar_compare,
	.find	= bfc_wchar_find,
	.move	= bfc_wchar_move,
	.copy	= bfc_wchar_copy,
	.eof	= bfc_wchar_eof,
	.not_eof= bfc_wchar_not_eof,
	.to_char= bfc_int_to_wchar,
	.to_int	= bfc_wchar_to_int,
	.eq_int = bfc_wchar_eq_int,
};
	
int
bfc_wchar_eq(wchar_t c, wchar_t d)
{
	return (c == d);
}

int
bfc_wchar_lt(wchar_t c, wchar_t d)
{
	return (c < d);
}

size_t
bfc_wchar_length(const wchar_t *s)
{
	return (wcslen(s));
}

wchar_t
bfc_wchar_assign(wchar_t *p, size_t n, wchar_t c)
{
	wmemset(p, c, n);
	return (c);
}

int
bfc_wchar_compare(const wchar_t *p, const wchar_t *q, size_t n)
{
	return (wmemcmp(p, q, n));
}

const wchar_t*
bfc_wchar_find(const wchar_t *p, size_t n, wchar_t c)
{
	return (wmemchr(p, c, n));
}

wchar_t *
bfc_wchar_move(wchar_t *dest, const wchar_t *src, size_t n)
{
	wmemmove(dest, src, n);
	return (dest);
}

wchar_t *
bfc_wchar_copy(wchar_t *dest, const wchar_t *src, size_t n)
{
	wmemcpy(dest, src, n);
	return (dest);
}

int
bfc_wchar_eof(void)
{
	return (WEOF);
}

int
bfc_wchar_not_eof(int c)
{
	if (c == WEOF) {
		return (WEOF-1);
	} else {
		return (c);
	}
}

wchar_t
bfc_int_to_wchar(int c)
{
	return ((wchar_t) c);
}

int
bfc_wchar_to_int(wchar_t c)
{
	if (sizeof(c) == 2) {
		return ((unsigned short) c);
	} else {
		return ((unsigned) c);
	}
}

int
bfc_wchar_eq_int(int x, int y)
{
	if ((x == WEOF) || (y == WEOF)) {
		return (0);
	} else {
		return (x == y);
	}
}

