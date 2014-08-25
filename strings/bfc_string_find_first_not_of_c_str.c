/**
 * @file bfc_string_find_first_not_of_c_str.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_find_first_not_of_c_str
 */
size_t
bfc_string_find_first_not_of_c_str(bfc_cstrptr_t s, const char* s2,
					size_t pos)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			find_first_not_of_buffer, (s, s2, pos, n),
			bfc_string_find_first_not_of_buffer(s, s2, pos, n));
}
