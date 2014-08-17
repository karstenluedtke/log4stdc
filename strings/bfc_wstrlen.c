/**
 * @file bfc_wstrlen.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_wstrptr_t, bfc_cwstrptr_t, wchar_t)
};

/**
 * @brief    bfc_wstrlen
 */
size_t
bfc_wstrlen(bfc_cwstrptr_t s)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, length, (s), 0);
}

