/**
 * @file bfc_string_assign_copy.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_assign_copy
 */
int
bfc_string_assign_copy(bfc_strptr_t s, bfc_cstrptr_t s2)
{
	const char *data = bfc_strdata(s2);
	size_t len = bfc_strlen(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, assign_buffer, (s,data,len),
			bfc_string_assign_buffer(s, data, len));
}

