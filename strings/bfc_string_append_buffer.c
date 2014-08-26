/**
 * @file bfc_string_append_buffer.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_append_buffer
 */
int
bfc_string_append_buffer(bfc_strptr_t s, const void *s2, size_t n)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			append_buffer, (s, s2, n),
			-ENOSYS);
}
