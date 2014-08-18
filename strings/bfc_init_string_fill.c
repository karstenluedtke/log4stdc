/**
 * @file bfc_init_string_fill.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_init_string_fill
 */
int
bfc_init_string_fill(void *buf, size_t bufsize, struct mempool *pool,
				size_t n, int c)
{
	bfc_strptr_t s = (bfc_strptr_t) buf;
	int rc;

	if ((rc = bfc_init_string(s, bufsize, pool)) < 0) {
		return(rc);
	}

	RETURN_METHCALL(bfc_string_classptr_t, s,
			assign_fill, (s, n, c),
			bfc_string_assign_fill(s, n, c));
}

