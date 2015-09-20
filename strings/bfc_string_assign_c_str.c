/**
 * @file bfc_string_assign_c_str.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "string_private.h"

/**
 * @brief    bfc_string_assign_c_str
 */
int
bfc_string_assign_c_str(bfc_objptr_t s, const void *s2)
{
	size_t n = (*STRING_TRAITS(s)->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, assign_buffer, (s, s2, n),
			bfc_string_assign_buffer(s, s2, n));
}

