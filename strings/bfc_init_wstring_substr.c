/**
 * @file bfc_init_wstring_substr.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "log4stdc.h"
#define  LOGGERNAME  "barefootc.string", 16

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_wstrptr_t, bfc_cwstrptr_t, wchar_t)
};

/**
 * @brief    bfc_init_wstring_substr
 */
int
bfc_init_wstring_substr(void *buf, size_t bufsize, struct mempool *pool,
			bfc_cwstrptr_t str, size_t pos, size_t n)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);

	L4SC_TRACE(logger, "%s(%p, %ld, str %p, pos %ld, n %ld)",
		__FUNCTION__, buf, (long) bufsize, str, (long) pos, (long) n);

	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(str))) {
		return (-ERANGE);
	}
	rc = bfc_init_wstring_buffer(buf, bufsize, pool,
		bfc_wstring_subdata(str, pos), bfc_wstring_sublen(str, pos, n));
	return (rc);
}

