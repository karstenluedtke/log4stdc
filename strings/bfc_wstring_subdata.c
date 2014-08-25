/**
 * @file bfc_wstring_subdata.c
 */

#include <stddef.h>
#include <errno.h>
#include <inttypes.h>
#include "barefootc/object.h"
#include "barefootc/string.h"

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_strptr_t, bfc_cstrptr_t, wchar_t)
};

/**
 * @brief    bfc_wstring_subdata
 */
const wchar_t *
bfc_wstring_subdata(bfc_cstrptr_t s, size_t pos)
{
	bfc_strptr_t s1 = (bfc_strptr_t) (uintptr_t) s;
	RETURN_METHCALL(bfc_string_classptr_t, s,
			index, (s1, pos),
			((const wchar_t *)s->buf)+pos);
}

