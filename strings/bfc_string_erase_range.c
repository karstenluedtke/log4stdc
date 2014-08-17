/**
 * @file bfc_string_erase_range.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_strptr_t, bfc_cstrptr_t, char)
};

/**
 * @brief    bfc_string_erase_range
 */
int
bfc_string_erase_range(bfc_strptr_t s,bfc_iterptr_t first,bfc_iterptr_t last)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_range_buffer, (s, first, last, NULL, 0),
			bfc_string_replace_range_buffer(s,first,last,NULL,0));
}

