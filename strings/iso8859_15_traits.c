
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

char	bfc_unicode_to_iso8859_15(int c);
int	bfc_iso8859_15_to_unicode(char c);

struct char_traits_class {
	BFC_CLASSHDR(const struct char_traits_class *, void *, const void *)
	BFC_CHAR_TRAITS_METHODS(char, int)
};

struct char_traits_class bfc_iso8859_15_traits_class = {
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
	.to_char= bfc_unicode_to_iso8859_15,
	.to_int	= bfc_iso8859_15_to_unicode,
	.eq_int = bfc_char_eq_int,
};

unsigned short iso8859_15_0xA0_0xBF_unicodes[32] = {
// http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-15.TXT
//
//	Name:             ISO/IEC 8859-15:1999 to Unicode
//	Unicode version:  3.0
//	Table version:    1.0
//	Table format:     Format A
//	Date:             1999 July 27
//	Authors:          Markus Kuhn <http://www.cl.cam.ac.uk/~mgk25/>
//			  Ken Whistler <kenw@sybase.com>
//
//	Copyright (c) 1998 - 1999 Unicode, Inc.  All Rights reserved.
//
//	This file is provided as-is by Unicode, Inc. (The Unicode Consortium).
//	No claims are made as to fitness for any particular purpose.  No
//	warranties of any kind are expressed or implied.  The recipient
//	agrees to determine applicability of information provided.  If this
//	file has been provided on optical media by Unicode, Inc., the sole
//	remedy for any claim will be exchange of defective media within 90
//	days of receipt.
//
//	Unicode, Inc. hereby grants the right to freely use the information
//	supplied in this file in the creation of products supporting the
//	Unicode Standard, and to make copies of this file in any form for
//	internal or external distribution as long as this notice remains
//	attached.
//
//	General notes:
//
//	This table contains the data the Unicode Consortium has on how
//       ISO/IEC 8859-15:1999 characters map into Unicode.
//
//	Format:  Three tab-separated columns
//		 Column #1 is the ISO/IEC 8859-15 code (in hex as 0xXX)
//		 Column #2 is the Unicode (in hex as 0xXXXX)
//		 Column #3 the Unicode name (follows a comment sign, '#')
//
//	The entries are in ISO/IEC 8859-15 order.
//
/* 0x00 .. 0x9F are mapped one-to-one */
/* 0xA0 */	0x00A0,	/*	NO-BREAK SPACE */
/* 0xA1 */	0x00A1,	/*	INVERTED EXCLAMATION MARK */
/* 0xA2 */	0x00A2,	/*	CENT SIGN */
/* 0xA3 */	0x00A3,	/*	POUND SIGN */
/* 0xA4 */	0x20AC,	/*	EURO SIGN */
/* 0xA5 */	0x00A5,	/*	YEN SIGN */
/* 0xA6 */	0x0160,	/*	LATIN CAPITAL LETTER S WITH CARON */
/* 0xA7 */	0x00A7,	/*	SECTION SIGN */
/* 0xA8 */	0x0161,	/*	LATIN SMALL LETTER S WITH CARON */
/* 0xA9 */	0x00A9,	/*	COPYRIGHT SIGN */
/* 0xAA */	0x00AA,	/*	FEMININE ORDINAL INDICATOR */
/* 0xAB */	0x00AB,	/*	LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
/* 0xAC */	0x00AC,	/*	NOT SIGN */
/* 0xAD */	0x00AD,	/*	SOFT HYPHEN */
/* 0xAE */	0x00AE,	/*	REGISTERED SIGN */
/* 0xAF */	0x00AF,	/*	MACRON */
/* 0xB0 */	0x00B0,	/*	DEGREE SIGN */
/* 0xB1 */	0x00B1,	/*	PLUS-MINUS SIGN */
/* 0xB2 */	0x00B2,	/*	SUPERSCRIPT TWO */
/* 0xB3 */	0x00B3,	/*	SUPERSCRIPT THREE */
/* 0xB4 */	0x017D,	/*	LATIN CAPITAL LETTER Z WITH CARON */
/* 0xB5 */	0x00B5,	/*	MICRO SIGN */
/* 0xB6 */	0x00B6,	/*	PILCROW SIGN */
/* 0xB7 */	0x00B7,	/*	MIDDLE DOT */
/* 0xB8 */	0x017E,	/*	LATIN SMALL LETTER Z WITH CARON */
/* 0xB9 */	0x00B9,	/*	SUPERSCRIPT ONE */
/* 0xBA */	0x00BA,	/*	MASCULINE ORDINAL INDICATOR */
/* 0xBB */	0x00BB,	/*	RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
/* 0xBC */	0x0152,	/*	LATIN CAPITAL LIGATURE OE */
/* 0xBD */	0x0153,	/*	LATIN SMALL LIGATURE OE */
/* 0xBE */	0x0178,	/*	LATIN CAPITAL LETTER Y WITH DIAERESIS */
/* 0xBF */	0x00BF,	/*	INVERTED QUESTION MARK */
/* 0xC0 .. 0xFF are mapped one-to-one */
};

int
bfc_iso8859_15_to_unicode(char c)
{
	unsigned char idx = (unsigned char) c;

	if ((idx >= 0xA0) && (idx <= 0xBF)) {
		return (iso8859_15_0xA0_0xBF_unicodes[idx & 0x1F]);
	} else {
		return ((int) idx);
	}
}

char
bfc_unicode_to_iso8859_15(int c)
{
	if ((c & ~0x7F) == 0) {
		return ((char) c);
	} else if ((c >= 0xA0) && (c <= 0xBF)) {
		unsigned char idx = (unsigned char) c & 0x1F;
		if (c == (int) iso8859_15_0xA0_0xBF_unicodes[idx]) {
			return (0xA0 + idx);
		}
		for (idx=0; idx < 32; idx++) {
			if (c == (int) iso8859_15_0xA0_0xBF_unicodes[idx]) {
				return (0xA0 + idx);
			}
		}
	} else if ((c & ~0xFF) == 0) {
		return ((char) c);
	}
	return ('?');
}

