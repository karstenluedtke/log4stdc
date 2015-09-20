/**
 * @file bfc_string_replace_substr.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_replace_substr
 */
int
bfc_string_replace_substr(bfc_objptr_t s,size_t pos1,size_t n1,
	 			bfc_cobjptr_t str, size_t pos2, size_t n2)
{
	const char *data = bfc_string_subdata(str, pos2);
	size_t len = bfc_string_sublen(str, pos2, n2);

	if ((pos2 == BFC_NPOS) || (pos2 > bfc_strlen(str))) {
		return (-ERANGE);
	}

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos1, n1, data, len),
			bfc_string_replace_buffer(s, pos1, n1, data, len));
}

