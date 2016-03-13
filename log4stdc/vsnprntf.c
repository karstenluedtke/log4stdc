
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <windows.h>
#define signed_long_long	INT64
#define unsigned_long_long	UINT64
#elif defined(__STDC__)
#define signed_long_long	long long
#define unsigned_long_long	unsigned long long
#endif

int l4sc_vsnprintf(char *buf, size_t bufsize, const char *fmt, va_list ap);

#define CONV_FLAG_LEFT_ALIGN		0x0001	/* - */
#define CONV_FLAG_INCLUDE_SIGN		0x0010	/* + */
#define CONV_FLAG_SPACE_SIGN		0x0020	/*   */
#define CONV_FLAG_LEADING_ZEROES	0x0040	/* 0 */
#define CONV_FLAG_ALT_FORMAT		0x0080	/* # */
#define CONV_FLAG_SPECIFIERS		"-+ 0#"

#define CONV_FLAG_CAPITALS		0x0100
#define CONV_PRECISION_AS_INT_ARG	0x1000
#define CONV_WIDTH_AS_INT_ARG		0x2000
#define CONV_DEFAULT_WIDTH		-1

#define CONV_ARG_MODIFIER_CHAR		'c'	/* hh */
#define CONV_ARG_MODIFIER_SHORT		'h'
#define CONV_ARG_MODIFIER_LONG		'l'
#define CONV_ARG_MODIFIER_LLONG		'L'	/* ll */
#define CONV_ARG_MODIFIER_INTMAX	'j'
#define CONV_ARG_MODIFIER_SIZE_T	'z'
#define CONV_ARG_MODIFIER_PTRDIFF_T	't'
#define CONV_ARG_MODIFIER_LDOUBLE	'L'	/* L  */
#define CONV_ARG_MODIFIERS		"hljztL"

#define CONV_SPEC_INTEGER		'd'	/* d, i */
#define CONV_SPEC_UNSIGNED		'u'
#define CONV_SPEC_OCTAL			'o'
#define CONV_SPEC_HEX			'x'
#define CONV_SPEC_HEX_CAPITALS		'X'
#define CONV_SPEC_DECIMAL		'f'
#define CONV_SPEC_DECIMAL_CAPITALS	'F'
#define CONV_SPEC_SCIENTIFIC		'e'
#define CONV_SPEC_SCIENTIFIC_CAPITALS	'E'
#define CONV_SPEC_ADAPTIVE		'g'
#define CONV_SPEC_ADAPTIVE_CAPITALS	'G'
#define CONV_SPEC_HEXFLOAT		'a'
#define CONV_SPEC_HEXFLOAT_CAPITALS	'A'
#define CONV_SPEC_CHARACTER		'c'
#define CONV_SPEC_STRING		's'
#define CONV_SPEC_POINTER		'p'
#define CONV_SPEC_STORE_BYTES_WRITTEN	'n'
#define CONV_SPEC_LITERAL_PERCENT	'%'
#define CONV_SPEC_SPECIFIERS		"duoxXcspfFeEgGaAn%"
#define CONV_SPEC_DOUBLE_SPECIFIERS	"fFeEgGaA"

#define PUTNEXTCHAR(ptr,c,limit)	\
	if ((ptr) < (limit)) {		\
		*((ptr)++) = (c);	\
	} else {			\
		(ptr)++;		\
	}

