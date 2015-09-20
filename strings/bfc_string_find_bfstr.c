/**
 * @file bfc_string_find_bfstr.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "string_private.h"

/**
 * @brief    bfc_string_find_bfstr
 */
size_t
bfc_string_find_bfstr(bfc_cobjptr_t s, bfc_cobjptr_t str, size_t pos)
{
	if (STRING_TRAITS(s) == STRING_TRAITS(str)) {
		const char *pattern = bfc_strdata(str);
		size_t len = bfc_strlen(str);
		RETURN_METHCALL(bfc_string_classptr_t, s,
				find_buffer, (s, pattern, pos, len),
				bfc_string_find_buffer(s, pattern, pos, len));
	} else {
		bfc_iterator_t pattern, pattend;
		bfc_string_begin_iterator(str, &pattern, sizeof(pattern));
		bfc_string_end_iterator  (str, &pattend, sizeof(pattend));
		return bfc_string_find_range(s, &pattern, &pattend, pos);
	}
}

