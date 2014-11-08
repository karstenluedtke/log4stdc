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
	if (BFC_CLASS(s)->traits == BFC_CLASS(s2)->traits) {
		size_t len = bfc_strlen(s2);
		const char *data = bfc_strdata(s2);
		return (bfc_string_replace_range_buffer(s, i1, i2, data, len));
	} else {
		bfc_iterator_t j1, j2;
		bfc_string_begin_iterator(s2, &j1, sizeof(j1));
		bfc_string_end_iterator  (s2, &j2, sizeof(j2));
		return bfc_string_replace_ranges(s, i1, i2, &j1, &j2);
	}
}

