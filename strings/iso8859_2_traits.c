
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

char	bfc_unicode_to_iso8859_2(int c);
int	bfc_iso8859_2_to_unicode(char c);

struct char_traits_class {
	BFC_CLASSHDR(const struct char_traits_class *, void *, const void *)
	BFC_CHAR_TRAITS_METHODS(char, int)
};

struct char_traits_class bfc_iso8859_2_traits_class = {
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
	.to_char= bfc_unicode_to_iso8859_2,
	.to_int	= bfc_iso8859_2_to_unicode,
	.eq_int = bfc_char_eq_int,
};

unsigned short iso8859_2_0xA0_0xFF_unicodes[96] = {
// http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-2.TXT
//
//	Name:             ISO 8859-2:1999 to Unicode
//	Unicode version:  3.0
//	Table version:    1.0
//	Table format:     Format A
//	Date:             1999 July 27
//	Authors:          Ken Whistler <kenw@sybase.com>
//
//	Copyright (c) 1991-1999 Unicode, Inc.  All Rights reserved.
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
//       ISO/IEC 8859-2:1999 characters map into Unicode.
//
//	Format:  Three tab-separated columns
//		 Column #1 is the ISO/IEC 8859-2 code (in hex as 0xXX)
//		 Column #2 is the Unicode (in hex as 0xXXXX)
//		 Column #3 the Unicode name (follows a comment sign, '#')
//
//	The entries are in ISO/IEC 8859-2 order.
//
/* 0x00 .. 0x9F are mapped one-to-one */
/* 0xA0 */	0x00A0,	/*	NO-BREAK SPACE */
/* 0xA1 */	0x0104,	/*	LATIN CAPITAL LETTER A WITH OGONEK */
/* 0xA2 */	0x02D8,	/*	BREVE */
/* 0xA3 */	0x0141,	/*	LATIN CAPITAL LETTER L WITH STROKE */
/* 0xA4 */	0x00A4,	/*	CURRENCY SIGN */
/* 0xA5 */	0x013D,	/*	LATIN CAPITAL LETTER L WITH CARON */
/* 0xA6 */	0x015A,	/*	LATIN CAPITAL LETTER S WITH ACUTE */
/* 0xA7 */	0x00A7,	/*	SECTION SIGN */
/* 0xA8 */	0x00A8,	/*	DIAERESIS */
/* 0xA9 */	0x0160,	/*	LATIN CAPITAL LETTER S WITH CARON */
/* 0xAA */	0x015E,	/*	LATIN CAPITAL LETTER S WITH CEDILLA */
/* 0xAB */	0x0164,	/*	LATIN CAPITAL LETTER T WITH CARON */
/* 0xAC */	0x0179,	/*	LATIN CAPITAL LETTER Z WITH ACUTE */
/* 0xAD */	0x00AD,	/*	SOFT HYPHEN */
/* 0xAE */	0x017D,	/*	LATIN CAPITAL LETTER Z WITH CARON */
/* 0xAF */	0x017B,	/*	LATIN CAPITAL LETTER Z WITH DOT ABOVE */
/* 0xB0 */	0x00B0,	/*	DEGREE SIGN */
/* 0xB1 */	0x0105,	/*	LATIN SMALL LETTER A WITH OGONEK */
/* 0xB2 */	0x02DB,	/*	OGONEK */
/* 0xB3 */	0x0142,	/*	LATIN SMALL LETTER L WITH STROKE */
/* 0xB4 */	0x00B4,	/*	ACUTE ACCENT */
/* 0xB5 */	0x013E,	/*	LATIN SMALL LETTER L WITH CARON */
/* 0xB6 */	0x015B,	/*	LATIN SMALL LETTER S WITH ACUTE */
/* 0xB7 */	0x02C7,	/*	CARON */
/* 0xB8 */	0x00B8,	/*	CEDILLA */
/* 0xB9 */	0x0161,	/*	LATIN SMALL LETTER S WITH CARON */
/* 0xBA */	0x015F,	/*	LATIN SMALL LETTER S WITH CEDILLA */
/* 0xBB */	0x0165,	/*	LATIN SMALL LETTER T WITH CARON */
/* 0xBC */	0x017A,	/*	LATIN SMALL LETTER Z WITH ACUTE */
/* 0xBD */	0x02DD,	/*	DOUBLE ACUTE ACCENT */
/* 0xBE */	0x017E,	/*	LATIN SMALL LETTER Z WITH CARON */
/* 0xBF */	0x017C,	/*	LATIN SMALL LETTER Z WITH DOT ABOVE */
/* 0xC0 */	0x0154,	/*	LATIN CAPITAL LETTER R WITH ACUTE */
/* 0xC1 */	0x00C1,	/*	LATIN CAPITAL LETTER A WITH ACUTE */
/* 0xC2 */	0x00C2,	/*	LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
/* 0xC3 */	0x0102,	/*	LATIN CAPITAL LETTER A WITH BREVE */
/* 0xC4 */	0x00C4,	/*	LATIN CAPITAL LETTER A WITH DIAERESIS */
/* 0xC5 */	0x0139,	/*	LATIN CAPITAL LETTER L WITH ACUTE */
/* 0xC6 */	0x0106,	/*	LATIN CAPITAL LETTER C WITH ACUTE */
/* 0xC7 */	0x00C7,	/*	LATIN CAPITAL LETTER C WITH CEDILLA */
/* 0xC8 */	0x010C,	/*	LATIN CAPITAL LETTER C WITH CARON */
/* 0xC9 */	0x00C9,	/*	LATIN CAPITAL LETTER E WITH ACUTE */
/* 0xCA */	0x0118,	/*	LATIN CAPITAL LETTER E WITH OGONEK */
/* 0xCB */	0x00CB,	/*	LATIN CAPITAL LETTER E WITH DIAERESIS */
/* 0xCC */	0x011A,	/*	LATIN CAPITAL LETTER E WITH CARON */
/* 0xCD */	0x00CD,	/*	LATIN CAPITAL LETTER I WITH ACUTE */
/* 0xCE */	0x00CE,	/*	LATIN CAPITAL LETTER I WITH CIRCUMFLEX */
/* 0xCF */	0x010E,	/*	LATIN CAPITAL LETTER D WITH CARON */
/* 0xD0 */	0x0110,	/*	LATIN CAPITAL LETTER D WITH STROKE */
/* 0xD1 */	0x0143,	/*	LATIN CAPITAL LETTER N WITH ACUTE */
/* 0xD2 */	0x0147,	/*	LATIN CAPITAL LETTER N WITH CARON */
/* 0xD3 */	0x00D3,	/*	LATIN CAPITAL LETTER O WITH ACUTE */
/* 0xD4 */	0x00D4,	/*	LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
/* 0xD5 */	0x0150,	/*	LATIN CAPITAL LETTER O WITH DOUBLE ACUTE */
/* 0xD6 */	0x00D6,	/*	LATIN CAPITAL LETTER O WITH DIAERESIS */
/* 0xD7 */	0x00D7,	/*	MULTIPLICATION SIGN */
/* 0xD8 */	0x0158,	/*	LATIN CAPITAL LETTER R WITH CARON */
/* 0xD9 */	0x016E,	/*	LATIN CAPITAL LETTER U WITH RING ABOVE */
/* 0xDA */	0x00DA,	/*	LATIN CAPITAL LETTER U WITH ACUTE */
/* 0xDB */	0x0170,	/*	LATIN CAPITAL LETTER U WITH DOUBLE ACUTE */
/* 0xDC */	0x00DC,	/*	LATIN CAPITAL LETTER U WITH DIAERESIS */
/* 0xDD */	0x00DD,	/*	LATIN CAPITAL LETTER Y WITH ACUTE */
/* 0xDE */	0x0162,	/*	LATIN CAPITAL LETTER T WITH CEDILLA */
/* 0xDF */	0x00DF,	/*	LATIN SMALL LETTER SHARP S */
/* 0xE0 */	0x0155,	/*	LATIN SMALL LETTER R WITH ACUTE */
/* 0xE1 */	0x00E1,	/*	LATIN SMALL LETTER A WITH ACUTE */
/* 0xE2 */	0x00E2,	/*	LATIN SMALL LETTER A WITH CIRCUMFLEX */
/* 0xE3 */	0x0103,	/*	LATIN SMALL LETTER A WITH BREVE */
/* 0xE4 */	0x00E4,	/*	LATIN SMALL LETTER A WITH DIAERESIS */
/* 0xE5 */	0x013A,	/*	LATIN SMALL LETTER L WITH ACUTE */
/* 0xE6 */	0x0107,	/*	LATIN SMALL LETTER C WITH ACUTE */
/* 0xE7 */	0x00E7,	/*	LATIN SMALL LETTER C WITH CEDILLA */
/* 0xE8 */	0x010D,	/*	LATIN SMALL LETTER C WITH CARON */
/* 0xE9 */	0x00E9,	/*	LATIN SMALL LETTER E WITH ACUTE */
/* 0xEA */	0x0119,	/*	LATIN SMALL LETTER E WITH OGONEK */
/* 0xEB */	0x00EB,	/*	LATIN SMALL LETTER E WITH DIAERESIS */
/* 0xEC */	0x011B,	/*	LATIN SMALL LETTER E WITH CARON */
/* 0xED */	0x00ED,	/*	LATIN SMALL LETTER I WITH ACUTE */
/* 0xEE */	0x00EE,	/*	LATIN SMALL LETTER I WITH CIRCUMFLEX */
/* 0xEF */	0x010F,	/*	LATIN SMALL LETTER D WITH CARON */
/* 0xF0 */	0x0111,	/*	LATIN SMALL LETTER D WITH STROKE */
/* 0xF1 */	0x0144,	/*	LATIN SMALL LETTER N WITH ACUTE */
/* 0xF2 */	0x0148,	/*	LATIN SMALL LETTER N WITH CARON */
/* 0xF3 */	0x00F3,	/*	LATIN SMALL LETTER O WITH ACUTE */
/* 0xF4 */	0x00F4,	/*	LATIN SMALL LETTER O WITH CIRCUMFLEX */
/* 0xF5 */	0x0151,	/*	LATIN SMALL LETTER O WITH DOUBLE ACUTE */
/* 0xF6 */	0x00F6,	/*	LATIN SMALL LETTER O WITH DIAERESIS */
/* 0xF7 */	0x00F7,	/*	DIVISION SIGN */
/* 0xF8 */	0x0159,	/*	LATIN SMALL LETTER R WITH CARON */
/* 0xF9 */	0x016F,	/*	LATIN SMALL LETTER U WITH RING ABOVE */
/* 0xFA */	0x00FA,	/*	LATIN SMALL LETTER U WITH ACUTE */
/* 0xFB */	0x0171,	/*	LATIN SMALL LETTER U WITH DOUBLE ACUTE */
/* 0xFC */	0x00FC,	/*	LATIN SMALL LETTER U WITH DIAERESIS */
/* 0xFD */	0x00FD,	/*	LATIN SMALL LETTER Y WITH ACUTE */
/* 0xFE */	0x0163,	/*	LATIN SMALL LETTER T WITH CEDILLA */
/* 0xFF */	0x02D9,	/*	DOT ABOVE */
};

int
bfc_iso8859_2_to_unicode(char c)
{
	unsigned char idx = (unsigned char) c;

	if ((idx >= 0xA0) && (idx <= 0xFF)) {
		return (iso8859_2_0xA0_0xFF_unicodes[idx - 0xA0]);
	} else {
		return ((int) idx);
	}
}

char
bfc_unicode_to_iso8859_2(int c)
{
	unsigned char idx;

	if ((c & ~0x7F) == 0) {
		return ((char) c);
	} else if ((c >= 0xA0) && (c <= 0xFF)) {
		idx = (unsigned char) c - 0xA0;
		if (c == (int) iso8859_2_0xA0_0xFF_unicodes[idx]) {
			return (0xA0 + idx);
		}
	} else if ((c & ~0xFF) == 0) {
		return ((char) c);
	}
	for (idx=0; idx < 96; idx++) {
		if (c == (int) iso8859_2_0xA0_0xFF_unicodes[idx]) {
			return (0xA0 + idx);
		}
	}
	return ('?');
}

