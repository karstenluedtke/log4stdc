/**
 * @file bfc_string_set_char.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_set_char
 */
int
bfc_string_set_char(bfc_strptr_t s, size_t pos, long c)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, setl, (s, pos, c), -ENOSYS);
}
