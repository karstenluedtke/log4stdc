/**
 * @file bfc_string_reserve.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_reserve
 */
int
bfc_string_reserve(bfc_objptr_t s, size_t n)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			reserve, (s, n),
			-ENOSYS);
}
