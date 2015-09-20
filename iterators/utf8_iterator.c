
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/string.h"
#include "log4stdc.h"

static int  init_iterator(void *buf,size_t bufsize,struct mempool *pool);
static long forward_getchar(bfc_citerptr_t it, size_t pos);
static int  forward_putchar(bfc_iterptr_t it, size_t pos, long unicode);
static int  advance_forward(bfc_iterptr_t it, ptrdiff_t n);

static long reverse_getchar(bfc_citerptr_t it, size_t pos);
static int  reverse_putchar(bfc_iterptr_t it, size_t pos, long unicode);
static int  advance_reverse(bfc_iterptr_t it, ptrdiff_t n);

extern const struct bfc_iterator_class bfc_forward_iterator_class;
extern const struct bfc_iterator_class bfc_reverse_iterator_class;

#define FORWARD_CLASS_NAME	"utf-8 string forward iterator"
#define REVERSE_CLASS_NAME	"utf-8 string reverse iterator"

const struct bfc_iterator_class bfc_forward_utf8_iterator_class = {
	.super	= &bfc_forward_iterator_class,
	.name	= FORWARD_CLASS_NAME,
	.init 	= init_iterator,
	/* Element access */
	.getl	= forward_getchar,
	.setl	= forward_putchar,
	/* Iterator functions */
	.advance    = advance_forward,
};

const struct bfc_iterator_class bfc_reverse_utf8_iterator_class = {
	.super	= &bfc_reverse_iterator_class,
	.name	= REVERSE_CLASS_NAME,
	.init 	= init_iterator,
	/* Element access */
	.getl	= reverse_getchar,
	.setl	= reverse_putchar,
	/* Iterator functions */
	.advance    = advance_reverse,
};

static int
init_iterator(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_iterptr_t it = (bfc_iterptr_t) buf;
	if (bufsize < sizeof(*it)) {
		return (-ENOSPC);
	}
	memset(it, 0, sizeof(*it));
	it->vptr = &bfc_forward_utf8_iterator_class;
	return (BFC_SUCCESS);
}

int
bfc_init_utf8_iterator(void *buf, size_t bufsize, bfc_cobjptr_t s, size_t pos)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	L4SC_TRACE(logger, "%s(%p, %ld, %p, %ld)",
		__FUNCTION__, buf, (long) bufsize, s, (long) pos);

	if ((rc = init_iterator(buf, bufsize, NULL)) == BFC_SUCCESS) {
		bfc_iterptr_t it = (bfc_iterptr_t) buf;
		size_t len = bfc_object_length(s);
		it->obj = (bfc_objptr_t) (uintptr_t) s;
		if (pos == BFC_NPOS) {
			it->pos = len;
		} else if (pos > len) {
			it->pos = len;
			rc = -ERANGE;
		} else {
			it->pos = pos;
		}
	}
	bfc_iterator_dump((bfc_citerptr_t) buf, 1, logger);
	return (rc);
}

int
bfc_init_utf8_reverse_iterator(void *buf, size_t bufsize,
				bfc_cobjptr_t s, size_t pos)
{
	int rc;
	bfc_iterptr_t it = (bfc_iterptr_t) buf;

	rc = bfc_init_utf8_iterator(buf, bufsize, s, pos);
	it->vptr = &bfc_reverse_utf8_iterator_class;
	if (pos == BFC_NPOS) {
		it->pos = pos; /* reverse end iterator */
	}
	return (rc);
}

