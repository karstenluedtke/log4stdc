/**
 * @file bfc_string_compare_substr.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_strptr_t, bfc_cstrptr_t, char)
};

/**
 * @brief    bfc_string_compare_substr
 */
int
bfc_string_compare_substr(bfc_cstrptr_t s, size_t pos1, size_t n1,
					bfc_cstrptr_t str)
{
	size_t n2 = bfc_strlen(str);
	const char *s2 = bfc_strdata(str);

	if ((pos1 == BFC_NPOS) || (pos1 > bfc_strlen(s))) {
		return (-ERANGE);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			compare_buffer, (s, pos1, n1, s2, n2),
			bfc_string_compare_buffer(s, pos1, n1, s2, n2));
}

