/**
 * @file bfc_init_string_copy.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "log4stdc.h"
#define  LOGGERNAME  "barefootc.string", 16

/**
 * @brief    bfc_init_string_copy
 */
int
bfc_init_string_copy(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cstrptr_t str)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, str %p)",
		__FUNCTION__, buf, (long) bufsize, pool, str);

	rc = bfc_init_string_buffer(buf, bufsize, pool,
				     bfc_strdata(str), bfc_strlen(str));
	return (rc);
}

