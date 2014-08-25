/**
 * @file bfc_string_compare_c_str.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_compare_c_str
 */
int
bfc_string_compare_c_str(bfc_cstrptr_t s, const char* s2)
{
	size_t n1 = bfc_strlen(s);
	size_t n2 = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			compare_buffer, (s, 0, n1, s2, n2),
			bfc_string_compare_buffer(s, 0, n1, s2, n2));
}
