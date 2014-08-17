/**
 * @file bfc_string_compare_c_str.c
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

