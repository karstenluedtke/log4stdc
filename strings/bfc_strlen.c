/**
 * @file bfc_strlen.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

#ifdef bfc_strlen
#undef bfc_strlen
#endif

/**
 * @brief    bfc_strlen
 */
size_t
bfc_strlen(bfc_cobjptr_t s)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, length, (s), 0);
}

