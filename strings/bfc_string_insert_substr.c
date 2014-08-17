/**
 * @file bfc_string_insert_substr.c
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
 * @brief    bfc_string_insert_substr
 */
int
bfc_string_insert_substr(bfc_strptr_t s, size_t pos,
			bfc_cstrptr_t s2, size_t subpos, size_t sublen)
{
	const char *data = bfc_string_subdata(s2, subpos);
	size_t len = bfc_string_sublen(s2, subpos, sublen);

	if ((subpos == BFC_NPOS) || (subpos > bfc_strlen(s2))) {
		return (-ERANGE);
	}

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, data, len),
			bfc_string_replace_buffer(s, pos, 0, data, len));
}

