/**
 * @file bfc_string_get_char.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_get_char
 */
long
bfc_string_get_char(bfc_cobjptr_t s, size_t pos)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, getl, (s, pos), -ENOSYS);
}
