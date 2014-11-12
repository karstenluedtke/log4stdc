/**
 * @file bfc_string_shared_substr.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_shared_substr
 */
int
bfc_string_shared_substr(bfc_cstrptr_t s, size_t pos, size_t n,
			 void *buf, size_t bufsize)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			substr, (s, pos, n, buf, bufsize),
			-ENOSYS);
}
