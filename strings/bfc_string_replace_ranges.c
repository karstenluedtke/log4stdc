/**
 * @file bfc_string_replace_ranges.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_replace_ranges
 */
int
bfc_string_replace_ranges(bfc_strptr_t s, bfc_iterptr_t i1, bfc_iterptr_t i2, bfc_iterptr_t j1, bfc_iterptr_t j2)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, i1, i2, j1, j2),
			-ENOSYS);
}
