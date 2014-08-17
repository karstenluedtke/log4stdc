/**
 * @file bfc_string_insert_copy.c
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
 * @brief    bfc_string_insert_copy
 */
int
bfc_string_insert_copy(bfc_strptr_t s, size_t pos, bfc_cstrptr_t s2)
{
	const char *data = bfc_strdata(s2);
	size_t len = bfc_strlen(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, data, len),
			bfc_string_replace_buffer(s, pos, 0, data, len));
}

