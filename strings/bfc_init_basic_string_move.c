/**
 * @file bfc_init_basic_string_move.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "log4stdc.h"

#define GET_STRBUF(s)		((s)->name)
#define SET_STRBUF(s,buf)	(s)->name = (buf)

/**
 * @brief    bfc_init_basic_string_move
 */
int
bfc_init_basic_string_move(void *buf, size_t bufsize, struct mempool *pool,
				bfc_strptr_t str)
{
	int rc;
	bfc_strptr_t obj = (bfc_strptr_t) buf;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, str %p)",
		__FUNCTION__, buf, (long) bufsize, pool, str);
	if ((rc = bfc_init_basic_string(obj, bufsize, pool)) < 0) {
		return(rc);
	}
	SET_STRBUF(obj, GET_STRBUF(str));
	obj->len = str->len;
	obj->bufsize = str->bufsize;
	str->bufsize = 0;
	str->len = 0;
	SET_STRBUF(str, NULL);
	return (rc);
}

