
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

extern struct bfc_iterator_class bfc_forward_iterator_class;
extern struct bfc_iterator_class bfc_reverse_iterator_class;

#define FORWARD_CLASS_NAME	"utf-8 string forward iterator"
#define REVERSE_CLASS_NAME	"utf-8 string reverse iterator"

struct bfc_iterator_class bfc_forward_utf8_iterator_class = {
	.super	= &bfc_forward_iterator_class,
	.name	= FORWARD_CLASS_NAME,
	.init 	= init_iterator,
	/* Element access */
	.getl	= forward_getchar,
	.setl	= forward_putchar,
	/* Iterator functions */
	.advance    = advance_forward,
};

struct bfc_iterator_class bfc_reverse_utf8_iterator_class = {
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
bfc_init_utf8_iterator(void *buf, size_t bufsize, bfc_cobjptr_t obj, size_t pos)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	L4SC_TRACE(logger, "%s(%p, %ld, %p, %ld)",
		__FUNCTION__, buf, (long) bufsize, obj, (long) pos);

	if ((rc = init_iterator(buf, bufsize, NULL)) == BFC_SUCCESS) {
		bfc_iterptr_t it = (bfc_iterptr_t) buf;
		size_t len = bfc_object_length(obj);
		it->obj = (bfc_objptr_t) (uintptr_t) obj;
		if (pos == BFC_NPOS) {
			it->pos = len;
		} else if (pos > len) {
			it->pos = len;
			rc = -ERANGE;
		} else {
			it->pos = pos;
		}
	}
	bfc_object_dump(buf, 1, logger);
	return (rc);
}

int
bfc_init_utf8_reverse_iterator(void *buf, size_t bufsize,
				bfc_cobjptr_t obj, size_t pos)
{
	int rc;
	bfc_iterptr_t it = (bfc_iterptr_t) buf;

	rc = bfc_init_utf8_iterator(buf, bufsize, obj, pos);
	it->vptr = &bfc_reverse_utf8_iterator_class;
	if (pos == BFC_NPOS) {
		it->pos = pos; /* reverse end iterator */
	}
	return (rc);
}

static long
forward_getchar(bfc_citerptr_t it, size_t pos)
{
	uint32_t unicode;
	long rc, rc2, rc3, rc4, rc5;

	if ((rc = bfc_container_getlong(it->obj, 0)) < 0) {
		return (rc);
	}
	unicode = rc & 0xFF;
	if ((unicode >= 0xC0)
	 && ((rc = bfc_container_getlong(it->obj, 1)) >= 0)) {
		if (unicode < 0xE0) {
			unicode = ((unicode & 0x1F) << 6) | (rc & 0x3F);
		} else if (unicode < 0xF0) {
	 		if ((rc2 = bfc_container_getlong(it->obj, 2)) >= 0) {
				unicode = ((unicode & 0x0F) << 12)
					| ((rc      & 0x3F) <<  6)
					| ((rc2     & 0x3F)      );
			}
		} else if (unicode < 0xF8) {
	 		if (((rc2 = bfc_container_getlong(it->obj, 2)) >= 0)
	 		 && ((rc3 = bfc_container_getlong(it->obj, 3)) >= 0)) {
				unicode = ((unicode & 0x07) << 18)
					| ((rc      & 0x3F) << 12)
					| ((rc2     & 0x3F) <<  6)
					| ((rc3     & 0x3F)      );
			}
		} else if (unicode < 0xFC) {
	 		if (((rc2 = bfc_container_getlong(it->obj, 2)) >= 0)
	 		 && ((rc3 = bfc_container_getlong(it->obj, 3)) >= 0)
	 		 && ((rc4 = bfc_container_getlong(it->obj, 4)) >= 0)) {
				unicode = ((unicode & 0x03) << 24)
					| ((rc      & 0x3F) << 18)
					| ((rc2     & 0x3F) << 12)
					| ((rc3     & 0x3F) <<  6)
					| ((rc4     & 0x3F)      );
			}
		} else {
			if (((rc2 = bfc_container_getlong(it->obj, 2)) >= 0)
	 		 && ((rc3 = bfc_container_getlong(it->obj, 3)) >= 0)
	 		 && ((rc4 = bfc_container_getlong(it->obj, 4)) >= 0)
	 		 && ((rc5 = bfc_container_getlong(it->obj, 5)) >= 0)) {
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
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_ERROR(logger, "%s(%p, %ld, %ld) not supported",
		__FUNCTION__, it, (long) pos, unicode);

	return (-ENOSYS);
}

static int
advance_forward(bfc_iterptr_t it, ptrdiff_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	L4SC_TRACE(logger, "%s(%p, %ld)", __FUNCTION__, it, (long) n);

	bfc_object_dump(it, 1, logger);
	return (BFC_SUCCESS);
}

static long
reverse_getchar(bfc_citerptr_t it, size_t pos)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_ERROR(logger, "%s(%p, %ld) not supported",
		__FUNCTION__, it, (long) pos);

	return (-ENOSYS);
}

static int
reverse_putchar(bfc_iterptr_t it, size_t pos, long unicode)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_ERROR(logger, "%s(%p, %ld, %ld) not supported",
		__FUNCTION__, it, (long) pos, unicode);

	return (-ENOSYS);
}

static int
advance_reverse(bfc_iterptr_t it, ptrdiff_t n)
{
	return (advance_forward(it, -n));
}

