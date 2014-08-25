/**
 * @file bfc_init_wstring_copy.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "log4stdc.h"
#define  BFC_STRING_LOGGER  "barefootc.string", 16

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_wstrptr_t, bfc_cwstrptr_t, wchar_t)
};

/**
 * @brief    bfc_init_wstring_copy
 */
int
bfc_init_wstring_copy(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cwstrptr_t str)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, str %p)",
		__FUNCTION__, buf, (long) bufsize, pool, str);

	rc = bfc_init_wstring_buffer(buf, bufsize, pool,
				     bfc_wstrdata(str), bfc_wstrlen(str));
	return (rc);
}
