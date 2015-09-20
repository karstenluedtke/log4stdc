/**
 * @file bfc_init_shared_wstring_substr.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_init_shared_wstring_substr
 */
int
bfc_init_shared_wstring_substr(void *buf, size_t bufsize,
				bfc_cobjptr_t s, size_t pos, size_t n)
{
	if ((pos == BFC_NPOS) || (pos > bfc_wstrlen(s))) {
		return (-ERANGE);
	}
	return (bfc_init_shared_wstring_buffer(buf, bufsize,
		bfc_wstring_subdata(s, pos), bfc_wstring_sublen(s, pos, n)));
}

