/**
 * @file bfc_string_assign.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "string_private.h"

/**
 * @brief    bfc_string_assign
 */
int
bfc_string_assign(bfc_objptr_t s, bfc_cobjptr_t s2)
{
	if (STRING_TRAITS(s) == STRING_TRAITS(s2)) {
		size_t len = bfc_strlen(s2);
		const char *data = bfc_strdata(s2);
		return (bfc_string_assign_buffer(s, data, len));
	} else {
		bfc_iterator_t start, limit;
		bfc_string_begin_iterator(s2, &start, sizeof(start));
		bfc_string_end_iterator  (s2, &limit, sizeof(limit));
		return (bfc_string_assign_range(s, &start, &limit));
	}
}

