/**
 * @file bfc_string_length.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_length
 */
size_t
bfc_string_length(bfc_cstrptr_t s)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, length, (s), 0);
}