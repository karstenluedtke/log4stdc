/**
 * @file bfc_init_string_substr.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "log4stdc.h"
#define  BFC_STRING_LOGGER  "barefootc.string", 16

/**
 * @brief    bfc_init_string_substr
 */
int
bfc_init_string_substr(void *buf, size_t bufsize, struct mempool *pool,
			bfc_cstrptr_t str, size_t pos, size_t n)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, str %p, pos %ld, n %ld)",
		__FUNCTION__, buf, (long) bufsize, str, (long) pos, (long) n);

	if ((pos == BFC_NPOS) || (pos > bfc_strlen(str))) {
		return (-ERANGE);
	}
	rc = bfc_init_string_buffer(buf, bufsize, pool,
		bfc_string_subdata(str, pos), bfc_string_sublen(str, pos, n));
	return (rc);
}
