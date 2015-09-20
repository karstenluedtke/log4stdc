/**
 * @file bfc_wstrdata.c
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
 * @brief    bfc_wstrdata
 */
const wchar_t *
bfc_wstrdata(bfc_cobjptr_t s)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			first, (s),
			(const wchar_t *) s->name);
}

