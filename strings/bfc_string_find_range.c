/**
 * @file bfc_string_find_range.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_find_range
 */
size_t
bfc_string_find_range(bfc_cobjptr_t s,
		      bfc_citerptr_t pattern, bfc_citerptr_t pattend,
		      size_t pos)
{
	bfc_iterator_t it, limit;

	if (bfc_iterator_distance(pattern, pattend) == 0) {
		return ((pos <= bfc_strlen(s))? pos: BFC_NPOS);
	} else if ((pos != BFC_NPOS) && (pos < bfc_strlen(s))) {
		const long c = bfc_iterator_getlong(pattern);
		bfc_string_begin_iterator(s, &it, sizeof(it));
		bfc_string_end_iterator(s, &limit, sizeof(limit));
		bfc_iterator_set_position(&it, pos);
		while (bfc_iterator_distance(&it, &limit) > 0) {
			if ((bfc_iterator_getlong(&it) == c)
			 && (bfc_string_compare_next_range(&it, &limit,
						pattern, pattend) == 0)) {
				return (bfc_iterator_position(&it));
			}
			bfc_iterator_advance(&it, 1);
		}
	}
	return (BFC_NPOS);
}

