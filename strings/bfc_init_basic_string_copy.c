/**
 * @file bfc_init_basic_string_copy.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "log4stdc.h"
#define  BFC_STRING_LOGGER  "barefootc.string", 16

/**
 * @brief    bfc_init_basic_string_copy
 */
int
bfc_init_basic_string_copy(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cstrptr_t str)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, str %p)",
		__FUNCTION__, buf, (long) bufsize, pool, str);

	return bfc_init_basic_string_buffer(buf, bufsize, pool,
				bfc_strdata(str), bfc_strlen(str));
}

