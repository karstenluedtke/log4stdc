/**
 * @file bfc_string_erase_char.c
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
 * @brief    bfc_string_erase_char
 */
int
bfc_string_erase_char(bfc_strptr_t s, bfc_iterptr_t p)
{
	size_t pos;

	if (p->obj != (bfc_objptr_t) s) {
		return (-EFAULT);
	}
	pos = bfc_iterator_position(p);
	if ((pos == BFC_NPOS) || (pos > bfc_strlen(s))) {
		return (-ERANGE);
	}
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 1, NULL, 0),
			bfc_string_replace_buffer(s, pos, 1, NULL, 0));
}

