/**
 * @file bfc_string_find_last_not_of_bfstr.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_find_last_not_of_bfstr
 */
size_t
bfc_string_find_last_not_of_bfstr(bfc_cstrptr_t s, bfc_cstrptr_t str,
					size_t pos)
{
	if (BFC_CLASS(s)->traits == BFC_CLASS(str)->traits) {
		const char *pattern = bfc_strdata(str);
		size_t patlen = bfc_strlen(str);
		RETURN_METHCALL(bfc_string_classptr_t, s,
			find_last_not_of_buffer, (s, pattern, pos, patlen),
			bfc_string_find_last_not_of_buffer(s, pattern,
								pos, patlen));
	} else {
		bfc_iterator_t pattern, pattend;
		bfc_string_begin_iterator(str, &pattern, sizeof(pattern));
		bfc_string_end_iterator  (str, &pattend, sizeof(pattend));
		return bfc_string_find_last_not_of_range(s, &pattern,
								&pattend, pos);
	}
}

