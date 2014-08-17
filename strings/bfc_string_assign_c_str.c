/**
 * @file bfc_string_assign_c_str.c
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
 * @brief    bfc_string_assign_c_str
 */
int
bfc_string_assign_c_str(bfc_strptr_t s, const char *s2)
{
	size_t n = (*s->vptr->traits->szlen)(s2);

	RETURN_METHCALL(bfc_string_classptr_t, s, assign_buffer, (s, s2, n),
			bfc_string_assign_buffer(s, s2, n));
}

