/**
 * @file bfc_wstring_sublen.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_objptr_t, bfc_cobjptr_t, wchar_t)
};

/**
 * @brief    bfc_wstring_sublen
 */
size_t
bfc_wstring_sublen(bfc_cobjptr_t s, size_t pos, size_t n)
{
	size_t remain, len = bfc_wstrlen(s);
	if ((pos == BFC_NPOS) || (pos >= len)) {
		return (0);
	} else if (n > (remain = len - pos)) {
		return (remain);
	}
	return (n);
}

