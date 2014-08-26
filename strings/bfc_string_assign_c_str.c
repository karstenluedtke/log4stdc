/**
 * @file bfc_string_assign_c_str.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_assign_c_str
 */
int
bfc_string_assign_c_str(bfc_strptr_t s, const void *s2)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, assign_buffer, (s, s2, n),
			bfc_string_assign_buffer(s, s2, n));
}

