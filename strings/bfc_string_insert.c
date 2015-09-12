/**
 * @file bfc_string_insert.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "string_private.h"

/**
 * @brief    bfc_string_insert
 */
int
bfc_string_insert(bfc_strptr_t s, size_t pos, bfc_cstrptr_t s2)
{
	if ((pos == BFC_NPOS) || (pos > bfc_strlen(s))) {
		return (-ERANGE);
	}
	if (STRING_TRAITS(s) == STRING_TRAITS(s2)) {
		size_t len = bfc_strlen(s2);
		const char *data = bfc_strdata(s2);
		return (bfc_string_replace_buffer(s, pos, 0, data, len));
	} else {
		bfc_iterator_t it, start, limit;
		bfc_string_begin_iterator(s, &it, sizeof(it));
		bfc_string_end_iterator(s, &limit, sizeof(limit));
		bfc_iterator_set_position(&it, pos);
		bfc_string_begin_iterator(s2, &start, sizeof(start));
		bfc_string_end_iterator  (s2, &limit, sizeof(limit));
		return bfc_string_insert_range(s, &it, &start, &limit);
	}
}

