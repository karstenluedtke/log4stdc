/**
 * @file bfc_string_sublen.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_sublen
 */
size_t
bfc_string_sublen(bfc_cobjptr_t s, size_t pos, size_t n)
{
	size_t remain, len = bfc_strlen(s);
	if ((pos == BFC_NPOS) || (pos >= len)) {
		return (0);
	} else if (n > (remain = len - pos)) {
		return (remain);
	}
	return (n);
}

