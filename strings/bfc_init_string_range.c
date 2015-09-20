/**
 * @file bfc_init_string_range.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_init_string_range
 */
int
bfc_init_string_range(void *buf, size_t bufsize, bfc_mempool_t pool,
				bfc_iterptr_t begin, bfc_iterptr_t end)
{
	bfc_objptr_t s = (bfc_objptr_t) buf;
	int rc;
	bfc_iterator_t zero;

	if ((rc = bfc_init_string(s, bufsize, pool)) < 0) {
		return(rc);
	}

	bfc_init_iterator(&zero, sizeof(zero), (bfc_objptr_t) s, 0);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, &zero, &zero, begin, end),
			bfc_string_replace_ranges(s,&zero,&zero,begin,end));
}