static int
put_string(char *buf, int width, int precision, int flags, const char *limit,
           const char *s)
{
	char *dp = buf;

#define PUT_LEFT_FILL(ptr,width,need,flags,limit,c)			\
	if ((width > need) && !((flags) & CONV_FLAG_LEFT_ALIGN)) {	\
		do {							\
			PUTNEXTCHAR(ptr, c, limit);			\
			width--;					\
		} while (width > need);					\
	}

#define PUT_RIGHT_FILL(ptr,width,need,flags,limit,c)			\
	while (width > 0) {						\
		PUTNEXTCHAR(ptr, c, limit);				\
		width--;						\
	}

#define PUT_STRING(ptr,width,precision,flags,limit,s)			\
	do {								\
		int _w = width;						\
		int _n = ((precision) >= 0)? (precision): strlen(s);	\
		PUT_LEFT_FILL(ptr, _w, _n, flags, limit, ' ');		\
		if (_n > 0) {						\
			if (ptr + _n < limit) {				\
				memcpy(ptr, s, _n);			\
			} else if (ptr < limit) {			\
				memcpy(ptr, s, limit - ptr);		\
			}						\
			ptr += _n;					\
			_w -= _n;					\
		}							\
		PUT_RIGHT_FILL(ptr, _w, _n, flags, limit, ' ');		\
	} while (0 /*just once */)

	PUT_STRING(dp,width,precision,flags,limit,s);
	return (dp - buf);
}

static int
put_unsigned(char *buf, int width, int precision, int flags, const char *limit,
             const char *prefix, int pfxlen, unsigned v)
{
	char *dp = buf;

#define POSITIVE_SIGN_BYTES(flags)					\
	(((flags) & (CONV_FLAG_INCLUDE_SIGN | CONV_FLAG_SPACE_SIGN))? 1: 0)

#define UNSIGNED_MUL(T,result,overflow,factor)				\
	do {								\
		const T _boundary = ((T)-1) / factor;			\
		if (result <= _boundary) {				\
			result *= factor;				\
		} else {						\
			overflow = 1;					\
		}							\
	} while (0 /*just once */)

#define UNSIGNED_EXP(T,result,overflow,base,e)				\
	do {								\
		int _pending = e;					\
		const T _boundary = ((T)-1) / base;			\
		for (result=1,overflow=0; _pending > 0; _pending--) {	\
			if (result <= _boundary) {			\
				result *= base;				\
			} else {					\
				overflow = 1;				\
				break;					\
			}						\
		}							\
	} while (0 /*just once */)

#define PUT_PREFIX_AND_SIGN(ptr,width,need,flags,limit,prefix,pfxlen)	\
	do {								\
		int _k;							\
		for (_k = 0; _k < pfxlen; _k++) {			\
			PUTNEXTCHAR(ptr, prefix[_k], limit);		\
			width--;					\
		}							\
		if ((flags) & CONV_FLAG_INCLUDE_SIGN) {			\
			PUTNEXTCHAR(ptr, '+', limit);			\
			width--;					\
		} else if ((flags) & CONV_FLAG_SPACE_SIGN) {		\
			PUTNEXTCHAR(ptr, ' ', limit);			\
			width--;					\
		}							\
	} while (0 /*just once */)

#define PUT_UNSIGNED(ptr,width,prec,flags,limit,prefix,pfxlen,T,base,val)\
	do {								\
		T _v = val;						\
		T _divisor = base;					\
		int _digits = 1;					\
		int _need, _w = width, _ovfl = 0;			\
		char _c = ' ';						\
		if ((_v == 0) && (prec == 0)) {				\
			_digits = 0;					\
		} else {						\
			while (!_ovfl && (_v >= _divisor)) {		\
				_digits++;				\
				UNSIGNED_MUL(T,_divisor,_ovfl,base);	\
			}						\
			if (_digits < prec) {				\
				_digits = prec;				\
			}						\
		}							\
		_need = _digits;					\
		if ((flags) & CONV_FLAG_LEADING_ZEROES) {		\
			PUT_PREFIX_AND_SIGN(ptr, _w, _need, flags,	\
						limit, prefix, pfxlen);	\
			PUT_LEFT_FILL(ptr,_w,_need,flags,limit, '0');	\
		} else {						\
			_need += pfxlen + POSITIVE_SIGN_BYTES(flags);	\
			PUT_LEFT_FILL(ptr, _w, _need,flags,limit, ' ');	\
			PUT_PREFIX_AND_SIGN(ptr, _w, _need, flags,	\
						limit, prefix, pfxlen);	\
		}							\
		while (_digits > 0) {					\
			UNSIGNED_EXP(T,_divisor,_ovfl,base,_digits-1);	\
			_c = (char)(_ovfl? 0:				\
				    (_divisor > 1)? _v/_divisor: _v);	\
			_v -= _c * _divisor;				\
			_c += '0';					\
			PUTNEXTCHAR(ptr, _c, limit);			\
			_digits--;					\
			_w--;						\
		}							\
		PUT_RIGHT_FILL(ptr, _w, _need, flags, limit, ' ');	\
	} while (0 /*just once */)

	PUT_UNSIGNED(dp,width,precision,flags,limit,
			prefix, pfxlen, unsigned, 10, v);
	return (dp - buf);
}

