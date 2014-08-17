/**
 * @file bfc_string_find_last_not_of_char.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_find_last_not_of_char
 */
size_t
bfc_string_find_last_not_of_char(bfc_cstrptr_t s, int c, size_t pos)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_last_not_of_char, (s, c, pos),
			-ENOSYS);
}
