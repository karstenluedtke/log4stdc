/**
 * @file bfc_init_wstring_fill.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_strptr_t, bfc_cstrptr_t, wchar_t)
};

/**
 * @brief    bfc_init_wstring_fill
 */
int
bfc_init_wstring_fill(void *buf, size_t bufsize, struct mempool *pool,
				size_t n, int c)
{
	bfc_strptr_t s = (bfc_strptr_t) buf;
	int rc;

	if ((rc = bfc_init_wstring(s, bufsize, pool)) < 0) {
		return(rc);
	}

	RETURN_METHCALL(bfc_string_classptr_t, s,
			assign_fill, (s, n, c),
			bfc_wstring_assign_fill(s, n, c));
}

