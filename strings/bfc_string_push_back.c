/**
 * @file bfc_string_push_back.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_push_back
 */
int
bfc_string_push_back(bfc_strptr_t s, int c)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, push_back, (s, c), -ENOSYS);
}
