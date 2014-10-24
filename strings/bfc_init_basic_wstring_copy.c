/**
 * @file bfc_init_basic_wstring_copy.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "log4stdc.h"

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_strptr_t, bfc_cstrptr_t, wchar_t)
};

/**
 * @brief    bfc_init_basic_wstring_copy
 */
int
bfc_init_basic_wstring_copy(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cstrptr_t str)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, str %p)",
		__FUNCTION__, buf, (long) bufsize, pool, str);

	return bfc_init_basic_wstring_buffer(buf, bufsize, pool,
				bfc_wstrdata(str), bfc_wstrlen(str));
}

