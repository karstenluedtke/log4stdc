/**
 * @file bfc_string_compare_bfstr.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "string_private.h"

/**
 * @brief    bfc_string_compare_bfstr
 */
int
bfc_string_compare_bfstr(bfc_cstrptr_t s, bfc_cstrptr_t str)
{
	size_t n1 = bfc_strlen(s);
	size_t n2 = bfc_strlen(str);
	const char *s2 = bfc_strdata(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			compare_buffer, (s, 0, n1, s2, n2),
			bfc_string_compare_buffer(s, 0, n1, s2, n2));
}

