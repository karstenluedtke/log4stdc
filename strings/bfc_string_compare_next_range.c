/**
 * @file bfc_string_compare_next_range.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_compare_next_range
 */
int
bfc_string_compare_next_range(bfc_citerptr_t it, bfc_citerptr_t limit,
			      bfc_citerptr_t pattern, bfc_citerptr_t pattend)
{
	bfc_iterator_t i1, j1;

	i1 = *it;
	j1 = *pattern;
	while (bfc_iterator_distance(&j1, pattend) > 0) {
		if (bfc_iterator_distance(&i1, limit) > 0) {
			const long c = bfc_iterator_getlong(&i1);
			const long d = bfc_iterator_getlong(&j1);
			if (c != d) {
				return ((int) (c - d));
			}
		} else {
			return (-1);
		}
		bfc_iterator_advance(&i1, 1);
		bfc_iterator_advance(&j1, 1);
	}
	return (0);
}

