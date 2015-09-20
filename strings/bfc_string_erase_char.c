/**
 * @file bfc_string_erase_char.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_erase_char
 */
int
bfc_string_erase_char(bfc_objptr_t s, bfc_iterptr_t p)
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

