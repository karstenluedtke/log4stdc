/**
 * @file bfc_string_find_last_not_of_range.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_find_last_not_of_range
 */
size_t
bfc_string_find_last_not_of_range(bfc_cstrptr_t s,
		      bfc_citerptr_t pattern, bfc_citerptr_t pattend,
		      size_t pos)
{
	long c;
	int found_in_pattern;
	const size_t len = bfc_strlen(s);
	bfc_iterator_t it, it0, limit, patit;

	if (len > 0) {
		bfc_string_begin_iterator(s, &it0, sizeof(it0));
		bfc_string_end_iterator(s, &limit, sizeof(limit));
		it = it0;
		if (pos < len) {
			bfc_iterator_set_position(&it, pos);
		} else {
			bfc_iterator_set_position(&it, len);
			bfc_iterator_advance(&it, -1);
		}
		while (bfc_iterator_distance(&it0, &it) > 0) {
			c = bfc_iterator_getlong(&it);
			found_in_pattern = 0;
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
			bfc_iterator_advance(&it, -1);
		}
		c = bfc_iterator_getlong(&it0);
		found_in_pattern = 0;
		patit = *pattern;
		while (bfc_iterator_distance(&patit, pattend) > 0) {
			if (bfc_iterator_getlong(&patit) == c) {
				found_in_pattern = 1;
				break;
			}
			bfc_iterator_advance(&patit, 1);
		}
		if (!found_in_pattern) {
			return (bfc_iterator_position(&it0));
		}
	}
	return (BFC_NPOS);
}

