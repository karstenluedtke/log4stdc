/**
 * @file bfc_string_insert_char.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_insert_char
 */
int
bfc_string_insert_char(bfc_strptr_t s, bfc_iterptr_t p, int c)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			insert_char, (s, p, c),
			-ENOSYS);
}
