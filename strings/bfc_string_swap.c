/**
 * @file bfc_string_swap.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_swap
 */
void
bfc_string_swap(bfc_strptr_t s, bfc_strptr_t str)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			swap, (s, str),
			-ENOSYS);
}
