/**
 * @file bfc_string_assign_substr.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_assign_substr
 */
int
bfc_string_assign_substr(bfc_objptr_t s, bfc_cobjptr_t s2,
			 		size_t subpos, size_t sublen)
{
	const char *data = bfc_string_subdata(s2, subpos);
	size_t len = bfc_string_sublen(s2, subpos, sublen);

	if ((subpos == BFC_NPOS) || (subpos > bfc_strlen(s2))) {
		return (-ERANGE);
	}

	RETURN_METHCALL(bfc_string_classptr_t, s, assign_buffer, (s,data,len),
			bfc_string_assign_buffer(s, data, len));
}

