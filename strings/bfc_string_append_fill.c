/**
 * @file bfc_string_append_fill.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_append_fill
 */
int
bfc_string_append_fill(bfc_strptr_t s, size_t n, int c)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			append_fill, (s, n, c),
			-ENOSYS);
}
