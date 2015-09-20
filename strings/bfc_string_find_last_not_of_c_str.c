/**
 * @file bfc_string_find_last_not_of_c_str.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "string_private.h"

/**
 * @brief    bfc_string_find_last_not_of_c_str
 */
size_t
bfc_string_find_last_not_of_c_str(bfc_cobjptr_t s, const void *s2,
					size_t pos)
{
	size_t n = (*STRING_TRAITS(s)->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_last_not_of_buffer, (s, s2, pos, n),
			bfc_string_find_last_not_of_buffer(s, s2, pos, n));
}

