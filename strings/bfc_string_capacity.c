/**
 * @file bfc_string_capacity.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_capacity
 */
size_t
bfc_string_capacity(bfc_cobjptr_t s)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			capacity, (s),
			-ENOSYS);
}
