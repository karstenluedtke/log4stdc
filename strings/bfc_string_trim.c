/**
 * @file bfc_string_trim.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_trim
 */
int
bfc_string_trim(bfc_strptr_t s)
{
	size_t len = bfc_strlen(s);
	size_t start = 0, limit = len;

	while ((limit > start) && (bfc_string_get_char(s, limit-1) <= ' ')) {
		limit--;
	}
	if (limit < len) {
		bfc_string_resize(s, limit, 0);
	}
	while ((start < limit) && (bfc_string_get_char(s, start) <= ' ')) {
		start++;
	}
	if (start > 0) {
		bfc_string_erase_seq(s, 0, start);
	}
	return ((int) bfc_strlen(s));
}

