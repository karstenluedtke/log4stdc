/**
 * @file bfc_init_basic_wstring_substr.c
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
 * @brief    bfc_init_basic_wstring_substr
 */
int
bfc_init_basic_wstring_substr(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cstrptr_t str, size_t pos, size_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, str %p, pos %ld, n %ld)",
		__FUNCTION__, buf, (long) bufsize, str, (long) pos, (long) n);

	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(str))) {
		return (-ERANGE);
	}
	return bfc_init_basic_wstring_buffer(buf, bufsize,
		pool? pool: bfc_basic_string_pool(str),
		bfc_wstrdata(str) + pos, bfc_wstring_sublen(str, pos, n));
}

