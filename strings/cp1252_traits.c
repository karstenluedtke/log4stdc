
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
char *	bfc_char_move(char *dest, const char *src, size_t n);
char *	bfc_char_copy(char *dest, const char *src, size_t n);
int	bfc_char_eof(void);			/* EOF, WEOF */
int	bfc_char_not_eof(int c);
int	bfc_char_eq_int(int x, int y);

char	bfc_unicode_to_cp1252(int c);
int	bfc_cp1252_to_unicode(char c);

struct char_traits_class {
	BFC_CLASSHDR(const struct char_traits_class *, void *, const void *)
	BFC_CHAR_TRAITS_METHODS(char, int)
};

struct char_traits_class bfc_cp1252_traits_class = {
	.super	= NULL,
	.name	= "char_traits",
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
	.to_char= bfc_unicode_to_cp1252,
	.to_int	= bfc_cp1252_to_unicode,
	.eq_int = bfc_char_eq_int,
};

unsigned short cp1252_0x80_0x9F_unicodes[32] = {
// http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1252.TXT
//
//    Name:     cp1252 to Unicode table
//    Unicode version: 2.0
//    Table version: 2.01
//    Table format:  Format A
//    Date:          04/15/98
//
//    Contact:       Shawn.Steele@microsoft.com
//
//    General notes: none
//
//    Format: Three tab-separated columns
//        Column #1 is the cp1252 code (in hex)
//        Column #2 is the Unicode (in hex as 0xXXXX)
//        Column #3 is the Unicode name (follows a comment sign, '#')
//
//    The entries are in cp1252 order
//
/* 0x00 .. 0x7F are mapped one-to-one */
/* 0x80 */	0x20AC,	/* EURO SIGN */
/* 0x81 */	0x0081,	/* UNDEFINED */
/* 0x82 */	0x201A,	/* SINGLE LOW-9 QUOTATION MARK */
/* 0x83 */	0x0192,	/* LATIN SMALL LETTER F WITH HOOK */
/* 0x84 */	0x201E,	/* DOUBLE LOW-9 QUOTATION MARK */
/* 0x85 */	0x2026,	/* HORIZONTAL ELLIPSIS */
/* 0x86 */	0x2020,	/* DAGGER */
/* 0x87 */	0x2021,	/* DOUBLE DAGGER */
/* 0x88 */	0x02C6,	/* MODIFIER LETTER CIRCUMFLEX ACCENT */
/* 0x89 */	0x2030,	/* PER MILLE SIGN */
/* 0x8A */	0x0160,	/* LATIN CAPITAL LETTER S WITH CARON */
/* 0x8B */	0x2039,	/* SINGLE LEFT-POINTING ANGLE QUOTATION MARK */
/* 0x8C */	0x0152,	/* LATIN CAPITAL LIGATURE OE */
/* 0x8D */	0x008D,	/* UNDEFINED */
/* 0x8E */	0x017D,	/* LATIN CAPITAL LETTER Z WITH CARON */
/* 0x8F */	0x008F,	/* UNDEFINED */
/* 0x90 */	0x0090,	/* UNDEFINED */
/* 0x91 */	0x2018,	/* LEFT SINGLE QUOTATION MARK */
/* 0x92 */	0x2019,	/* RIGHT SINGLE QUOTATION MARK */
/* 0x93 */	0x201C,	/* LEFT DOUBLE QUOTATION MARK */
/* 0x94 */	0x201D,	/* RIGHT DOUBLE QUOTATION MARK */
/* 0x95 */	0x2022,	/* BULLET */
/* 0x96 */	0x2013,	/* EN DASH */
/* 0x97 */	0x2014,	/* EM DASH */
/* 0x98 */	0x02DC,	/* SMALL TILDE */
/* 0x99 */	0x2122,	/* TRADE MARK SIGN */
/* 0x9A */	0x0161,	/* LATIN SMALL LETTER S WITH CARON */
/* 0x9B */	0x203A,	/* SINGLE RIGHT-POINTING ANGLE QUOTATION MARK */
/* 0x9C */	0x0153,	/* LATIN SMALL LIGATURE OE */
/* 0x9D */	0x009D,	/* UNDEFINED */
/* 0x9E */	0x017E,	/* LATIN SMALL LETTER Z WITH CARON */
/* 0x9F */	0x0178,	/* LATIN CAPITAL LETTER Y WITH DIAERESIS */
/* 0xA0 .. 0xFF are mapped one-to-one */
};

int
bfc_cp1252_to_unicode(char c)
{
	unsigned char idx;

	if ((c & ~0x7F) == 0) {
		return (c);
	} else if ((idx = (unsigned char) c) >= 0xA0) {
		return ((int) idx);
	} else {
		return (cp1252_0x80_0x9F_unicodes[idx & 0x1F]);
	}
}

char
bfc_unicode_to_cp1252(int c)
{
	unsigned char idx;

	if ((c & ~0x7F) == 0) {
		return ((char) c);
	} else if ((c >= 0x80) && (c <= 0x9F)) {
		idx = (unsigned char) c & 0x1F;
		if (c == (int) cp1252_0x80_0x9F_unicodes[idx]) {
			return (0x80 + idx);
		}
	} else if ((c & ~0xFF) == 0) {
		return ((char) c);
	}
	for (idx=0; idx < 32; idx++) {
		if (c == (int) cp1252_0x80_0x9F_unicodes[idx]) {
			return (0x80 + idx);
		}
	}
	return ('?');
}

