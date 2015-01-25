
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

char	bfc_unicode_to_cp1250(int c);
int	bfc_cp1250_to_unicode(char c);

struct char_traits_class {
	BFC_CLASSHDR(const struct char_traits_class *, void *, const void *)
	BFC_CHAR_TRAITS_METHODS(char, int)
};

const struct char_traits_class bfc_cp1250_traits_class = {
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
	.to_char= bfc_unicode_to_cp1250,
	.to_int	= bfc_cp1250_to_unicode,
	.eq_int = bfc_char_eq_int,
};

unsigned short cp1250_0x80_0xFF_unicodes[128] = {
// http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1250.TXT
//
//    Name:     cp1250 to Unicode table
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
//        Column #1 is the cp1250 code (in hex)
//        Column #2 is the Unicode (in hex as 0xXXXX)
//        Column #3 is the Unicode name (follows a comment sign, '#')
//
//    The entries are in cp1250 order
//
/* 0x00 .. 0x7F are mapped one-to-one */
/* 0x80 */	0x20AC,	/* EURO SIGN */
/* 0x81 */	0x0081,	/* UNDEFINED */
/* 0x82 */	0x201A,	/* SINGLE LOW-9 QUOTATION MARK */
/* 0x83 */	0x0083,	/* UNDEFINED */
/* 0x84 */	0x201E,	/* DOUBLE LOW-9 QUOTATION MARK */
/* 0x85 */	0x2026,	/* HORIZONTAL ELLIPSIS */
/* 0x86 */	0x2020,	/* DAGGER */
/* 0x87 */	0x2021,	/* DOUBLE DAGGER */
/* 0x88 */	0x0088,	/* UNDEFINED */
/* 0x89 */	0x2030,	/* PER MILLE SIGN */
/* 0x8A */	0x0160,	/* LATIN CAPITAL LETTER S WITH CARON */
/* 0x8B */	0x2039,	/* SINGLE LEFT-POINTING ANGLE QUOTATION MARK */
/* 0x8C */	0x015A,	/* LATIN CAPITAL LETTER S WITH ACUTE */
/* 0x8D */	0x0164,	/* LATIN CAPITAL LETTER T WITH CARON */
/* 0x8E */	0x017D,	/* LATIN CAPITAL LETTER Z WITH CARON */
/* 0x8F */	0x0179,	/* LATIN CAPITAL LETTER Z WITH ACUTE */
/* 0x90 */	0x0090,	/* UNDEFINED */
/* 0x91 */	0x2018,	/* LEFT SINGLE QUOTATION MARK */
/* 0x92 */	0x2019,	/* RIGHT SINGLE QUOTATION MARK */
/* 0x93 */	0x201C,	/* LEFT DOUBLE QUOTATION MARK */
/* 0x94 */	0x201D,	/* RIGHT DOUBLE QUOTATION MARK */
/* 0x95 */	0x2022,	/* BULLET */
/* 0x96 */	0x2013,	/* EN DASH */
/* 0x97 */	0x2014,	/* EM DASH */
/* 0x98 */	0x0098,	/* UNDEFINED */
/* 0x99 */	0x2122,	/* TRADE MARK SIGN */
/* 0x9A */	0x0161,	/* LATIN SMALL LETTER S WITH CARON */
/* 0x9B */	0x203A,	/* SINGLE RIGHT-POINTING ANGLE QUOTATION MARK */
/* 0x9C */	0x015B,	/* LATIN SMALL LETTER S WITH ACUTE */
/* 0x9D */	0x0165,	/* LATIN SMALL LETTER T WITH CARON */
/* 0x9E */	0x017E,	/* LATIN SMALL LETTER Z WITH CARON */
/* 0x9F */	0x017A,	/* LATIN SMALL LETTER Z WITH ACUTE */
/* 0xA0 */	0x00A0,	/* NO-BREAK SPACE */
/* 0xA1 */	0x02C7,	/* CARON */
/* 0xA2 */	0x02D8,	/* BREVE */
/* 0xA3 */	0x0141,	/* LATIN CAPITAL LETTER L WITH STROKE */
/* 0xA4 */	0x00A4,	/* CURRENCY SIGN */
/* 0xA5 */	0x0104,	/* LATIN CAPITAL LETTER A WITH OGONEK */
/* 0xA6 */	0x00A6,	/* BROKEN BAR */
/* 0xA7 */	0x00A7,	/* SECTION SIGN */
/* 0xA8 */	0x00A8,	/* DIAERESIS */
/* 0xA9 */	0x00A9,	/* COPYRIGHT SIGN */
/* 0xAA */	0x015E,	/* LATIN CAPITAL LETTER S WITH CEDILLA */
/* 0xAB */	0x00AB,	/* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
/* 0xAC */	0x00AC,	/* NOT SIGN */
/* 0xAD */	0x00AD,	/* SOFT HYPHEN */
/* 0xAE */	0x00AE,	/* REGISTERED SIGN */
/* 0xAF */	0x017B,	/* LATIN CAPITAL LETTER Z WITH DOT ABOVE */
/* 0xB0 */	0x00B0,	/* DEGREE SIGN */
/* 0xB1 */	0x00B1,	/* PLUS-MINUS SIGN */
/* 0xB2 */	0x02DB,	/* OGONEK */
/* 0xB3 */	0x0142,	/* LATIN SMALL LETTER L WITH STROKE */
/* 0xB4 */	0x00B4,	/* ACUTE ACCENT */
/* 0xB5 */	0x00B5,	/* MICRO SIGN */
/* 0xB6 */	0x00B6,	/* PILCROW SIGN */
/* 0xB7 */	0x00B7,	/* MIDDLE DOT */
/* 0xB8 */	0x00B8,	/* CEDILLA */
/* 0xB9 */	0x0105,	/* LATIN SMALL LETTER A WITH OGONEK */
/* 0xBA */	0x015F,	/* LATIN SMALL LETTER S WITH CEDILLA */
/* 0xBB */	0x00BB,	/* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
/* 0xBC */	0x013D,	/* LATIN CAPITAL LETTER L WITH CARON */
/* 0xBD */	0x02DD,	/* DOUBLE ACUTE ACCENT */
/* 0xBE */	0x013E,	/* LATIN SMALL LETTER L WITH CARON */
/* 0xBF */	0x017C,	/* LATIN SMALL LETTER Z WITH DOT ABOVE */
/* 0xC0 */	0x0154,	/* LATIN CAPITAL LETTER R WITH ACUTE */
/* 0xC1 */	0x00C1,	/* LATIN CAPITAL LETTER A WITH ACUTE */
/* 0xC2 */	0x00C2,	/* LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
/* 0xC3 */	0x0102,	/* LATIN CAPITAL LETTER A WITH BREVE */
/* 0xC4 */	0x00C4,	/* LATIN CAPITAL LETTER A WITH DIAERESIS */
/* 0xC5 */	0x0139,	/* LATIN CAPITAL LETTER L WITH ACUTE */
/* 0xC6 */	0x0106,	/* LATIN CAPITAL LETTER C WITH ACUTE */
/* 0xC7 */	0x00C7,	/* LATIN CAPITAL LETTER C WITH CEDILLA */
/* 0xC8 */	0x010C,	/* LATIN CAPITAL LETTER C WITH CARON */
/* 0xC9 */	0x00C9,	/* LATIN CAPITAL LETTER E WITH ACUTE */
/* 0xCA */	0x0118,	/* LATIN CAPITAL LETTER E WITH OGONEK */
/* 0xCB */	0x00CB,	/* LATIN CAPITAL LETTER E WITH DIAERESIS */
/* 0xCC */	0x011A,	/* LATIN CAPITAL LETTER E WITH CARON */
/* 0xCD */	0x00CD,	/* LATIN CAPITAL LETTER I WITH ACUTE */
/* 0xCE */	0x00CE,	/* LATIN CAPITAL LETTER I WITH CIRCUMFLEX */
/* 0xCF */	0x010E,	/* LATIN CAPITAL LETTER D WITH CARON */
/* 0xD0 */	0x0110,	/* LATIN CAPITAL LETTER D WITH STROKE */
/* 0xD1 */	0x0143,	/* LATIN CAPITAL LETTER N WITH ACUTE */
/* 0xD2 */	0x0147,	/* LATIN CAPITAL LETTER N WITH CARON */
/* 0xD3 */	0x00D3,	/* LATIN CAPITAL LETTER O WITH ACUTE */
/* 0xD4 */	0x00D4,	/* LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
/* 0xD5 */	0x0150,	/* LATIN CAPITAL LETTER O WITH DOUBLE ACUTE */
/* 0xD6 */	0x00D6,	/* LATIN CAPITAL LETTER O WITH DIAERESIS */
/* 0xD7 */	0x00D7,	/* MULTIPLICATION SIGN */
/* 0xD8 */	0x0158,	/* LATIN CAPITAL LETTER R WITH CARON */
/* 0xD9 */	0x016E,	/* LATIN CAPITAL LETTER U WITH RING ABOVE */
/* 0xDA */	0x00DA,	/* LATIN CAPITAL LETTER U WITH ACUTE */
/* 0xDB */	0x0170,	/* LATIN CAPITAL LETTER U WITH DOUBLE ACUTE */
/* 0xDC */	0x00DC,	/* LATIN CAPITAL LETTER U WITH DIAERESIS */
/* 0xDD */	0x00DD,	/* LATIN CAPITAL LETTER Y WITH ACUTE */
/* 0xDE */	0x0162,	/* LATIN CAPITAL LETTER T WITH CEDILLA */
/* 0xDF */	0x00DF,	/* LATIN SMALL LETTER SHARP S */
/* 0xE0 */	0x0155,	/* LATIN SMALL LETTER R WITH ACUTE */
/* 0xE1 */	0x00E1,	/* LATIN SMALL LETTER A WITH ACUTE */
/* 0xE2 */	0x00E2,	/* LATIN SMALL LETTER A WITH CIRCUMFLEX */
/* 0xE3 */	0x0103,	/* LATIN SMALL LETTER A WITH BREVE */
/* 0xE4 */	0x00E4,	/* LATIN SMALL LETTER A WITH DIAERESIS */
/* 0xE5 */	0x013A,	/* LATIN SMALL LETTER L WITH ACUTE */
/* 0xE6 */	0x0107,	/* LATIN SMALL LETTER C WITH ACUTE */
/* 0xE7 */	0x00E7,	/* LATIN SMALL LETTER C WITH CEDILLA */
/* 0xE8 */	0x010D,	/* LATIN SMALL LETTER C WITH CARON */
/* 0xE9 */	0x00E9,	/* LATIN SMALL LETTER E WITH ACUTE */
/* 0xEA */	0x0119,	/* LATIN SMALL LETTER E WITH OGONEK */
/* 0xEB */	0x00EB,	/* LATIN SMALL LETTER E WITH DIAERESIS */
/* 0xEC */	0x011B,	/* LATIN SMALL LETTER E WITH CARON */
/* 0xED */	0x00ED,	/* LATIN SMALL LETTER I WITH ACUTE */
/* 0xEE */	0x00EE,	/* LATIN SMALL LETTER I WITH CIRCUMFLEX */
/* 0xEF */	0x010F,	/* LATIN SMALL LETTER D WITH CARON */
/* 0xF0 */	0x0111,	/* LATIN SMALL LETTER D WITH STROKE */
/* 0xF1 */	0x0144,	/* LATIN SMALL LETTER N WITH ACUTE */
/* 0xF2 */	0x0148,	/* LATIN SMALL LETTER N WITH CARON */
/* 0xF3 */	0x00F3,	/* LATIN SMALL LETTER O WITH ACUTE */
/* 0xF4 */	0x00F4,	/* LATIN SMALL LETTER O WITH CIRCUMFLEX */
/* 0xF5 */	0x0151,	/* LATIN SMALL LETTER O WITH DOUBLE ACUTE */
/* 0xF6 */	0x00F6,	/* LATIN SMALL LETTER O WITH DIAERESIS */
/* 0xF7 */	0x00F7,	/* DIVISION SIGN */
/* 0xF8 */	0x0159,	/* LATIN SMALL LETTER R WITH CARON */
/* 0xF9 */	0x016F,	/* LATIN SMALL LETTER U WITH RING ABOVE */
/* 0xFA */	0x00FA,	/* LATIN SMALL LETTER U WITH ACUTE */
/* 0xFB */	0x0171,	/* LATIN SMALL LETTER U WITH DOUBLE ACUTE */
/* 0xFC */	0x00FC,	/* LATIN SMALL LETTER U WITH DIAERESIS */
/* 0xFD */	0x00FD,	/* LATIN SMALL LETTER Y WITH ACUTE */
/* 0xFE */	0x0163,	/* LATIN SMALL LETTER T WITH CEDILLA */
/* 0xFF */	0x02D9,	/* DOT ABOVE */
};

int
bfc_cp1250_to_unicode(char c)
{
	if ((c & ~0x7F) == 0) {
		return (c);
	} else {
		return (cp1250_0x80_0xFF_unicodes[c & 0x7F]);
	}
}

char
bfc_unicode_to_cp1250(int c)
{
	unsigned char idx;

	if ((c & ~0x7F) == 0) {
		return ((char) c);
	} else {
		idx = (unsigned char) c & 0x7F;
		if (c == (int) cp1250_0x80_0xFF_unicodes[idx]) {
			return (0x80 + idx);
		}
	}
	for (idx=0; idx < 128; idx++) {
		if (c == (int) cp1250_0x80_0xFF_unicodes[idx]) {
			return (0x80 + idx);
		}
	}
	return ('?');
}