static long
forward_getchar(bfc_citerptr_t it, size_t pos)
{
	bfc_cstrptr_t s = (bfc_cstrptr_t) it->obj;
	size_t p = it->pos + pos;
	uint32_t unicode;
	long rc, rc2, rc3, rc4, rc5;

	if ((rc = bfc_string_get_char(s, p)) < 0x80) {
		return (rc);
	}
	unicode = rc & 0xFF;
	if ((unicode >= 0xC0)
	 && ((rc = bfc_string_get_char(s, p+1)) >= 0)) {
		if (unicode < 0xE0) {
			unicode = ((unicode & 0x1F) << 6) | (rc & 0x3F);
		} else if (unicode < 0xF0) {
	 		if ((rc2 = bfc_string_get_char(s, p+2)) >= 0) {
				unicode = ((unicode & 0x0F) << 12)
					| ((rc      & 0x3F) <<  6)
					| ((rc2     & 0x3F)      );
			}
			/* non-std encoding w/ high and low surrogates */
			if ((unicode >= 0xD800) && (unicode < 0xDC00)
	 		    && ((rc3 = bfc_string_get_char(s, p+3)) == 0xED)
	 		    && ((rc4 = bfc_string_get_char(s, p+4)) >= 0)
	 		    && ((rc5 = bfc_string_get_char(s, p+5)) >= 0)) {
				unicode = (((unicode & 0x3FF)+ 0x40) << 10)
					| ((rc4      & 0x0F) <<  6)
					| ((rc5      & 0x3F)      );
			}
		} else if (unicode < 0xF8) {
	 		if (((rc2 = bfc_string_get_char(s, p+2)) >= 0)
	 		 && ((rc3 = bfc_string_get_char(s, p+3)) >= 0)) {
				unicode = ((unicode & 0x07) << 18)
					| ((rc      & 0x3F) << 12)
					| ((rc2     & 0x3F) <<  6)
					| ((rc3     & 0x3F)      );
			}
		} else if (unicode < 0xFC) {
	 		if (((rc2 = bfc_string_get_char(s, p+2)) >= 0)
	 		 && ((rc3 = bfc_string_get_char(s, p+3)) >= 0)
	 		 && ((rc4 = bfc_string_get_char(s, p+4)) >= 0)) {
				unicode = ((unicode & 0x03) << 24)
					| ((rc      & 0x3F) << 18)
					| ((rc2     & 0x3F) << 12)
					| ((rc3     & 0x3F) <<  6)
					| ((rc4     & 0x3F)      );
			}
		} else {
			if (((rc2 = bfc_string_get_char(s, p+2)) >= 0)
	 		 && ((rc3 = bfc_string_get_char(s, p+3)) >= 0)
	 		 && ((rc4 = bfc_string_get_char(s, p+4)) >= 0)
	 		 && ((rc5 = bfc_string_get_char(s, p+5)) >= 0)) {
				unicode = ((unicode & 0x01) << 30)
					| ((rc      & 0x3F) << 24)
					| ((rc2     & 0x3F) << 18)
					| ((rc3     & 0x3F) << 12)
					| ((rc4     & 0x3F) <<  6)
					| ((rc5     & 0x3F)      );
			}
		}
	}
	return ((long) unicode);
}

static int
forward_putchar(bfc_iterptr_t it, size_t pos, long unicode)
{
	int rc;
	bfc_strptr_t s = (bfc_strptr_t) it->obj;
	size_t p = it->pos + pos;
	uint32_t u = (uint32_t) unicode;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%s %p, %ld, %ld)",
		__FUNCTION__, BFC_CLASS(it)->name, it, (long) p, (long) u);

	if ((u) <= 0x7FuL) {
		if ((rc = bfc_string_reserve(s, p+1)) >= 0) {
			rc = bfc_string_set_char(s, p, u);
		}
	} else if ((u) <= 0x7FFuL) {
		if ((rc = bfc_string_reserve(s, p+2)) >= 0) {
			bfc_string_set_char(s, p+0, 0xC0 | ((u) >> 6));
			rc = bfc_string_set_char(s, p+1, 0x80 | ((u)   & 0x3F));
		}
	} else if ((u) <= 0xFFFFuL) {
		if ((rc = bfc_string_reserve(s, p+3)) >= 0) {
			bfc_string_set_char(s, p+0, 0xE0 | ((u) >> 12));
			bfc_string_set_char(s, p+1, 0x80 |(((u) >>  6) & 0x3F));
			rc = bfc_string_set_char(s, p+2, 0x80 | ((u)   & 0x3F));
		}
	} else if ((u) <= 0x1FFFFFuL) {
		if ((rc = bfc_string_reserve(s, p+4)) >= 0) {
			bfc_string_set_char(s, p+0, 0xF0 | ((u) >> 18));
			bfc_string_set_char(s, p+1, 0x80 |(((u) >> 12) & 0x3F));
			bfc_string_set_char(s, p+2, 0x80 |(((u) >>  6) & 0x3F));
			rc = bfc_string_set_char(s, p+3, 0x80 | ((u)   & 0x3F));
		}
	} else if ((u) <= 0x3FFFFFFuL) {
		if ((rc = bfc_string_reserve(s, p+5)) >= 0) {
			bfc_string_set_char(s, p+0, 0xF8 | ((u) >> 24));
			bfc_string_set_char(s, p+1, 0x80 |(((u) >> 18) & 0x3F));
			bfc_string_set_char(s, p+2, 0x80 |(((u) >> 12) & 0x3F));
			bfc_string_set_char(s, p+3, 0x80 |(((u) >>  6) & 0x3F));
			rc = bfc_string_set_char(s, p+4, 0x80 | ((u)   & 0x3F));
		}
	} else {
		if ((rc = bfc_string_reserve(s, p+6)) >= 0) {
			bfc_string_set_char(s, p+0, 0xFC |(((u) >> 30) & 0x01));
			bfc_string_set_char(s, p+1, 0x80 |(((u) >> 24) & 0x3F));
			bfc_string_set_char(s, p+2, 0x80 |(((u) >> 18) & 0x3F));
			bfc_string_set_char(s, p+3, 0x80 |(((u) >> 12) & 0x3F));
			bfc_string_set_char(s, p+4, 0x80 |(((u) >>  6) & 0x3F));
			rc = bfc_string_set_char(s, p+5, 0x80 | ((u)   & 0x3F));
		}
	}
	return (rc);
}

