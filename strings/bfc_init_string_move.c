/**
 * @file bfc_init_string_move.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/unconst.h"
#include "string_private.h"
#include "log4stdc.h"

/**
 * @brief    bfc_init_string_move
 */
int
bfc_init_string_move(void *buf, size_t bufsize, bfc_strptr_t str)
{
	int rc;
	bfc_strptr_t obj = (bfc_strptr_t) buf;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, str %p)",
		__FUNCTION__, buf, (long) bufsize, str);
	if ((rc = bfc_init_string(obj, bufsize, NULL)) < 0) {
		return(rc);
	}
	SET_STRBUF(obj, GET_STRBUF(str));
	STRING_LEN(obj) = STRING_LEN(str);
	STRING_BUFSIZE(obj) = STRING_BUFSIZE(str);
	STRING_BUFSIZE(str) = 0;
	STRING_LEN(str) = 0;
	SET_STRBUF(str, NULL);
	return (rc);
}

