/**
 * @file bfc_string_insert_buffer.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_insert_buffer
 */
int
bfc_string_insert_buffer(bfc_strptr_t s, size_t pos, const void *s2, size_t n)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			insert_buffer, (s, pos, s2, n),
			-ENOSYS);
}
