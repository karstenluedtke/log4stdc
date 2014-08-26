/**
 * @file bfc_string_replace_range_buffer.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_replace_range_buffer
 */
int
bfc_string_replace_range_buffer(bfc_strptr_t s,
				bfc_iterptr_t i1, bfc_iterptr_t i2,
				const void *s2, size_t n)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_range_buffer, (s, i1, i2, s2, n),
			-ENOSYS);
}
