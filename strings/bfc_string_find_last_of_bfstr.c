/**
 * @file bfc_string_find_last_of_bfstr.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_strptr_t, bfc_cstrptr_t, char)
};

/**
 * @brief    bfc_string_find_last_of_bfstr
 */
size_t
bfc_string_find_last_of_bfstr(bfc_cstrptr_t s, bfc_cstrptr_t str, size_t pos)
{
	const char *pattern = bfc_strdata(str);
	size_t patlen = bfc_strlen(str);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_last_of_buffer, (s, pattern, pos, patlen),
			bfc_string_find_last_of_buffer(s,pattern,pos,patlen));
}

