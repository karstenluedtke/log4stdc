/**
 * @file bfc_string_find_first_not_of_range.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_find_first_not_of_range
 */
size_t
bfc_string_find_first_not_of_range(bfc_cstrptr_t s,
		      bfc_citerptr_t pattern, bfc_citerptr_t pattend,
		      size_t pos)
{
	bfc_iterator_t it, limit, patit;

	if ((pos != BFC_NPOS) && (pos < bfc_strlen(s))) {
		bfc_string_begin_iterator(s, &it, sizeof(it));
		bfc_string_end_iterator(s, &limit, sizeof(limit));
		bfc_iterator_set_position(&it, pos);
		while (bfc_iterator_distance(&it, &limit) > 0) {
			const long c = bfc_iterator_getlong(&it);
			int found_in_pattern = 0;
			patit = *pattern;
			while (bfc_iterator_distance(&patit, pattend) > 0) {
				if (bfc_iterator_getlong(&patit) == c) {
					found_in_pattern = 1;
					break;
				}
				bfc_iterator_advance(&patit, 1);
			}
			if (!found_in_pattern) {
				return (bfc_iterator_position(&it));
			}
			bfc_iterator_advance(&it, 1);
		}
	}
	return (BFC_NPOS);
}

