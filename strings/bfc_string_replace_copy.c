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
	if ((pos1 == BFC_NPOS) || (pos1 > bfc_strlen(s))) {
		return (-ERANGE);
	}
	if (BFC_CLASS(s)->traits == BFC_CLASS(str)->traits) {
		size_t len = bfc_strlen(str);
		const char *data = bfc_strdata(str);
		return (bfc_string_replace_buffer(s, pos1, n1, data, len));
	} else {
		bfc_iterator_t i1, i2, start, limit;
		bfc_string_begin_iterator(s, &start, sizeof(start));
		bfc_string_end_iterator  (s, &limit, sizeof(limit));
		i1 = start;
		while (((size_t)bfc_iterator_distance(&start, &i1) < pos1)
			    && (bfc_iterator_distance(&i1, &limit) > 0)) {
			bfc_iterator_advance(&i1, 1);
		}
		i2 = i1;
		while (((size_t)bfc_iterator_distance(&i1, &i2) < n1)
			    && (bfc_iterator_distance(&i2, &limit) > 0)) {
			bfc_iterator_advance(&i2, 1);
		}
		bfc_string_begin_iterator(str, &start, sizeof(start));
		bfc_string_end_iterator  (str, &limit, sizeof(limit));
		return bfc_string_replace_ranges(s, &i1, &i2, &start, &limit);
	}
}

