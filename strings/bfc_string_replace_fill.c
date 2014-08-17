/**
 * @file bfc_string_replace_fill.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_replace_fill
 */
int
bfc_string_replace_fill(bfc_strptr_t s, size_t pos, size_t n1, size_t n2, int c)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_fill, (s, pos, n1, n2, c),
			-ENOSYS);
}
