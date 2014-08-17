/**
 * @file bfc_string_compare_substrs.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_compare_substrs
 */
int
bfc_string_compare_substrs(bfc_cstrptr_t s, size_t pos1, size_t n1,
				 bfc_cstrptr_t str, size_t pos2, size_t n2)
{
	const size_t l2 = bfc_string_sublen(str, pos2, n2);
	const char *s2 = bfc_string_subdata(str, (l2 > 0)? pos2: 0);

	if ((pos1 == BFC_NPOS) || (pos1 > bfc_strlen(s))) {
		return (-ERANGE);
	}
	if ((pos2 == BFC_NPOS) || (pos2 > bfc_strlen(str))) {
		return (-ERANGE);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			compare_buffer, (s, pos1, n1, s2, l2),
			bfc_string_compare_buffer(s, pos1, n1, s2, l2));
}

