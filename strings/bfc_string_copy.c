/**
 * @file bfc_string_copy.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_copy
 */
size_t
bfc_string_copy(bfc_cstrptr_t s, char* s2, size_t n, size_t pos)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			copy, (s, s2, n, pos),
			-ENOSYS);
}
