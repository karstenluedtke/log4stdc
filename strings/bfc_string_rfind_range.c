/**
 * @file bfc_string_rfind_range.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_rfind_range
 */
size_t
bfc_string_rfind_range(bfc_cstrptr_t s,
		       bfc_citerptr_t pattern, bfc_citerptr_t pattend,
		       size_t pos)
{
	const size_t len = bfc_strlen(s);
	bfc_iterator_t it, it0, limit;

	if (bfc_iterator_distance(pattern, pattend) == 0) {
		return ((pos < len)? pos: len);
	} else if (len > 0) {
		const long c = bfc_iterator_getlong(pattern);
		bfc_string_begin_iterator(s, &it0, sizeof(it0));
		bfc_string_end_iterator(s, &limit, sizeof(limit));
		it = it0;
		bfc_iterator_set_position(&it, (pos < len)? pos: len-1);
		while (bfc_iterator_distance(&it0, &it) > 0) {
			if ((bfc_iterator_getlong(&it) == c)
			 && (bfc_string_compare_next_range(&it, &limit,
						   pattern, pattend) == 0)) {
				return (bfc_iterator_position(&it));
			}
			bfc_iterator_advance(&it, -1);
		}
		if ((bfc_iterator_getlong(&it0) == c)
		 && (bfc_string_compare_next_range(&it0, &limit,
						   pattern, pattend) == 0)) {
			return (bfc_iterator_position(&it0));
		}
	}
	return (BFC_NPOS);
}