static int
put_ulong(char *buf, int width, int precision, int flags, const char *limit,
             const char *prefix, int pfxlen, unsigned long v)
{
	char *dp = buf;

	PUT_UNSIGNED(dp,width,precision,flags,limit,
			prefix, pfxlen, unsigned long, 10, v);
	return (dp - buf);
}

#ifdef unsigned_long_long
static int
put_ullong(char *buf, int width, int precision, int flags, const char *limit,
             const char *prefix, int pfxlen, unsigned_long_long v)
{
	char *dp = buf;

	PUT_UNSIGNED(dp,width,precision,flags,limit,
			prefix, pfxlen, unsigned_long_long, 10, v);
	return (dp - buf);
}
#endif

static int
put_hex(char *buf, int width, int precision, int flags, const char *limit,
        const char *prefix, int pfxlen, unsigned v)
{
	char *dp = buf;

#define PUT_SIGN_AND_PREFIX(ptr,width,need,flags,limit,prefix,pfxlen)	\
	do {								\
		int _k;							\
		if ((flags) & CONV_FLAG_INCLUDE_SIGN) {			\
			PUTNEXTCHAR(ptr, '+', limit);			\
			width--;					\
		} else if ((flags) & CONV_FLAG_SPACE_SIGN) {		\
			PUTNEXTCHAR(ptr, ' ', limit);			\
			width--;					\
		}							\
		for (_k = 0; _k < pfxlen; _k++) {			\
			PUTNEXTCHAR(ptr, prefix[_k], limit);		\
			width--;					\
		}							\
	} while (0 /*just once */)

#define PUT_HEX(ptr,width,prec,flags,limit,prefix,pfxlen,T,digbits,val)	\
	do {								\
		T _v = val;						\
		int _digits = 1;					\
		int _digshift = digbits;				\
		int _need, _w = width;					\
		char _c = ' ';						\
		if ((_v == 0) && (prec == 0)) {				\
			_digits = 0;					\
		} else {						\
			while ((_v >> _digshift) >= (T) 1) {		\
				_digits++;				\
				_digshift += digbits;			\
				if (_digshift >= 8*sizeof(T)) {		\
					break;				\
				}					\
			}						\
			while (_digits < prec) {			\
				_digits++;				\
				_digshift += digbits;			\
			}						\
		}							\
		_need = _digits;					\
		if ((flags) & CONV_FLAG_LEADING_ZEROES) {		\
			PUT_SIGN_AND_PREFIX(ptr, _w, _need, flags,	\
						limit, prefix, pfxlen);	\
			PUT_LEFT_FILL(ptr,_w,_need,flags,limit, '0');	\
		} else {						\
			_need += pfxlen + POSITIVE_SIGN_BYTES(flags);	\
			PUT_LEFT_FILL(ptr, _w, _need,flags,limit, ' ');	\
			PUT_SIGN_AND_PREFIX(ptr, _w, _need, flags,	\
						limit, prefix, pfxlen);	\
		}							\
		while (_digits > 0) {					\
			_digshift -= digbits;				\
			_c = (_digshift >= 8*sizeof(T))? (char) 0:	\
					  (char) (_v >> _digshift);	\
			_v -= (((T) _c) << _digshift);			\
			if (_c < 10) {					\
				_c += '0';				\
			} else if ((flags) & CONV_FLAG_CAPITALS) {	\
				_c += ('A' - 10);			\
			} else {					\
				_c += ('a' - 10);			\
			}						\
			PUTNEXTCHAR(ptr, _c, limit);			\
			_digits--;					\
			_w--;						\
		}							\
		PUT_RIGHT_FILL(ptr, _w, _need, flags, limit, ' ');	\
	} while (0 /*just once */)

	PUT_HEX(dp,width,precision,flags,limit,
		prefix, pfxlen, unsigned, 4, v);
	return (dp - buf);
}

