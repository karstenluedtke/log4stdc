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
			     bfc_wstrptr_t, bfc_cwstrptr_t, wchar_t)
};

/**
 * @brief    bfc_wstring_subdata
 */
const wchar_t *
bfc_wstring_subdata(bfc_cwstrptr_t s, size_t pos)
{
	bfc_wstrptr_t s1 = (bfc_wstrptr_t) (uintptr_t) s;
	RETURN_METHCALL(bfc_string_classptr_t, s, index,(s1,pos), s->buf+pos);
}

