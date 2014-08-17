/**
 * @file bfc_string_append_copy.c
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
 * @brief    bfc_string_append_copy
 */
int
bfc_string_append_copy(bfc_strptr_t s, bfc_cstrptr_t s2)
{
	const char *data = bfc_strdata(s2);
	size_t len = bfc_strlen(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, append_buffer, (s,data,len),
			bfc_string_append_buffer(s, data, len));
}

