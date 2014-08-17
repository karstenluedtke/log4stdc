/**
 * @file bfc_string_replace_copy.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_replace_copy
 */
int
bfc_string_replace_copy(bfc_strptr_t s, size_t pos1, size_t n1,
			  bfc_cstrptr_t str)
{
	const char *data = bfc_strdata(str);
	size_t len = bfc_strlen(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos1, n1, data, len),
			bfc_string_replace_buffer(s, pos1, n1, data, len));
}

