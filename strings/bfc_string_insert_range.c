/**
 * @file bfc_string_insert_range.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_insert_range
 */
int
bfc_string_insert_range(bfc_strptr_t s, bfc_iterptr_t p,
			 bfc_iterptr_t first, bfc_iterptr_t last)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, p, p, first, last),
			bfc_string_replace_ranges(s, p, p, first, last));
}

