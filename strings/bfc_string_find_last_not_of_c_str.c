/**
 * @file bfc_string_find_last_not_of_c_str.c
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
 * @brief    bfc_string_find_last_not_of_c_str
 */
size_t
bfc_string_find_last_not_of_c_str(bfc_cstrptr_t s, const char* s2,
					size_t pos)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_last_not_of_buffer, (s, s2, pos, n),
			bfc_string_find_last_not_of_buffer(s, s2, pos, n));
}