static int
put_lhex(char *buf, int width, int precision, int flags, const char *limit,
         const char *prefix, int pfxlen, unsigned long v)
{
	char *dp = buf;

	PUT_HEX(dp,width,precision,flags,limit,
		prefix, pfxlen, unsigned long, 4, v);
	return (dp - buf);
}

#ifdef unsigned_long_long
static int
put_llhex(char *buf, int width, int precision, int flags, const char *limit,
          const char *prefix, int pfxlen, unsigned_long_long v)
{
	char *dp = buf;

	PUT_HEX(dp,width,precision,flags,limit,
		prefix, pfxlen, unsigned_long_long, 4, v);
	return (dp - buf);
}
#endif

static int
put_octal(char *buf, int width, int precision, int flags, const char *limit,
          const char *prefix, int pfxlen, unsigned v)
{
	char *dp = buf;

	PUT_HEX(dp,width,precision,flags,limit,
		prefix, pfxlen, unsigned, 3, v);
	return (dp - buf);
}

static int
put_loctal(char *buf, int width, int precision, int flags, const char *limit,
           const char *prefix, int pfxlen, unsigned long v)
{
	char *dp = buf;

	PUT_HEX(dp,width,precision,flags,limit,
		prefix, pfxlen, unsigned long, 3, v);
	return (dp - buf);
}

#ifdef unsigned_long_long
static int
put_lloctal(char *buf, int width, int precision, int flags, const char *limit,
           const char *prefix, int pfxlen, unsigned_long_long v)
{
	char *dp = buf;

	PUT_HEX(dp,width,precision,flags,limit,
		prefix, pfxlen, unsigned_long_long, 3, v);
	return (dp - buf);
}
#endif

