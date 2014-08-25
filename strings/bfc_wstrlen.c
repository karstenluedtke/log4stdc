/**
 * @file bfc_wstrlen.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_strptr_t, bfc_cstrptr_t, wchar_t)
};

/**
 * @brief    bfc_wstrlen
 */
size_t
bfc_wstrlen(bfc_cstrptr_t s)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, length, (s), 0);
}

