/**
 * @file bfc_init_basic_wstring_range.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_wstrptr_t, bfc_cwstrptr_t, wchar_t)
};

/**
 * @brief    bfc_init_basic_wstring_range
 */
int
bfc_init_basic_wstring_range(void *buf, size_t bufsize, struct mempool *pool,
				bfc_iterptr_t begin, bfc_iterptr_t end)
{
	bfc_wstrptr_t s = (bfc_wstrptr_t) buf;
	int rc;
	bfc_iterator_t zero;

	if ((rc = bfc_init_basic_wstring(s, bufsize, pool)) < 0) {
		return(rc);
	}

	bfc_init_iterator(&zero, sizeof(zero), (bfc_objptr_t) s, 0);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, &zero, &zero, begin, end),
			bfc_wstring_replace_ranges(s,&zero,&zero,begin,end));
}

