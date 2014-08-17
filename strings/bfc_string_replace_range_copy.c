/**
 * @file bfc_string_replace_range_copy.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_replace_range_copy
 */
int
bfc_string_replace_range_copy(bfc_strptr_t s, bfc_iterptr_t i1,
					bfc_iterptr_t i2, bfc_cstrptr_t s2)
{
	const char *data = bfc_strdata(s2);
	size_t len = bfc_strlen(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_range_buffer, (s, i1, i2, data, len),
			bfc_string_replace_range_buffer(s, i1, i2, data, len));
}

