/**
 * @file bfc_string_replace_range_fill.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_replace_range_fill
 */
int
bfc_string_replace_range_fill(bfc_objptr_t s,
			      bfc_iterptr_t i1, bfc_iterptr_t i2,
			      size_t n, int c)
{
	bfc_iterator_t j1, j2;

	bfc_init_source_iterator(&j1, sizeof(j1), c, 0);
	bfc_init_source_iterator(&j2, sizeof(j2), c, n);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, i1, i2, &j1, &j2),
			-ENOSYS);
}

