/**
 * @file bfc_string_find_last_of_buffer.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_find_last_of_buffer
 */
size_t
bfc_string_find_last_of_buffer(bfc_cobjptr_t s,
				const void *s2, size_t pos, size_t n)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_last_of_buffer, (s, s2, pos, n),
			-ENOSYS);
}