int
l4sc_vsnprintf(char *buf, size_t bufsize, const char *fmt, va_list ap)
{
	char *dp = buf;
	const char *limit = (buf == NULL)? NULL: buf + bufsize;
	const char *cp = fmt;
	const char *s, *spec;
	int n, width, precision, flags;
	char c, conversion, modifier;
	double f;

	while ((spec = strchr(cp, '%')) != NULL) {
		if ((n = spec - cp) > 0) {
			if (dp + n < limit) {
				memcpy(dp, cp, n);
				dp += n;
				cp = spec;
			} else {
				do {
					PUTNEXTCHAR(dp, *cp, limit);
				} while (++cp < spec);
			}
		}
		/* cp now points to a '%' */
		conversion = modifier = 0;
		width = precision = CONV_DEFAULT_WIDTH;
		flags = 0;
		c = *(++cp);
		while (strchr(CONV_FLAG_SPECIFIERS, c)) {
			switch (c) {
			case '-': flags |= CONV_FLAG_LEFT_ALIGN; break;
			case '+': flags |= CONV_FLAG_INCLUDE_SIGN; break;
			case ' ': flags |= CONV_FLAG_SPACE_SIGN; break;
			case '0': flags |= CONV_FLAG_LEADING_ZEROES; break;
			case '#': flags |= CONV_FLAG_ALT_FORMAT; break;
			default: ;
			}
			c = *(++cp);
		}
		if (c == '*') {
			flags |= CONV_WIDTH_AS_INT_ARG;
			c = *(++cp);
		} else if (strchr("0123456789", c)) {
			width = c - '0';
			c = *(++cp);
			while (strchr("0123456789", c)) {
				width = (10 * width) + (c - '0');
				c = *(++cp);
			}
		}
		if (c == '.') {
			precision = 0;
			c = *(++cp);
			if (c == '*') {
				flags |= CONV_PRECISION_AS_INT_ARG;
				c = *(++cp);
			} else if (strchr("0123456789", c)) {
				precision = c - '0';
				c = *(++cp);
				while (strchr("0123456789", c)) {
					precision = (10 * precision) + (c-'0');
					c = *(++cp);
				}
			}
		}
		while (strchr(CONV_ARG_MODIFIERS, c)) {
			switch (c) {
			case 'h':
				if (cp[1] == 'h') {
					cp++;
					modifier = CONV_ARG_MODIFIER_CHAR;
				} else {
					modifier = CONV_ARG_MODIFIER_SHORT;
				}
				break;
			case 'l':
				if (cp[1] == 'l') {
					cp++;
					modifier = CONV_ARG_MODIFIER_LLONG;
				} else {
					modifier = CONV_ARG_MODIFIER_LONG;
				}
				break;
			default:
				modifier = c;
			}
			c = *(++cp);
		}
		conversion = c;
		c = *(++cp);
		/* cp now points behind the spec */
		if (flags & CONV_WIDTH_AS_INT_ARG) {
			width = va_arg(ap, int);
		}
		if (flags & CONV_PRECISION_AS_INT_ARG) {
			precision = va_arg(ap, int);
		}
		switch (conversion) {
		case CONV_SPEC_LITERAL_PERCENT:
			PUTNEXTCHAR(dp, '%', limit);
			break;
		case CONV_SPEC_CHARACTER:
			c = (char) va_arg(ap, int);
			PUTNEXTCHAR(dp, c, limit);
			break;
		case CONV_SPEC_STRING:
			s = va_arg(ap, char *);
			n = put_string(dp, width, precision, flags, limit, s);
			dp += (n > 0)? n: 0;
			break;
		case CONV_SPEC_INTEGER:
			n = 0;
			if (modifier == CONV_ARG_MODIFIER_LONG) {
				long v = va_arg(ap, long);
				if (v < 0) {
					n = put_ulong(dp, width, precision,
					      flags & ~CONV_FLAG_INCLUDE_SIGN,
					      limit, "-", 1, -v);
				} else {
					n = put_ulong(dp, width, precision,
                                              flags, limit, "", 0, v);
				}
#ifdef signed_long_long
			} else if (modifier == CONV_ARG_MODIFIER_LLONG) {
				signed_long_long v=va_arg(ap,signed_long_long);
				if (v < 0) {
					n = put_ullong(dp, width, precision,
					      flags & ~CONV_FLAG_INCLUDE_SIGN,
					      limit, "-", 1, -v);
				} else {
					n = put_ullong(dp, width, precision,
                                              flags, limit, "", 0, v);
				}
#endif
			} else {
				int v = va_arg(ap, int);
				if (v < 0) {
					n = put_unsigned(dp, width, precision,
					      flags & ~CONV_FLAG_INCLUDE_SIGN,
					      limit, "-", 1, -v);
				} else {
					n = put_unsigned(dp, width, precision,
                                              flags, limit, "", 0, v);
				}
			}
			dp += (n > 0)? n: 0;
			break;
		case CONV_SPEC_UNSIGNED:
			n = 0;
			if (modifier == CONV_ARG_MODIFIER_LONG) {
				unsigned long v = va_arg(ap, unsigned long);
				n = put_ulong(dp, width, precision,
                                              flags, limit, "", 0, v);
#ifdef unsigned_long_long
			} else if (modifier == CONV_ARG_MODIFIER_LLONG) {
				unsigned_long_long v =
					va_arg(ap, unsigned_long_long);
				n = put_ullong(dp, width, precision,
                                              flags, limit, "", 0, v);
#endif
			} else {
				unsigned v = va_arg(ap, unsigned);
				n = put_unsigned(dp, width, precision,
                                              flags, limit, "", 0, v);
			}
			dp += (n > 0)? n: 0;
			break;
		case CONV_SPEC_OCTAL:
			n = 0;
			if (modifier == CONV_ARG_MODIFIER_LONG) {
				unsigned long v = va_arg(ap, unsigned long);
				n = put_loctal(dp, width, precision,
                                              flags, limit, "", 0, v);
#ifdef unsigned_long_long
			} else if (modifier == CONV_ARG_MODIFIER_LLONG) {
				unsigned_long_long v =
					va_arg(ap, unsigned_long_long);
				n = put_lloctal(dp, width, precision,
                                              flags, limit, "", 0, v);
#endif
			} else {
				unsigned v = va_arg(ap, unsigned);
				n = put_octal(dp, width, precision,
                                              flags, limit, "", 0, v);
			}
			dp += (n > 0)? n: 0;
			break;
		case CONV_SPEC_HEX_CAPITALS:
			flags |= CONV_FLAG_CAPITALS;
			/* and fall */
		case CONV_SPEC_HEX:
			n = 0;
			if (modifier == CONV_ARG_MODIFIER_LONG) {
				unsigned long v = va_arg(ap, unsigned long);
				n = put_lhex(dp, width, precision,
                                             flags, limit, "", 0, v);
#ifdef unsigned_long_long
			} else if (modifier == CONV_ARG_MODIFIER_LLONG) {
				unsigned_long_long v =
					va_arg(ap, unsigned_long_long);
				n = put_llhex(dp, width, precision,
                                              flags, limit, "", 0, v);
#endif
			} else {
				unsigned v = va_arg(ap, unsigned);
				n = put_hex(dp, width, precision,
                                            flags, limit, "", 0, v);
			}
			dp += (n > 0)? n: 0;
			break;
		case CONV_SPEC_POINTER:
			n = 0;
			if (sizeof(void *) <= sizeof(unsigned)) {
				unsigned v = (unsigned) va_arg(ap, void *);
				n = put_hex(dp, width, precision,
                                            flags, limit, "0x", 2, v);
			} else if (sizeof(void *) <= sizeof(unsigned long)) {
				unsigned long v =
					(unsigned long) va_arg(ap, void *);
				n = put_lhex(dp, width, precision,
                                             flags, limit, "0x", 2, v);
#ifdef unsigned_long_long
			} else if (sizeof(void*)<=sizeof(unsigned_long_long)) {
				unsigned_long_long v =
					(unsigned_long_long) va_arg(ap, void *);
				n = put_llhex(dp, width, precision,
                                              flags, limit, "0x", 2, v);
#endif
			} else {
				unsigned long v =
					(unsigned long) va_arg(ap, void *);
				n = put_lhex(dp, width, precision,
                                             flags, limit, "0x", 2, v);
			}
			dp += (n > 0)? n: 0;
			break;
		case CONV_SPEC_DECIMAL:
		case CONV_SPEC_DECIMAL_CAPITALS:
		case CONV_SPEC_SCIENTIFIC:
		case CONV_SPEC_SCIENTIFIC_CAPITALS:
		case CONV_SPEC_ADAPTIVE:
		case CONV_SPEC_ADAPTIVE_CAPITALS:
		case CONV_SPEC_HEXFLOAT:
		case CONV_SPEC_HEXFLOAT_CAPITALS:
		case CONV_SPEC_STORE_BYTES_WRITTEN:
		default: /* just copy the spec */
			f = va_arg(ap, double);
			f = f;
			while (spec < cp) {
				PUTNEXTCHAR(dp, *spec, limit);
				spec++;
			}
		}
	}
	if ((n = strlen(cp)) > 0) {
		if (dp + n < limit) {
			memcpy(dp, cp, n);
			dp += n;
			cp += n;
		} else {
			do {
				PUTNEXTCHAR(dp, *cp, limit);
			} while (*(++cp) != '\0');
		}
	}
	if (dp < limit) {
		*dp = '\0';
	} else if ((buf != NULL) && (bufsize > 0)) {
		buf[bufsize-1] = '\0';
	}
	return (dp - buf);
}

