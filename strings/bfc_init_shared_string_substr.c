/**
 * @file bfc_init_shared_string_substr.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_init_shared_string_substr
 */
int
bfc_init_shared_string_substr(void *buf, size_t bufsize,
				bfc_cstrptr_t s, size_t pos, size_t n)
{
	if ((pos == BFC_NPOS) || (pos > bfc_string_length(s))) {
		return (-ERANGE);
	}
	return (bfc_init_shared_string_buffer(buf, bufsize,
		bfc_string_subdata(s, pos), bfc_string_sublen(s, pos, n)));
}

