/**
 * @file bfc_wstring_subdata.c
 */

#include <stddef.h>
#include <errno.h>
#include <inttypes.h>
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/unconst.h"

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_objptr_t, bfc_cobjptr_t, wchar_t)
};

/**
 * @brief    bfc_wstring_subdata
 */
const wchar_t *
bfc_wstring_subdata(bfc_cobjptr_t s, size_t pos)
{
	bfc_objptr_t s1 = BFC_UNCONST(bfc_objptr_t, s);
	RETURN_METHCALL(bfc_string_classptr_t, s,
			index, (s1, pos),
			((const wchar_t *)s->name)+pos);
}

