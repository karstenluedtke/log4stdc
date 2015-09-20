/**
 * @file bfc_string_erase_seq.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_erase_seq
 */
int
bfc_string_erase_seq(bfc_objptr_t s, size_t pos, size_t len)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, len, NULL, 0),
			bfc_string_replace_buffer(s, pos, len, NULL, 0));
}

