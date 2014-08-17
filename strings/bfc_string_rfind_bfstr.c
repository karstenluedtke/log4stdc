/**
 * @file bfc_string_rfind_bfstr.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_rfind_bfstr
 */
size_t
bfc_string_rfind_bfstr(bfc_cstrptr_t s,bfc_cstrptr_t str,size_t pos)
{
	const char *pattern = bfc_strdata(str);
	size_t patlen = bfc_strlen(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			rfind_buffer, (s, pattern, pos, patlen),
			bfc_string_rfind_buffer(s, pattern, pos, patlen));
}

