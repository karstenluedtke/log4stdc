/**
 * @file bfc_string_compare_substr_c_str.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "string_private.h"

/**
 * @brief    bfc_string_compare_substr_c_str
 */
int
bfc_string_compare_substr_c_str(bfc_cstrptr_t s, size_t pos1, size_t n1,
				 const void *s2)
{
	size_t n2 = (*STRING_TRAITS(s)->szlen)(s2);

	if ((pos1 == BFC_NPOS) || (pos1 > bfc_strlen(s))) {
		return (-ERANGE);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			compare_buffer, (s, pos1, n1, s2, n2),
			bfc_string_compare_buffer(s, pos1, n1, s2, n2));
}

