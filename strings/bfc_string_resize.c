/**
 * @file bfc_string_resize.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_resize
 */
int
bfc_string_resize(bfc_objptr_t s, size_t n, int c)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			resize, (s, n, c),
			-ENOSYS);
}
