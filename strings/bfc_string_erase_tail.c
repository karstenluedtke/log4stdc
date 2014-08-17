/**
 * @file bfc_string_erase_tail.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_erase_tail
 */
int
bfc_string_erase_tail(bfc_strptr_t s, size_t pos)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, BFC_NPOS, NULL, 0),
			bfc_string_replace_buffer(s, pos, BFC_NPOS, NULL, 0));
}

