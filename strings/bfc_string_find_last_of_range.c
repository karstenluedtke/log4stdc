/**
 * @file bfc_string_find_last_of_range.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_find_last_of_range
 */
size_t
bfc_string_find_last_of_range(bfc_cobjptr_t s,
		      bfc_citerptr_t pattern, bfc_citerptr_t pattend,
		      size_t pos)
{
	long c;
	const size_t len = bfc_strlen(s);
	bfc_iterator_t it, it0, limit, patit;

	if ((bfc_iterator_distance(pattern, pattend) > 0) && (len > 0)) {
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
			patit = *pattern;
			while (bfc_iterator_distance(&patit, pattend) > 0) {
				if (bfc_iterator_getlong(&patit) == c) {
					return (bfc_iterator_position(&it));
				}
				bfc_iterator_advance(&patit, 1);
			}
			bfc_iterator_advance(&it, -1);
		}
		c = bfc_iterator_getlong(&it0);
		patit = *pattern;
		while (bfc_iterator_distance(&patit, pattend) > 0) {
			if (bfc_iterator_getlong(&patit) == c) {
				return (bfc_iterator_position(&it0));
			}
			bfc_iterator_advance(&patit, 1);
		}
	}
	return (BFC_NPOS);
}

