/**
 * @file bfc_string_assign_buffer.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_assign_buffer
 */
int
bfc_string_assign_buffer(bfc_objptr_t s, const void *s2, size_t n)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			assign_buffer, (s, s2,n),
			-ENOSYS);
}
