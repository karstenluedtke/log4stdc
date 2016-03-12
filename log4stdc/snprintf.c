
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

int
l4sc_snprintf(char *buf, size_t bufsize, const char *fmt, ...)
{
	int rc;
	va_list ap;

	va_start(ap, fmt);
	rc = l4sc_vsnprintf(buf, bufsize, fmt, ap);
	va_end(ap);

	return (rc);
}

#define CONV_FLAG_LEFT_ALIGN		0x0001	/* - */
#define CONV_FLAG_INCLUDE_SIGN		0x0010	/* + */
#define CONV_FLAG_SPACE_SIGN		0x0020	/*   */
#define CONV_FLAG_LEADING_ZEROES	0x0040	/* 0 */
#define CONV_FLAG_ALT_FORMAT		0x0080	/* # */
#define CONV_FLAG_SPECIFIERS		"-+ 0#"

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
#define PUT_STRING(ptr,width,precision,flags,limit,s)			\
	do {								\
		int _w = width;						\
		int _n = ((precision) >= 0)? (precision): strlen(s);	\
		while ((_w > _n) && !((flags) & CONV_FLAG_LEFT_ALIGN)) {\
			PUTNEXTCHAR(ptr, ' ', limit);			\
			_w--;						\
		}							\
		if (_n > 0) {						\
			if (ptr + _n < limit) {				\
				memcpy(ptr, s, _n);			\
			} else if (ptr < limit) {			\
				memcpy(ptr, s, limit - ptr);		\
			}						\
			ptr += _n;					\
			_w -= _n;					\
		}							\
		while (_w > 0) {					\
			PUTNEXTCHAR(ptr, ' ', limit);			\
			_w--;						\
		}							\
	} while (0 /*just once */)

	PUT_STRING(dp,width,precision,flags,limit,s);
	return (dp - buf);
}

static int
put_unsigned(char *buf, int width, int precision, int flags, const char *limit,
             const char *prefix, int pfxlen, unsigned v)
{
	char *dp = buf;
	return (dp - buf);
}

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
			PUT_STRING(dp, width, precision, flags, limit, s);
			break;
		case CONV_SPEC_INTEGER:
			n = 0;
			if (modifier == CONV_ARG_MODIFIER_LONG) {
				long v = va_arg(ap, long);
			} else if (modifier == CONV_ARG_MODIFIER_LLONG) {
				long long v = va_arg(ap, long long);
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
			if (n > 0) {
				dp += n;
			}
			break;
		case CONV_SPEC_UNSIGNED:
			break;
		case CONV_SPEC_OCTAL:
			break;
		case CONV_SPEC_HEX:
		case CONV_SPEC_HEX_CAPITALS:
			break;
		case CONV_SPEC_POINTER:
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

