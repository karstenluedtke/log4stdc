
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "barefootc/object.h"
#include "barefootc/string.h"

int	bfc_char_eq(char c, char d);		/* c == d */
int	bfc_char_lt(char c, char d);		/* c <  d */
size_t	bfc_char_length(const char *s);	/* strlen, wcslen */
char	bfc_char_assign(char *p, size_t n, char c); /* memset */
int	bfc_char_compare(const char *p, const char *q, size_t n);
const char* bfc_char_find(const char *p, size_t n, char c);
char *bfc_char_move(char *dest, const char *src, size_t n);
char *bfc_char_copy(char *dest, const char *src, size_t n);
int	bfc_char_eof(void);			/* EOF, WEOF */
int	bfc_char_not_eof(int c);
char	bfc_int_to_char(int c);
int	bfc_char_to_int(char c);
int	bfc_char_eq_int(int x, int y);

struct charraits_class {
	BFC_CLASSHDR(const struct charraits_class *, void *, const void *)
	BFC_CHAR_TRAITS_METHODS(char, int)
};

struct charraits_class bfc_char_traits_class = {
	.super	= NULL,
	.name	= "charraits",
	.eq	= bfc_char_eq,
	.lt	= bfc_char_lt,
	.szlen	= bfc_char_length,
	.assign	= bfc_char_assign,
	.compare= bfc_char_compare,
	.find	= bfc_char_find,
	.move	= bfc_char_move,
	.copy	= bfc_char_copy,
	.eof	= bfc_char_eof,
	.not_eof= bfc_char_not_eof,
	.to_char= bfc_int_to_char,
	.to_int	= bfc_char_to_int,
	.eq_int = bfc_char_eq_int,
};
	
int
bfc_char_eq(char c, char d)
{
	return (c == d);
}

int
bfc_char_lt(char c, char d)
{
	return (c < d);
}

size_t
bfc_char_length(const char *s)
{
	return (strlen(s));
}

char
bfc_char_assign(char *p, size_t n, char c)
{
	memset(p, c, n);
	return (c);
}

int
bfc_char_compare(const char *p, const char *q, size_t n)
{
	return (memcmp(p, q, n));
}

const char*
bfc_char_find(const char *p, size_t n, char c)
{
	return (memchr(p, c, n));
}

char *
bfc_char_move(char *dest, const char *src, size_t n)
{
	memmove(dest, src, n);
	return (dest);
}

char *
bfc_char_copy(char *dest, const char *src, size_t n)
{
	memcpy(dest, src, n);
	return (dest);
}

int
bfc_char_eof(void)
{
	return (EOF);
}

int
bfc_char_not_eof(int c)
{
	if (c == EOF) {
		return (0x1FF);
	} else {
		return (c);
	}
}

char
bfc_int_to_char(int c)
{
	return ((char) c);
}

int
bfc_char_to_int(char c)
{
	return ((unsigned char) c);
}

int
bfc_char_eq_int(int x, int y)
{
	if ((x == EOF) || (y == EOF)) {
		return (0);
	} else {
		return (x == y);
	}
}