static int
advance_forward(bfc_iterptr_t it, ptrdiff_t n)
{
	int i, rc;
	bfc_cstrptr_t s = (bfc_cstrptr_t) it->obj;
	const size_t len = bfc_strlen(s);
	
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	L4SC_TRACE(logger, "%s(%s %p, %ld): pos before = %ld",
		__FUNCTION__, BFC_CLASS(it)->name, it, (long)n, (long)it->pos);

	if ((size_t) n > len) /* this includes all negative values */ {
		if (n > 0) {
			it->pos = len;
			return (-ERANGE);
		} else if (n + len < 0) {
			it->pos = 0;
			return (-ERANGE);
		} else {
			return (advance_reverse(it, -n));
		}
	}

	for (i = 0; (i < (int) n) && (it->pos < len); i++) {
		if ((rc = bfc_string_get_char(s, it->pos)) < 0xC0) {
			it->pos++;
		} else if (rc < 0xE0) {
			it->pos += 2;
		} else if (rc < 0xF0) {
			if ((rc == 0xED)
			 && (forward_getchar(it, 0) >= 0x10000uL)) {
				/*non-std encoding w/ high and low surrogates*/
				it->pos += 6;
			} else {
				it->pos += 3;
			}
		} else if (rc < 0xF8) {
			it->pos += 4;
		} else if (rc < 0xFC) {
			it->pos += 5;
		} else if (rc < 0xFE) {
			it->pos += 6;
		} else {
			it->pos++; /* skip invalid code */
		}
		if (it->pos > len) {
			it->pos = len;
		}
	}
	bfc_iterator_dump(it, 1, logger);
	return (BFC_SUCCESS);
}

static int
advance_reverse(bfc_iterptr_t it, ptrdiff_t n)
{
	int i, rc;
	bfc_cstrptr_t s = (bfc_cstrptr_t) it->obj;
	const size_t len = bfc_strlen(s);
	
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	L4SC_TRACE(logger, "%s(%s %p, %ld): pos before = %ld",
		__FUNCTION__, BFC_CLASS(it)->name, it, (long)n, (long)it->pos);

	if ((size_t) n > len) /* this includes all negative values */ {
		if (n > 0) {
			it->pos = 0;
			return (-ERANGE);
		} else if (n + len < 0) {
			it->pos = len;
			return (-ERANGE);
		} else {
			return (advance_forward(it, -n));
		}
	}

	for (i = 0; (i < (int) n) && (it->pos > 0); i++) {
		do {
			it->pos--;
			rc = bfc_string_get_char(s, it->pos);
		} while ((it->pos > 0) && (rc >= 0x80) && (rc < 0xC0));
	}
	bfc_iterator_dump(it, 1, logger);
	return (BFC_SUCCESS);
}

static long
reverse_getchar(bfc_citerptr_t it, size_t pos)
{
	long rc;
	bfc_cstrptr_t s = (bfc_cstrptr_t) it->obj;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	if ((rc = bfc_string_get_char(s, it->pos)) < 0x80) {
		return (rc);
	}
	L4SC_ERROR(logger, "%s(%p, %ld) with byte %ld not supported",
		__FUNCTION__, it, (long) pos, rc);

	return (-ENOSYS);
}

static int
reverse_putchar(bfc_iterptr_t it, size_t pos, long unicode)
{
	bfc_strptr_t s = (bfc_strptr_t) it->obj;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	if ((unicode >= 0) && (unicode < 0x80)) {
		return (bfc_string_set_char(s, it->pos, unicode));
	}

	L4SC_ERROR(logger, "%s(%p, %ld, %ld) not supported",
		__FUNCTION__, it, (long) pos, unicode);

	return (-ENOSYS);
}

