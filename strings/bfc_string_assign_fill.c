/**
 * @file bfc_string_assign_fill.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_assign_fill
 */
int
bfc_string_assign_fill(bfc_strptr_t s, size_t n, int c)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			assign_fill, (s, n, c),
			-ENOSYS);
}
