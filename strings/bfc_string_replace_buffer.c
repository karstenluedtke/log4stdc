/**
 * @file bfc_string_replace_buffer.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_replace_buffer
 */
int
bfc_string_replace_buffer(bfc_objptr_t s, size_t pos, size_t n1,
			  const void *s2, size_t n2)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, n1, s2, n2),
			-ENOSYS);
}
